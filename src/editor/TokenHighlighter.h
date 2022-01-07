/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2020 - Raw Material Software Limited

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 6 End-User License
   Agreement and JUCE Privacy Policy (both effective as of the 16th June 2020).

   End User License Agreement: www.juce.com/juce-6-licence
   Privacy Policy: www.juce.com/juce-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#pragma once

#include "CodeEditor.h"
#include <juce_gui_extra/juce_gui_extra.h>

class TokenHighlighter
{
public:
    struct TokenInfo
    {
        //==============================================================================================================
        int tokenId;
        int styleFlags { -1 };
    
        //==============================================================================================================
        TokenInfo(int id)
            : tokenId(id)
        {}
        
        TokenInfo(int id, int fontStyleFlags)
            : tokenId(id),
              styleFlags(fontStyleFlags)
        {}
    };
    
    TokenHighlighter() = default;
    virtual ~TokenHighlighter() = default;
    
    //==============================================================================
    /** Reads the next token from the source and returns its token type.

        This must leave the source pointing to the first character in the
        next token.
    */
    virtual TokenInfo readNextToken (juce::CodeDocument::Iterator& source) = 0;
    
    /** Returns a suggested syntax highlighting colour scheme. */
    virtual CodeEditor::ColourScheme getDefaultColourScheme() = 0;
    
private:
    JUCE_LEAK_DETECTOR (TokenHighlighter)
};
