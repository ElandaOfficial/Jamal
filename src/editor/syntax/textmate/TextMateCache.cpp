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
    @file   TextMateCache.cpp
    @date   13, January 2022

    ===============================================================
 */

#include "TextMateCache.h"
#include "TextMateParser.h"

//**********************************************************************************************************************
// region TextMateCache
//======================================================================================================================
TextMateCache& TextMateCache::getInstance() noexcept
{
    static TextMateCache cache;
    return cache;
}

//======================================================================================================================
void TextMateCache::addGrammar(TextMateGrammar grammar)
{
    if (grammarDefinitions.find(grammar.languageInfo.scopeName) != grammarDefinitions.end())
    {
        grammarDefinitions.emplace(grammar.languageInfo.scopeName, std::move(grammar));
    }
}

//======================================================================================================================
const TextMateGrammar* TextMateCache::fromFile(const juce::File &file)
{
    if (file.exists() && !file.isDirectory() && file.getFileExtension() == "tmLanguage")
    {
        const juce::String          data   = file.loadFileAsString();
        TextMateParser::ParseResult result = TextMateParser::parse(data);
        
        if (result.first == TextMateParser::ParseStatus::Success)
        {
            TextMateGrammar    &grammar = result.second;
            const juce::String name     = grammar.languageInfo.scopeName;
            
            if (grammarDefinitions.find(name) != grammarDefinitions.end())
            {
                return &grammarDefinitions.emplace(name, std::move(grammar)).first->second;
            }
        }
    }
    
    return nullptr;
}

const TextMateGrammar* TextMateCache::fromMemory(const void *jsonData, std::size_t dataSize)
{
    juce::MemoryInputStream mis(jsonData, dataSize, false);
    
    const juce::String          data   = mis.readEntireStreamAsString();
    TextMateParser::ParseResult result = TextMateParser::parse(data);
    
    if (result.first == TextMateParser::ParseStatus::Success)
    {
        TextMateGrammar    &grammar = result.second;
        const juce::String name     = grammar.languageInfo.scopeName;
        
        if (grammarDefinitions.find(name) != grammarDefinitions.end())
        {
            return &grammarDefinitions.emplace(name, std::move(grammar)).first->second;
        }
    }
    
    return nullptr;
}

//======================================================================================================================
const TextMateGrammar *TextMateCache::findForExtension(const juce::String &extension) const
{
    return nullptr;
}

//======================================================================================================================
void TextMateCache::clearCache()
{
    grammarDefinitions.clear();
}
//======================================================================================================================
// endregion TextMateCache
//**********************************************************************************************************************
