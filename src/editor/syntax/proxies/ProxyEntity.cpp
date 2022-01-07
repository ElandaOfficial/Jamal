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

#include "ProxyEntity.h"

#include "../JamlTokeniserUtils.h"
#include "../JamlTokenList.h"

bool ProxyEntity::validate(const TokenIterator &iterator) const
{
    return (*iterator == '&');
}

int ProxyEntity::processToken(TokenIterator &iterator)
{
    if (!JamlTokeniserUtils::isTagOpen(iterator) && JamlTokeniserUtils::skipIfEntity(iterator))
    {
        return JamlTokenList::Entity;
    }
    
    ++iterator;
    return DefaultToken;
}
