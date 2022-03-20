/**
    ===============================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any internal version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <https://www.gnu.org/licenses/>.

    Copyright (c) 2021 ElandaSunshine
    ===============================================================

    @author Elanda
    @file   TextMateParser.cpp
    @date   13, January 2022

    ===============================================================
 */

#include "TextMateParser.h"
#include "TextMateGrammar.h"

//**********************************************************************************************************************
// region Namespace
//======================================================================================================================
namespace
{
    bool getAndSetProperty(const juce::var &property, juce::String &destination)
    {
        if (!property.isVoid() && property.isString())
        {
            destination = property.toString();
            return true;
        }
        
        return false;
    }
    
    void getCaptures(const juce::var &root, std::unordered_map<juce::String, juce::String> &captures)
    {
        std::vector<std::pair<int, juce::String>> captures_temp;
        
        if (juce::DynamicObject *const captures_obj = root.getDynamicObject())
        {
            for (const auto &[id, val] : captures_obj->getProperties())
            {
                if (juce::DynamicObject *const entry_obj = val.getDynamicObject())
                {
                    const juce::var &var = entry_obj->getProperty("name");
                    
                    if (var.isString())
                    {
                        captures.emplace(id.toString(), var.toString());
                    }
                }
            }
        }
    }
    
    bool parsePattern(const juce::DynamicObject &pattern_root, TextMateGrammar::Rule &rule)
    {
        if (pattern_root.hasProperty("include"))
        {
            if (   pattern_root.hasProperty("match")       || pattern_root.hasProperty("begin")
                || pattern_root.hasProperty("end")         || pattern_root.hasProperty("patterns")
                || pattern_root.hasProperty("captures")    || pattern_root.hasProperty("beginCaptures")
                || pattern_root.hasProperty("endCaptures") || pattern_root.hasProperty("contentName")
                || pattern_root.hasProperty("name"))
            {
                return TextMateParser::ParseStatus::InvalidRuleBase;
            }
            
            rule.include = pattern_root.getProperty("include").toString();
            return TextMateParser::ParseStatus::Success;
        }
        
        if (pattern_root.hasProperty("match"))
        {
            if (   pattern_root.hasProperty("include")     || pattern_root.hasProperty("begin")
                || pattern_root.hasProperty("end")         || pattern_root.hasProperty("beginCaptures")
                || pattern_root.hasProperty("endCaptures") || pattern_root.hasProperty("contentName"))
            {
                return TextMateParser::ParseStatus::InvalidRuleBase;
            }
            
            juce::String match = pattern_root.getProperty("match").toString();
            
            if (match.isEmpty())
            {
                return TextMateParser::ParseStatus::MissingRuleExpression;
            }
            
            std::swap(rule.expression.beginOrMatch, match);
        }
        else if (pattern_root.hasProperty("begin") && pattern_root.hasProperty("end"))
        {
            if (pattern_root.hasProperty("include") || pattern_root.hasProperty("match"))
            {
                return TextMateParser::ParseStatus::InvalidRuleBase;
            }
            
            juce::String begin = pattern_root.getProperty("begin").toString();
            juce::String end   = pattern_root.getProperty("end")  .toString();
            
            if (begin.isEmpty() || end.isEmpty())
            {
                return TextMateParser::ParseStatus::MissingRuleExpression;
            }
            
            std::swap(rule.expression.beginOrMatch, begin);
            std::swap(rule.expression.end,          end);
            
            (void) getAndSetProperty(pattern_root.getProperty("contentName"), rule.contentName);
            getCaptures(pattern_root.getProperty("beginCaptures"), rule.captures.begin);
            getCaptures(pattern_root.getProperty("endCaptures"),   rule.captures.end);
        }
        else
        {
            return TextMateParser::ParseStatus::InvalidRuleBase;
        }
        
        if (!getAndSetProperty(pattern_root.getProperty("name"), rule.name))
        {
            return TextMateParser::ParseStatus::MissingRuleName;
        }
        
        getCaptures(pattern_root.getProperty("captures"), rule.captures.captures);
        
        {
            const juce::var &prop_sub_patterns = pattern_root.getProperty("patterns");
            
            if (prop_sub_patterns.isArray())
            {
                const juce::Array<juce::var> *const patterns = prop_sub_patterns.getArray();
                
                for (const auto &pattern : *patterns)
                {
                    if (const juce::DynamicObject *const sub_pattern_root = pattern.getDynamicObject())
                    {
                        TextMateGrammar::Rule sub_rule;
                        
                        if (const int result = ::parsePattern(*sub_pattern_root, sub_rule))
                        {
                            return result;
                        }
                        
                        rule.patterns.emplace_back(std::move(sub_rule));
                    }
                }
            }
        }
        
        return TextMateParser::ParseStatus::Success;
    }
}
//======================================================================================================================
// endregion Namespace
//**********************************************************************************************************************
// region TextMateParser
//======================================================================================================================
TextMateParser::ParseResult TextMateParser::parse(const juce::String &text)
{
    juce::var       tm_root;
    TextMateGrammar grammar;
    
    if (juce::JSON::parse(text, tm_root).wasOk())
    {
        if (const juce::DynamicObject *const root_obj = tm_root.getDynamicObject())
        {
            TextMateGrammar::LanguageInfo info;
            TextMateGrammar::Expression   foldingMarker;
            
            // Language info
            {
                if (!::getAndSetProperty(root_obj->getProperty("scopeName"), info.scopeName))
                {
                    return std::make_pair(ParseStatus::MissingScopeName, std::move(grammar));
                }
                
                (void) ::getAndSetProperty(root_obj->getProperty("firstLineMatch"), info.firstLineMatch);
                
                {
                    const juce::var &prop_file_types = root_obj->getProperty("fileTypes");
                    
                    if (!prop_file_types.isVoid() && prop_file_types.isArray())
                    {
                        if (const juce::Array<juce::var> *const types = prop_file_types.getArray())
                        {
                            std::vector<juce::String> &file_types = info.fileTypes;
                            
                            for (const auto &file_type: *types)
                            {
                                file_types.emplace_back(file_type.toString());
                            }
                        }
                    }
                }
            }
            
            // Folding markers
            if (   (::getAndSetProperty(root_obj->getProperty("foldingStartMarker"), foldingMarker.beginOrMatch)
                        != ::getAndSetProperty(root_obj->getProperty("foldingStopMarker"), foldingMarker.end))
                || (foldingMarker.beginOrMatch.isEmpty() != foldingMarker.end.isEmpty()))
            {
                return std::make_pair(ParseStatus::MissingFoldingMarkerCounterpart, std::move(grammar));
            }
            
            // Patterns
            {
                const juce::var &prop_patterns = root_obj->getProperty("patterns");
                
                if (!prop_patterns.isVoid())
                {
                    if (!prop_patterns.isArray())
                    {
                        return std::make_pair(ParseStatus::MissingPatterns, std::move(grammar));
                    }
                    
                    const juce::Array<juce::var> *const patterns = prop_patterns.getArray();
                    
                    for (const auto &pattern : *patterns)
                    {
                        if (const juce::DynamicObject *const pattern_root = pattern.getDynamicObject())
                        {
                            TextMateGrammar::Rule rule;
                            
                            if (const int result = ::parsePattern(*pattern_root, rule))
                            {
                                return std::make_pair(result, std::move(grammar));
                            }
                            
                            grammar.patterns.emplace_back(std::move(rule));
                        }
                    }
                }
            }
            
            // Repository
            {
                const juce::var &prop_repository = root_obj->getProperty("repository");
                
                if (juce::DynamicObject *const repository_entry_obj = prop_repository.getDynamicObject())
                {
                    for (const auto &[id, pattern_root] : repository_entry_obj->getProperties())
                    {
                        if (const juce::DynamicObject *const repository_pattern_obj = pattern_root.getDynamicObject())
                        {
                            TextMateGrammar::Rule rule;
                            
                            if (const int result = ::parsePattern(*repository_pattern_obj, rule))
                            {
                                return std::make_pair(result, std::move(grammar));
                            }
                            
                            grammar.repository.emplace(id.toString(), std::move(rule));
                        }
                    }
                }
            }
            
            std::swap(grammar.languageInfo,  info);
            std::swap(grammar.foldingMarker, foldingMarker);
        }
    }
    
    return std::make_pair(ParseStatus::Success, std::move(grammar));
}
//======================================================================================================================
// endregion TextMateParser
//**********************************************************************************************************************
