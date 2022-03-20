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
    @file   TextMateGrammar.h
    @date   13, January 2022

    ===============================================================
 */
 
#pragma once

#include <juce_core/juce_core.h>

struct TextMateGrammar
{
    //==================================================================================================================
    struct LanguageInfo
    {
        std::vector<juce::String> fileTypes;
        juce::String              scopeName;
        juce::String              firstLineMatch;
    };
    
    struct Expression
    {
        juce::String beginOrMatch;
        juce::String end;
    };
    
    struct Rule
    {
        //==============================================================================================================
        struct CaptureList
        {
            //==========================================================================================================
            using CaptureArray = std::unordered_map<juce::String, juce::String>;
            
            //==========================================================================================================
            CaptureArray captures;
            CaptureArray begin;
            CaptureArray end;
        };
        
        //==============================================================================================================
        CaptureList       captures;
        Expression        expression;
        std::vector<Rule> patterns;
        juce::String      name;
        juce::String      contentName;
        juce::String      include;
    };
    
    //==================================================================================================================
    LanguageInfo                           languageInfo;
    Expression                             foldingMarker;
    std::vector<Rule>                      patterns;
    std::unordered_map<juce::String, Rule> repository;
};
