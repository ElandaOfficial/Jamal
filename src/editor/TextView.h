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
    @file   TextView.h
    @date   23, January 2022

    ===============================================================
 */

#pragma once

#include "render/TextViewLayout.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <jaut_gui/jaut_gui.h>

// The text-view class for internal text and token handling
class TextView : public juce::Component
{
public:
    struct Token
    {
        int startPos; // Token start pos for that line
        int id;       // The syntax id for this token
    };
    
    struct TokenType
    {
        juce::Colour colour;     // Colour of this token type
        int          styleFlags; // Bit mask for juce::Font style flags
    };
    
    class Line
    {
    public:
        void addToken(Token newToken);
        
        //==============================================================================================================
        void updateSize(const juce::Font &font, double lineSpacingFactor);
        
        //==============================================================================================================
        Token               getToken(int index)     const noexcept;
        const juce::String& getText()               const noexcept;
        juce::String        getTokenText(int index) const noexcept;
        
        //==============================================================================================================
        int getNumTokens() const noexcept;
        
    private:
        std::vector<Token> tokens;
        juce::String       text;
        jaut::Size<double> size;
    };
    
    //==================================================================================================================
    void paint(juce::Graphics &g) override;
    void resized() override;
    
    //==================================================================================================================
    void addLine(Line newLine);
    
    template<class ForwardIt>
    void addLines(ForwardIt begin, ForwardIt end)
    {
        lines.reserve(lines.size() + std::distance(begin, end));
        lines.insert(lines.end(), begin, end);
    }
    
    //==================================================================================================================
    const TokenType&  getTokenType(int id) const noexcept;
    const juce::Font& getFont()            const noexcept;
    
    //==================================================================================================================
    void setFont(juce::Font newFont) noexcept;
    
    //==================================================================================================================
    float getLineSpacing() const noexcept { return lineSpacing; }
    
    Line&       getLine(int lineNumber)       noexcept { return lines[static_cast<std::size_t>(lineNumber)]; }
    const Line& getLine(int lineNumber) const noexcept { return lines[static_cast<std::size_t>(lineNumber)]; }
    
    //==================================================================================================================
    void setLineSpacing(float newValue) noexcept { lineSpacing = newValue; }
    
private:
    juce::ScrollBar scrollBarVertical;
    juce::ScrollBar scrollBarHorizontal;
    
    std::vector<Line>      lines;
    std::vector<TokenType> tokenTypes;
    
    TextViewLayout layout;
    TokenType      defaultTokenType;
    juce::Font     font;
    
    float lineSpacing;
};

