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

#include "JamlTokenList.h"
#include "proxies/ProxyCDataBlock.h"
#include "proxies/ProxyEntity.h"
#include "proxies/ProxyLiteral.h"
#include "proxies/ProxyOperator.h"
#include "proxies/ProxyTagOperator.h"
#include "proxies/ProxyText.h"

namespace
{
    void skipToEndOfContent(juce::CodeDocument::Iterator &source) noexcept
    {
        for (;;)
        {
            const juce::juce_wchar c = source.peekNextChar();
            
            if (c == 0 || c == '<' || c == '&' || juce::CharacterFunctions::isWhitespace(c))
            {
                break;
            }
            
            source.skip();
        }
    }
}

//======================================================================================================================
JamlTokeniser::JamlTokeniser()
{
    (void) registerProxy(std::make_unique<ProxyCDataBlock>());
    (void) registerProxy(std::make_unique<ProxyTagOperator>());
    (void) registerProxy(std::make_unique<ProxyOperator>());
    (void) registerProxy(std::make_unique<ProxyLiteral>());
    (void) registerProxy(std::make_unique<ProxyEntity>());
    (void) registerProxy(std::make_unique<ProxyText>());
}

//======================================================================================================================
JamlTokeniser::TokenInfo JamlTokeniser::readNextToken(juce::CodeDocument::Iterator &source)
{
    DBG("RUNNING");
    
    if (juce::CharacterFunctions::isWhitespace(source.peekNextChar()))
    {
        source.skipWhitespace();
        return JamlTokenList::Whitespace;
    }
    
    for (const auto &proxy : proxies)
    {
        TokenIterator iterator(source);
        
        if (proxy->validate(iterator))
        {
            if (const int result = proxy->processToken(iterator))
            {
                source = iterator.getSource();
                return result;
            }
    
            source = iterator.getSource();
            break;
        }
    }
    
    ::skipToEndOfContent(source);
    return JamlTokenList::Text;
}

CodeEditor::ColourScheme JamlTokeniser::getDefaultColourScheme()
{
    CodeEditor::ColourScheme scheme;
    
    for (const auto &[name, code] : JamlTokenList::colours)
    {
        scheme.set(name.data(), juce::Colour(code));
    }
    
    return scheme;
}

//======================================================================================================================
bool JamlTokeniser::registerProxy(std::unique_ptr<ITokenProxy> proxy)
{
    if (!proxy)
    {
        return false;
    }
    
    proxies.emplace_back(std::move(proxy));
    return true;
}
