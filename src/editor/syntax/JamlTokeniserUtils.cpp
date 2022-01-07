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
    @file   JamlTokeniserUtils.h
    @date   5, January 2022
    ===============================================================
 */

#include "JamlTokeniserUtils.h"
#include "TokenIterator.h"

void JamlTokeniserUtils::skipComment(TokenIterator &iterator) noexcept
{
    for (; *iterator != 0; ++iterator)
    {
        if (*iterator == '>' && iterator.isEndOf("-->"))
        {
            ++iterator;
            break;
        }
    }
}

void JamlTokeniserUtils::skipCData(TokenIterator &iterator) noexcept
{
    for (; *iterator != 0; ++iterator)
    {
        if (*iterator == ']' && iterator.isStartOf("]]>"))
        {
            return;
        }
    }
}

bool JamlTokeniserUtils::skipIfEntity(TokenIterator &iterator) noexcept
{
    int           length = 0;
    TokenIterator temp   = iterator;
    ++temp;
    
    if (*temp == '#')
    {
        ++temp;
        
        if (!juce::CharacterFunctions::isLetterOrDigit(*temp))
        {
            return false;
        }
    }
    else if (!juce::CharacterFunctions::isLetterOrDigit(*temp))
    {
        return false;
    }
    
    for (;;)
    {
        const juce::juce_wchar c = *temp;
        
        if (c == ';')
        {
            break;
        }
        
        if (!juce::CharacterFunctions::isLetterOrDigit(c) || c == 0)
        {
            return false;
        }
        
        ++temp;
        ++length;
    }
    
    ++iterator;
    
    if (*iterator == '#')
    {
        ++iterator;
    }
    
    iterator += (length + 1);
    return true;
}

void JamlTokeniserUtils::skipToEndOfText(TokenIterator &iterator) noexcept
{
    for(; *iterator != 0; ++iterator)
    {
        const juce::juce_wchar c = *iterator;
        
        if (c == '<' || c  == '&')
        {
            break;
        }
    }
}

//======================================================================================================================
bool JamlTokeniserUtils::isTagOpen(TokenIterator iterator) noexcept
{
    for (;; --iterator)
    {
        const juce::juce_wchar c = *iterator;
    
        if (c == '>' || c == 0)
        {
            return false;
        }
    
        if (c == '<')
        {
            break;
        }
    }
    
    ++iterator;
    
    if (*iterator == '/' || *iterator == '?')
    {
        ++iterator;
        
        if (juce::CppTokeniserFunctions::isIdentifierStart(*iterator))
        {
            return true;
        }
    }
    else if (juce::CppTokeniserFunctions::isIdentifierStart(*iterator))
    {
        return true;
    }
    
    return false;
}

bool JamlTokeniserUtils::isCDataBlock(TokenIterator iterator) noexcept
{
    --iterator;
    
    for (; *iterator != 0; --iterator)
    {
        const juce::juce_wchar c = *iterator;
        
        if (c == '>' && iterator.isEndOf("]]>"))
        {
            break;
        }
        
        if (c == '[' && iterator.isEndOf("<![CDATA["))
        {
            return true;
        }
    }
    
    return false;
}

bool JamlTokeniserUtils::isTextSegment(TokenIterator iterator) noexcept
{
    --iterator;
    
    for (;; --iterator)
    {
        const juce::juce_wchar &c = *iterator;
        
        if (c == 0 || c == '>')
        {
            return true;
        }
        
        if (c == '<')
        {
            return false;
        }
    }
}

bool JamlTokeniserUtils::isPreprocessingDirective(TokenIterator iterator) noexcept
{
    --iterator;
    
    for (; *iterator != 0; --iterator)
    {
        const juce::juce_wchar c = *iterator;
        
        if (c == '>' || c == 0)
        {
            break;
        }
        
        if (c == '<')
        {
            return (iterator.peekNext() == '?');
        }
    }
    
    return false;
}

//======================================================================================================================
bool JamlTokeniserUtils::isPrecededByIdentifier(TokenIterator iterator) noexcept
{
    bool found_at_least_one = false;
    --iterator;
    
    for (; *iterator != 0; --iterator)
    {
        const juce::juce_wchar c = *iterator;
        
        if ((juce::CharacterFunctions::isWhitespace(c) && found_at_least_one)
            || c == ':' || c == '<' || iterator.isEndOf("</"))
        {
            break;
        }
        
        if (!juce::CppTokeniserFunctions::isIdentifierBody(c) && !juce::CharacterFunctions::isWhitespace(c))
        {
            return false;
        }
        
        found_at_least_one = true;
    }
    
    return found_at_least_one && juce::CppTokeniserFunctions::isIdentifierStart(iterator.peekNext());
}

bool JamlTokeniserUtils::isFollowedByIdentifier(TokenIterator iterator) noexcept
{
    bool found_at_least_one = false;
    ++iterator;
    
    for (; *iterator != 0; ++iterator)
    {
        const juce::juce_wchar c = *iterator;
        
        if ((juce::CharacterFunctions::isWhitespace(c) && found_at_least_one)
            || c == ':' || c == '=' || c == '>' || iterator.isStartOf("/>"))
        {
            break;
        }
        
        if (!juce::CppTokeniserFunctions::isIdentifierBody(c) && !juce::CharacterFunctions::isWhitespace(c))
        {
            return false;
        }
        
        if (!found_at_least_one)
        {
            if (!juce::CharacterFunctions::isWhitespace(c) && !juce::CppTokeniserFunctions::isIdentifierStart(c))
            {
                return false;
            }
            
            found_at_least_one = true;
        }
    }
    
    return found_at_least_one;
}
