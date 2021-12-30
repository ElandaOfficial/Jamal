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
    @file   JamlTokeniser.cpp
    @date   29, Decembre 2021

    ===============================================================
 */

#include "JamlTokeniser.h"

namespace
{
    void skipToEndOfXmlComment(juce::CodeDocument::Iterator& source) noexcept
    {
        std::array<juce::juce_wchar, 2> last {};
        
        for (;;)
        {
            const juce::juce_wchar c = source.nextChar();
            
            if (c == 0 || (c == '>' && last[0] == '-' && last[1] == '-'))
            {
                break;
            }
            
            last[1] = last[0];
            last[0] = c;
        }
    }
    
    void skipToEndOfContent(juce::CodeDocument::Iterator& source) noexcept
    {
        for (;;)
        {
            const juce::juce_wchar c = source.nextChar();
            
            if (c == 0 || c == '<')
            {
                break;
            }
        }
    }
}

int JamlTokeniser::readNextToken(juce::CodeDocument::Iterator &source)
{
    source.skipWhitespace();
    
    const juce::juce_wchar first = source.peekNextChar();
    
    if (first == 0 && lastToken == TokenType::Unknown)
    {
        return JamlEntity::Invalid;
    }
    
    if (first == '\'' || first == '"')
    {
        if (lastToken == TokenType::Equal)
        {
            lastToken = JamlEntity::Literal;
            juce::CppTokeniserFunctions::skipQuotedString(source);
            
            return JamlEntity::Literal;
        }
        
        if (lastToken == TokenType::TagCloseBegin || lastToken == TokenType::TagCloseEnd
            || lastToken == TokenType::TagEmptyClose)
        {
            lastToken = TokenType::Content;
            ::skipToEndOfContent(source);
            
            return JamlEntity::Content;
        }
    }
    else if (first == '<')
    {
        const juce::juce_wchar next = source.peekNextChar();
        
        if (next == '!')
        {
            source.skip();
            
            if (source.peekNextChar() == '-')
            {
                source.skip();
                
                if (source.peekNextChar() == '-')
                {
                    ::skipToEndOfXmlComment(source);
                    lastToken = TokenType::CommentClose;
                    return JamlEntity::Comment;
                }
            }
            
            lastToken = TokenType::Unknown;
            return JamlEntity::Invalid;
        }
        
        if (juce::CharacterFunctions::isWhitespace(next))
        {
            lastToken = TokenType::Unknown;
            return JamlEntity::Invalid;
        }
        
        if (next == '/')
        {
            source.skip();
            lastToken = TokenType::CloseTagOpen;
        }
        else
        {
            lastToken = TokenType::TagOpen;
        }
        
        return JamlEntity::Punctuation;
    }
    else if (first == '>')
    {
        if (   lastToken == TokenType::TagTextClose || lastToken == TokenType::TagTextOpen
            || lastToken == TokenType::Literal)
        {
            if (lastToken == TokenType::TagTextOpen || lastToken == TokenType::Literal)
            {
                lastToken = TokenType::TagCloseBegin;
            }
            else
            {
                lastToken = TokenType::TagCloseEnd;
            }
            
            return JamlEntity::Punctuation;
        }
    }
    else if (first == '/')
    {
        if (lastToken == TokenType::TagTextOpen || lastToken == TokenType::Literal)
        {
            if (source.peekNextChar() == '>')
            {
                source.skip();
                lastToken = TokenType::TagEmptyClose;
                
                return JamlEntity::Punctuation;
            }
        }
        
        if (lastToken == TokenType::TagCloseBegin || lastToken == TokenType::TagCloseEnd
            || lastToken == TokenType::TagEmptyClose)
        {
            lastToken = TokenType::Content;
            ::skipToEndOfContent(source);
            
            return JamlEntity::Content;
        }
    }
    else if (first == '=')
    {
        if (lastToken == TokenType::AttributeText || lastToken == TokenType::Namespace)
        {
            lastToken = TokenType::Equal;
            return JamlEntity::Operator;
        }
        
        if (lastToken == TokenType::TagCloseBegin || lastToken == TokenType::TagCloseEnd
            || lastToken == TokenType::TagEmptyClose)
        {
            lastToken = TokenType::Content;
            ::skipToEndOfContent(source);
            
            return JamlEntity::Content;
        }
    }
    else if (first == ':')
    {
        if (lastToken == TokenType::AttributeText)
        {
            lastToken = TokenType::Colon;
            return JamlEntity::Operator;
        }
        
        if (lastToken == TokenType::TagCloseBegin || lastToken == TokenType::TagCloseEnd
            || lastToken == TokenType::TagEmptyClose)
        {
            lastToken = TokenType::Content;
            ::skipToEndOfContent(source);
            
            return JamlEntity::Content;
        }
    }
    else
    {
        if (lastToken == TokenType::TagOpen || lastToken == TokenType::CloseTagOpen)
        {
            if (juce::CppTokeniserFunctions::isIdentifierStart(first))
            {
                lastToken = (lastToken == TokenType::TagOpen ? TokenType::TagTextOpen : TokenType::TagTextClose);
                juce::CppTokeniserFunctions::parseIdentifier(source);
                
                return JamlEntity::Tag;
            }
        }
        else if (lastToken == TokenType::TagCloseBegin || lastToken == TokenType::TagCloseEnd
                 || lastToken == TokenType::TagEmptyClose)
        {
            lastToken = TokenType::Content;
            ::skipToEndOfContent(source);
    
            return JamlEntity::Content;
        }
        else if (lastToken == TokenType::TagTextOpen)
        {
            if (juce::CppTokeniserFunctions::isIdentifierStart(first))
            {
                lastToken = TokenType::AttributeText;
                juce::CppTokeniserFunctions::parseIdentifier(source);
                
                return JamlEntity::Attribute;
            }
        }
        else if (lastToken == TokenType::Colon)
        {
            if (juce::CppTokeniserFunctions::isIdentifierStart(first))
            {
                lastToken = TokenType::Namespace;
                juce::CppTokeniserFunctions::parseIdentifier(source);
        
                return JamlEntity::Attribute;
            }
        }
    }
    
    lastToken = TokenType::Unknown;
    return JamlEntity::Invalid;
}

juce::CodeEditorComponent::ColourScheme JamlTokeniser::getDefaultColourScheme()
{
    struct Type
    {
        const char   *name;
        juce::Colour colour;
    };
    
    static const std::array scheme_tokens
    {
        Type { "Punctuation", juce::Colours::lightgrey            },
        Type { "Operator",    juce::Colours::lightgrey            },
        Type { "Attribute",   juce::Colours::lightblue            },
        Type { "Literal",     juce::Colour(0xFFD69D85)            },
        Type { "Comment",     juce::Colours::greenyellow          },
        Type { "CData",       juce::Colours::lightgoldenrodyellow },
        Type { "Tag",         juce::Colours::lightgrey            },
        Type { "Content",     juce::Colours::lightgrey            },
        Type { "Invalid",     juce::Colours::darkred              }
    };
    
    juce::CodeEditorComponent::ColourScheme scheme;
    
    for (const auto &[name, colour] : scheme_tokens)
    {
        scheme.set(name, colour);
    }
    
    return scheme;
}
