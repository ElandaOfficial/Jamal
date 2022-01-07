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
    @file   ProxyTagOperator.cpp
    @date   06, January 2022
    ===============================================================
 */

#include "ProxyTagOperator.h"

#include "../JamlTokeniserUtils.h"
#include "../JamlTokenList.h"

bool ProxyTagOperator::validate(const TokenIterator &iterator) const
{
    const juce::juce_wchar c = *iterator;
    return (c == '<' || c == '>' || c == '/' || c == ']' || c == '?');
}

int ProxyTagOperator::processToken(TokenIterator &iterator)
{
    const juce::juce_wchar c = *iterator;
    
    switch (c)
    {
        case '<':
        {
            const juce::juce_wchar next = iterator.peekNext();
            
            if (next == '/')
            {
                ++iterator;
                
                if (juce::CppTokeniserFunctions::isIdentifierStart(iterator.peekNext()))
                {
                    ++iterator;
                    return JamlTokenList::TagOperator;
                }
            }
            else if (next == '!')
            {
                if (iterator.isStartOf("<!--"))
                {
                    JamlTokeniserUtils::skipComment(iterator);
                    return JamlTokenList::Comment;
                }
                
                if (iterator.isStartOf("<![CDATA["))
                {
                    iterator += 9;
                    return JamlTokenList::TagCData;
                }
            }
            else if (next == '?')
            {
                iterator += 2;
                
                if (juce::CppTokeniserFunctions::isIdentifierStart(*iterator))
                {
                    return JamlTokenList::TagPreprocessor;
                }
            }
            else if (juce::CppTokeniserFunctions::isIdentifierStart(next))
            {
                ++iterator;
                return JamlTokenList::TagOperator;
            }
            
            ++iterator;
            break;
        }
        
        case '>':
        {
            if (JamlTokeniserUtils::isTagOpen(iterator))
            {
                ++iterator;
                return JamlTokenList::TagOperator;
            }
            
            break;
        }
        
        case '/':
        {
            if (JamlTokeniserUtils::isTagOpen(iterator) && iterator.peekNext() == '>')
            {
                iterator += 2;
                return JamlTokenList::TagOperator;
            }
            
            break;
        }
        
        case ']':
        {
            if (iterator.isStartOf("]]>") && JamlTokeniserUtils::isCDataBlock(iterator))
            {
                iterator += 3;
                return JamlTokenList::TagCData;
            }
            
            break;
        }
        
        case '?':
        {
            if (JamlTokeniserUtils::isPreprocessingDirective(iterator) && iterator.peekNext() == '>')
            {
                iterator += 2;
                return JamlTokenList::TagPreprocessor;
            }
            
            break;
        }
        
        default: {}
    }
    
    return DefaultToken;
}
