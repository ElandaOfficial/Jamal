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
    @file   TextMateCache.h
    @date   13, January 2022

    ===============================================================
 */
 
#pragma once

#include "TextMateGrammar.h"
#include <juce_core/juce_core.h>

class TextMateCache
{
public:
    //==================================================================================================================
    static TextMateCache& getInstance() noexcept;
    
    //==================================================================================================================
    TextMateCache()  = default;
    ~TextMateCache() = default;
    
    //==================================================================================================================
    void addGrammar(TextMateGrammar grammar);
    
    //==================================================================================================================
    const TextMateGrammar* fromFile(const juce::File &file);
    const TextMateGrammar* fromMemory(const void *jsonData, std::size_t dataSize);
    
    //==================================================================================================================
    const TextMateGrammar* findForExtension(const juce::String &extension) const;
    
    //==================================================================================================================
    void clearCache();
    
private:
    std::unordered_map<juce::String, TextMateGrammar> grammarDefinitions;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextMateCache)
};
