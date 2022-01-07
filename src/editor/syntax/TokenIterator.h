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
    @file   TokenIterator.h
    @date   5, January 2022
    ===============================================================
 */

#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class TokenIterator
{
public:
    explicit TokenIterator(const juce::CodeDocument::Iterator &documentIterator) noexcept;
    
    TokenIterator(const TokenIterator &other) noexcept = default;
    TokenIterator(TokenIterator &&other)      noexcept;
    
    //==================================================================================================================
    TokenIterator& operator=(TokenIterator other) noexcept;
    
    //==================================================================================================================
    bool operator==(const TokenIterator &other) noexcept;
    bool operator!=(const TokenIterator &other) noexcept;
    
    //==================================================================================================================
    bool operator> (const TokenIterator &other) noexcept;
    bool operator< (const TokenIterator &other) noexcept;
    bool operator>=(const TokenIterator &other) noexcept;
    bool operator<=(const TokenIterator &other) noexcept;
    
    //==================================================================================================================
    juce::juce_wchar operator*() const noexcept;
    
    //==================================================================================================================
    TokenIterator& operator+=(int value) noexcept;
    TokenIterator& operator-=(int value) noexcept;
    
    //==================================================================================================================
    TokenIterator& operator++()    noexcept;
    TokenIterator  operator++(int) & noexcept;
    TokenIterator& operator--()    noexcept;
    TokenIterator  operator--(int) & noexcept;
    
    //==================================================================================================================
    TokenIterator operator+(int value) const noexcept;
    TokenIterator operator-(int value) const noexcept;
    
    //==================================================================================================================
    bool isStartOf(const juce::String &sequence)   const;
    bool isEndOf(const juce::String &sequence)     const;
    bool isFollowedByToken(juce::juce_wchar token) const noexcept;
    bool isPrecededByToken(juce::juce_wchar token) const noexcept;
    
    //==================================================================================================================
    void skipTrailingWhitespace()  noexcept;
    void skipPrecedingWhitespace() noexcept;
    
    //==================================================================================================================
    juce::juce_wchar peekNext()     const;
    juce::juce_wchar peekPrevious() const;
    
    //==================================================================================================================
    friend void swap(TokenIterator &left, TokenIterator &right) noexcept
    {
        using std::swap;
        swap(left.source,    right.source);
    }
    
    //==================================================================================================================
    juce::CodeDocument::Iterator&       getSource()       noexcept;
    const juce::CodeDocument::Iterator& getSource() const noexcept;
    
private:
    juce::CodeDocument::Iterator source;
};
