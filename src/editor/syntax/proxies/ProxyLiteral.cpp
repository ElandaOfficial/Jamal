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
    @file   Operator.cpp
    @date   06, January 2022
    ===============================================================
 */

#include "ProxyLiteral.h"

#include "../JamlTokeniserUtils.h"
#include "../JamlTokenList.h"

bool ProxyLiteral::validate(const TokenIterator &iterator) const
{
    const juce::juce_wchar c = *iterator;
    return (c == '\'' || c == '"');
}

int ProxyLiteral::processToken(TokenIterator &iterator)
{
    if (JamlTokeniserUtils::isTagOpen(iterator) && iterator.isPrecededByToken('='))
    {
        const juce::juce_wchar c = *iterator;
        juce::CppTokeniserFunctions::skipQuotedString(iterator.getSource());
        return (c == '\'' ? JamlTokenList::SingleQuoteLiteral : JamlTokenList::DoubleQuoteLiteral);
    }
    
    return DefaultToken;
}
