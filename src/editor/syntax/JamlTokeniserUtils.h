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

#pragma once

class TokenIterator;
struct JamlTokeniserUtils
{
    //==================================================================================================================
    static void skipComment(TokenIterator &iterator)     noexcept;
    static void skipCData(TokenIterator &iterator)       noexcept;
    static bool skipIfEntity(TokenIterator &iterator)    noexcept;
    static void skipToEndOfText(TokenIterator &iterator) noexcept;
    
    //==================================================================================================================
    static bool isTagOpen(TokenIterator iterator)                noexcept;
    static bool isCDataBlock(TokenIterator iterator)             noexcept;
    static bool isTextSegment(TokenIterator iterator)            noexcept;
    static bool isPreprocessingDirective(TokenIterator iterator) noexcept;
    
    //==================================================================================================================
    static bool isPrecededByIdentifier(TokenIterator iterator) noexcept;
    static bool isFollowedByIdentifier(TokenIterator iterator) noexcept;
};
