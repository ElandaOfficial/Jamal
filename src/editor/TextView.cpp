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
    @file   TextView.cpp
    @date   23, January 2022

    ===============================================================
 */

#include "TextView.h"

//**********************************************************************************************************************
// region Namespace
//======================================================================================================================
//======================================================================================================================
// endregion Namespace
//**********************************************************************************************************************
// region TextView
//======================================================================================================================
//======================================================================================================================
// region Line
//======================================================================================================================
void TextView::Line::addToken(Token newToken)
{
    tokens.emplace_back(newToken);
}

//======================================================================================================================
void TextView::Line::updateSize(const juce::Font &font, double lineSpacingFactor)
{
    size.width  = font.getStringWidthFloat(text);
    size.height = font.getHeight() * lineSpacingFactor;
}

//======================================================================================================================
const juce::String& TextView::Line::getText() const noexcept
{
    return text;
}

TextView::Token TextView::Line::getToken(int index) const noexcept
{ 
    return tokens[static_cast<std::size_t>(index)];
}

juce::String TextView::Line::getTokenText(int index) const noexcept
{
    const int num_tokens = getNumTokens();
    
    if (jaut::fit(index, 0, num_tokens))
    {
        int token_end = text.length();
        
        if (index < (num_tokens - 1))
        {
            token_end = getToken(index + 1).startPos;
        }
        
        return text.substring(getToken(index).startPos, token_end).trimEnd();
    }
    
    return "";
}

//======================================================================================================================
int TextView::Line::getNumTokens() const noexcept
{
    return static_cast<int>(tokens.size());
}

//======================================================================================================================
// endregion Line
//**********************************************************************************************************************
// region TextView
//======================================================================================================================
void TextView::paint(juce::Graphics &g)
{
    TextViewLayout layout;
    
    for (const auto &line : lines)
    {
        layout.createLayout(*this, line, )
    }
}

void TextView::resized()
{
    
}

//======================================================================================================================
void TextView::addLine(Line newLine)
{
    lines.emplace_back(std::move(newLine));
}

//======================================================================================================================
const TextView::TokenType& TextView::getTokenType(int id) const noexcept
{
    if (id > 0 && id < static_cast<int>(tokenTypes.size()))
    {
        return tokenTypes[static_cast<std::size_t>(id)];
    }
    
    return defaultTokenType;
}

const juce::Font &TextView::getFont() const noexcept
{
    return font;
}

//======================================================================================================================
void TextView::setFont(juce::Font newFont) noexcept
{
    std::swap(font, newFont);
}
//======================================================================================================================
// endregion TextView
//======================================================================================================================
//======================================================================================================================
// endregion TextView
//**********************************************************************************************************************
