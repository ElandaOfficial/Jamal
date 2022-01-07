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
    @file   TokenIterator.cpp
    @date   5, January 2022
    ===============================================================
 */

#include "TokenIterator.h"

//======================================================================================================================
TokenIterator::TokenIterator(const juce::CodeDocument::Iterator &documentIterator) noexcept
    : source(documentIterator)
{}

TokenIterator::TokenIterator(TokenIterator &&other) noexcept
{
    swap(*this, other);
}

//======================================================================================================================
TokenIterator& TokenIterator::operator=(TokenIterator other) noexcept
{
    swap(*this, other);
    return *this;
}

//======================================================================================================================
bool TokenIterator::operator==(const TokenIterator &other) noexcept
{
    return other.source.getPosition() == source.getPosition();
}

bool TokenIterator::operator!=(const TokenIterator &other) noexcept
{
    return other.source.getPosition() != source.getPosition();
}

//======================================================================================================================
bool TokenIterator::operator<(const TokenIterator &other) noexcept
{
    return source.getPosition() < other.source.getPosition();
}

bool TokenIterator::operator>(const TokenIterator &other) noexcept
{
    return source.getPosition() > other.source.getPosition();
}

bool TokenIterator::operator<=(const TokenIterator &other) noexcept
{
    return source.getPosition() <= other.source.getPosition();
}

bool TokenIterator::operator>=(const TokenIterator &other) noexcept
{
    return source.getPosition() >= other.source.getPosition();
}

//======================================================================================================================
juce::juce_wchar TokenIterator::operator*() const noexcept
{
    return source.peekNextChar();
}

//======================================================================================================================
TokenIterator& TokenIterator::operator+=(int value) noexcept
{
    for (; value > 0; --value)
    {
        source.skip();
    }
    
    return *this;
}

TokenIterator& TokenIterator::operator-=(int value) noexcept
{
    for (; value > 0; --value)
    {
        (void) source.previousChar();
    }
    
    return *this;
}

//======================================================================================================================
TokenIterator& TokenIterator::operator++() noexcept
{
    source.skip();
    return *this;
}

TokenIterator TokenIterator::operator++(int) & noexcept
{
    TokenIterator temp(*this);
    source.skip();
    return temp;
}

TokenIterator& TokenIterator::operator--() noexcept
{
    (void) source.previousChar();
    return *this;
}

TokenIterator TokenIterator::operator--(int) & noexcept
{
    TokenIterator temp(*this);
    (void) source.previousChar();
    return temp;
}

//======================================================================================================================
TokenIterator TokenIterator::operator+(int value) const noexcept
{
    TokenIterator temp(*this);
    
    for (; value > 0; --value)
    {
        (void) ++temp;
    }
    
    return temp;
}

TokenIterator TokenIterator::operator-(int value) const noexcept
{
    TokenIterator temp(*this);
    
    for (; value > 0; --value)
    {
        (void) --temp;
    }
    
    return temp;
}

//======================================================================================================================
bool TokenIterator::isStartOf(const juce::String &sequence) const
{
    juce::CodeDocument::Iterator it(source);
    
    for (const auto &cc : sequence)
    {
        const juce::juce_wchar c = it.peekNextChar();
        
        if (c == 0 || c != cc)
        {
            return false;
        }
        
        it.skip();
    }
    
    return true;
}

bool TokenIterator::isEndOf(const juce::String &sequence) const
{
    juce::CodeDocument::Iterator it(source);
    
    for (int i = sequence.length() - 1; i >= 0; --i)
    {
        const juce::juce_wchar c = it.peekNextChar();
        
        if (c != sequence[i])
        {
            return false;
        }
        
        (void) it.previousChar();
    }
    
    return true;
}

bool TokenIterator::isFollowedByToken(juce::juce_wchar token) const noexcept
{
    juce::CodeDocument::Iterator it(source);
    it.skip();
    
    for (;;)
    {
        const juce::juce_wchar c = it.peekNextChar();
        
        if (c == 0 || (c != token && !juce::CharacterFunctions::isWhitespace(c)))
        {
            return false;
        }
        
        if (c == token)
        {
            return true;
        }
        
        it.skip();
    }
}

bool TokenIterator::isPrecededByToken(juce::juce_wchar token) const noexcept
{
    juce::CodeDocument::Iterator it(source);
    
    for (;;)
    {
        const juce::juce_wchar c = it.peekPreviousChar();
        
        if (c == 0 || (c != token && !juce::CharacterFunctions::isWhitespace(c)))
        {
            return false;
        }
        
        if (c == token)
        {
            return true;
        }
        
        (void) it.previousChar();
    }
}

//======================================================================================================================
void TokenIterator::skipTrailingWhitespace() noexcept
{
    source.skipWhitespace();
}

void TokenIterator::skipPrecedingWhitespace() noexcept
{
    while (juce::CharacterFunctions::isWhitespace(source.peekPreviousChar()))
    {
        (void) source.previousChar();
    }
}

//======================================================================================================================
juce::juce_wchar TokenIterator::peekNext() const
{
    return *(*this + 1);
}

juce::juce_wchar TokenIterator::peekPrevious() const
{
    return source.peekPreviousChar();
}

//======================================================================================================================
juce::CodeDocument::Iterator       &TokenIterator::getSource()       noexcept { return source; }
const juce::CodeDocument::Iterator &TokenIterator::getSource() const noexcept { return source; }
