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
    @file   ProxyText.cpp
    @date   06, January 2022
    ===============================================================
 */

#include "ProxyText.h"

#include "../JamlTokeniserUtils.h"
#include "../JamlTokenList.h"

bool ProxyText::validate(const TokenIterator&) const
{
    return true;
}

int ProxyText::processToken(TokenIterator &iterator)
{
    if (juce::CppTokeniserFunctions::isIdentifierStart(*iterator))
    {
        TokenIterator prev_it = iterator - 1;
        
        if (*prev_it == '<' || prev_it.isEndOf("</"))
        {
            (void) juce::CppTokeniserFunctions::parseIdentifier(iterator.getSource());
            return (*iterator == ':' ? JamlTokenList::Namespace : JamlTokenList::TagName);
        }
        
        if (prev_it.isEndOf("<?"))
        {
            (void) juce::CppTokeniserFunctions::parseIdentifier(iterator.getSource());
            return JamlTokenList::TagPreprocessor;
        }
        
        if (JamlTokeniserUtils::isTagOpen(iterator))
        {
            if (iterator.isStartOf("xmlns"))
            {
                iterator += 5;
                
                if (*iterator == ':')
                {
                    return JamlTokenList::XmlnsDeclaration;
                }
            }
            
            if (*prev_it == ':')
            {
                if (prev_it.isEndOf("xmlns:"))
                {
                    juce::CppTokeniserFunctions::parseIdentifier(iterator.getSource());
                    return JamlTokenList::Namespace;
                }
                
                if (juce::CppTokeniserFunctions::isIdentifierBody(prev_it.peekPrevious()))
                {
                    --prev_it;
                    
                    for (; *prev_it != 0; --prev_it)
                    {
                        const juce::juce_wchar c = *prev_it;
                        
                        if (juce::CharacterFunctions::isWhitespace(c))
                        {
                            (void) juce::CppTokeniserFunctions::parseIdentifier(iterator.getSource());
                            return JamlTokenList::AttributeName;
                        }
                        
                        if (c == '<' || iterator.isEndOf("</"))
                        {
                            (void) juce::CppTokeniserFunctions::parseIdentifier(iterator.getSource());
                            return JamlTokenList::TagName;
                        }
                    }
                }
            }
            else if (juce::CppTokeniserFunctions::isIdentifierStart(*iterator))
            {
                (void) juce::CppTokeniserFunctions::parseIdentifier(iterator.getSource());
                return (*iterator == ':' ? JamlTokenList::Namespace : JamlTokenList::AttributeName);
            }
        }
    }
    
    return DefaultToken;
}
