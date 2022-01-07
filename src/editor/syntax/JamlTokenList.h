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
    @file   JamlTokenList.h
    @date   06, January 2022
    ===============================================================
 */

#pragma once

struct JamlTokenList
{
    //==================================================================================================================
    enum
    {
        Text,
        TagOperator,
        TagName,
        TagCData,
        TagPreprocessor,
        Comment,
        AttributeName,
        Operator,
        SingleQuoteLiteral,
        DoubleQuoteLiteral,
        Entity,
        CDataText,
        Namespace,
        XmlnsDeclaration,
        Whitespace
    };
    
    //==================================================================================================================
    struct ColourPair
    {
        std::string_view name;
        std::uint32_t    code;
    };
    
    //==================================================================================================================
    static constexpr std::array colours {
        ColourPair { "Text",               0xFFD3D3D3 }, // light grey
        ColourPair { "TagOperator",        0xFFFF0000 }, // light grey
        ColourPair { "TagName",            0xFF68BBE3 }, // keyword blue
        ColourPair { "TagCData",           0xFF9370DB }, // medium purple
        ColourPair { "TagPreprocessor",    0xFFFFC0CB }, // pink
        ColourPair { "Comment",            0xFF2E8B57 }, // sea-green
        ColourPair { "AttributeName",      0xFFADD8E6 }, // light blue
        ColourPair { "Operator",           0xFFD3D3D3 }, // light grey
        ColourPair { "SingleQuoteLiteral", 0xFFD69D85 }, // stringy red
        ColourPair { "DoubleQuoteLiteral", 0xFFD69D85 }, // stringy red
        ColourPair { "Entity",             0xFFFFFF00 }, // yellow
        ColourPair { "CDataText",          0xFFADFF2F }, // green yellow
        ColourPair { "Namespace",          0xFF696969 }, // dim grey
        ColourPair { "XmlnsDeclaration",   0xFF228B22 }, // orest green
        ColourPair { "Whitespace",         0x00000000 }  // blank 
    };
};
