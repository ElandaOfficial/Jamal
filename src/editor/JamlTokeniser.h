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
    @file   JamlTokeniser.h
    @date   29, Decembre 2021

    ===============================================================
 */

#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class JamlTokeniser : public juce::CodeTokeniser
{
public:
    JamlTokeniser() = default;
    
    //==================================================================================================================
    int readNextToken(juce::CodeDocument::Iterator &source)          override;
    juce::CodeEditorComponent::ColourScheme getDefaultColourScheme() override;
    
private:
    struct JamlEntity
    {
        enum
        {
            Punctuation,
            Operator,
            Attribute,
            Literal,
            Comment,
            CData,
            Tag,
            Content,
            Invalid
        };
    };
    
    struct TokenType
    {
        enum
        {
            Unknown,       // whatever
            TagOpen,       // <
            TagCloseBegin, // <tag>
            TagCloseEnd,   // </tag>
            TagEmptyClose, // />
            CloseTagOpen,  // </
            Escape,        // \x
            CommentOpen,   // <!--
            CommentClose,  // -->
            CDataOpen,     // <![CDATA[
            CDataClose,    // ]]>
            Literal,       // abc
            Content,       // abc
            TagTextOpen,   // <abc
            TagTextClose,  // </abc
            AttributeText, // abc
            Equal,         // =
            Colon,         // :
            Namespace      // x
        };
    };
    
    //==================================================================================================================
    int lastToken { TokenType::Unknown };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JamlTokeniser)
};
