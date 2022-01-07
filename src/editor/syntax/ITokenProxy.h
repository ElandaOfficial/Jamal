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
    @file   ITokenProxy.h
    @date   5, January 2022
    ===============================================================
 */

#pragma once

#include "TokenIterator.h"
#include <juce_core/juce_core.h>

class ITokenProxy
{
public:
    enum ReservedToken {
        DefaultToken = 0
    };
    
    //==================================================================================================================
    virtual ~ITokenProxy() = default;
    
    //==================================================================================================================
    virtual bool validate(const TokenIterator &iterator) const = 0;
    
    //==================================================================================================================
    /**
     *  Process the token.
     *  
     *  @param iterator The token iterator
     *  @return The colour id to return for this token, or 0 if the default colour should be used
     */
    virtual int processToken(TokenIterator &iterator) = 0;
};
