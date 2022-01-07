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
    @file   ProxyOperator.cpp
    @date   06, January 2022
    ===============================================================
 */

#include "ProxyOperator.h"

#include "../JamlTokeniserUtils.h"
#include "../JamlTokenList.h"

bool ProxyOperator::validate(const TokenIterator &iterator) const
{
    const juce::juce_wchar c = *iterator;
    return (c == ':' || c == '=');
}

int ProxyOperator::processToken(TokenIterator &iterator)
{
    if (JamlTokeniserUtils::isTagOpen(iterator) && JamlTokeniserUtils::isPrecededByIdentifier(iterator))
    {
        const juce::juce_wchar c = *iterator;
        
        if (   (c == ':' && JamlTokeniserUtils::isFollowedByIdentifier(iterator))
            || (c == '=' && (iterator.isFollowedByToken('\'') || iterator.isFollowedByToken('"'))))
        {
            ++iterator;
            return JamlTokenList::Operator;
        }
    }
    
    return DefaultToken;
}
