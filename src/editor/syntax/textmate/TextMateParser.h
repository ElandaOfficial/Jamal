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
    @file   TextMateParser.h
    @date   13, January 2022

    ===============================================================
 */

#pragma once

#include <juce_core/juce_core.h>

class TextMateGrammar;
class TextMateParser
{
public:
    using ParseResult = std::pair<int, TextMateGrammar>;
    
    //==================================================================================================================
    struct ParseStatus
    {
        enum
        {
            Success,
            MissingScopeName,
            MissingFoldingMarkerCounterpart,
            MissingPatterns,
            MissingRuleName,
            MissingRuleExpression,
            AmbiguousRuleExpression,
            InvalidRuleBase,
            InvalidRuleCapture,
            NoRepositoryPatternFound
        };
    };
    
    //==================================================================================================================
    static ParseResult parse(const juce::String &text);
};
