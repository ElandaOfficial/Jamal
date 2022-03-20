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
    @file   TextViewLayout.cpp
    @date   23, January 2022

    ===============================================================
 */

#include "TextViewLayout.h"

struct Token
{
    Token(const juce::String& t, const juce::Font& f, juce::Colour c, bool whitespace)
        : text (t), font (f), colour (c),
          area (font.getStringWidthFloat (t), f.getHeight()),
          isWhitespace (whitespace),
          isNewLine (t.containsChar ('\n') || t.containsChar ('\r'))
    {}
    
    const juce::String text;
    const juce::Font font;
    const juce::Colour colour;
    juce::Rectangle<float> area;
    int line;
    float lineHeight;
    const bool isWhitespace, isNewLine;
    
    Token& operator= (const Token&) = delete;
};

struct TokenList
{
    TokenList() noexcept = default;
    
    void createLayout (const TextView& text, const TextView::Line &line, TextViewLayout& layout)
    {
        layout.ensureStorageAllocated (totalLines);
        
        addTextRuns(text, line);
        layoutRuns (text.getLineSpacing());
        
        int charPosition = 0;
        int lineStartPosition = 0;
        int runStartPosition = 0;
        
        std::unique_ptr<TextViewLayout::Line> currentLine;
        std::unique_ptr<TextViewLayout::Run> currentRun;
        
        bool needToSetLineOrigin = true;
        
        for (int i = 0; i < tokens.size(); ++i)
        {
            auto& t = *tokens.getUnchecked (i);
            
            juce::Array<int> newGlyphs;
            juce::Array<float> xOffsets;
            t.font.getGlyphPositions (getTrimmedEndIfNotAllWhitespace (t.text), newGlyphs, xOffsets);
            
            if (currentRun == nullptr)  currentRun  = std::make_unique<TextViewLayout::Run>();
            if (currentLine == nullptr) currentLine = std::make_unique<TextViewLayout::Line>();
            
            const auto numGlyphs = newGlyphs.size();
            charPosition += numGlyphs;
            
            if (numGlyphs > 0 && (! (t.isWhitespace || t.isNewLine) || needToSetLineOrigin))
            {
                currentRun->glyphs.ensureStorageAllocated (currentRun->glyphs.size() + newGlyphs.size());
                auto tokenOrigin = t.area.getPosition().translated (0, t.font.getAscent());
                
                if (needToSetLineOrigin)
                {
                    needToSetLineOrigin = false;
                    currentLine->lineOrigin = tokenOrigin; // NOLINT
                }
                
                auto glyphOffset = tokenOrigin - currentLine->lineOrigin;
                
                for (int j = 0; j < newGlyphs.size(); ++j)
                {
                    auto x = xOffsets.getUnchecked (j);
                    currentRun->glyphs.add (TextViewLayout::Glyph(newGlyphs.getUnchecked (j),
                                                                  glyphOffset.translated (x, 0),
                                                                  xOffsets.getUnchecked (j + 1) - x));
                }
            }
            
            if (auto* nextToken = tokens[i + 1])
            {
                if (t.font != nextToken->font || t.colour != nextToken->colour)
                {
                    addRun (*currentLine, currentRun.release(), t, runStartPosition, charPosition);
                    runStartPosition = charPosition;
                }
                
                if (t.line != nextToken->line)
                {
                    if (currentRun == nullptr)
                        currentRun = std::make_unique<TextViewLayout::Run>();
                    
                    addRun (*currentLine, currentRun.release(), t, runStartPosition, charPosition);
                    currentLine->stringRange = { lineStartPosition, charPosition };
                    
                    if (! needToSetLineOrigin)
                        layout.addLine (std::move (currentLine));
                    
                    runStartPosition = charPosition;
                    lineStartPosition = charPosition;
                    needToSetLineOrigin = true;
                }
            }
            else
            {
                addRun (*currentLine, currentRun.release(), t, runStartPosition, charPosition);
                currentLine->stringRange = { lineStartPosition, charPosition };
                
                if (! needToSetLineOrigin)
                    layout.addLine (std::move (currentLine));
                
                needToSetLineOrigin = true;
            }
        }
        
        const juce::Justification justification = juce::Justification::centredLeft;
        
        if ((justification.getFlags() & (juce::Justification::right | juce::Justification::horizontallyCentred)) != 0)
        {
            auto totalW = layout.width;
            bool isCentred = (justification.getFlags() & juce::Justification::horizontallyCentred) != 0;
            
            for (auto* lay_line : layout.lines)
            {
                auto dx = totalW - lay_line->getLineBoundsX().getLength();
                
                if (isCentred)
                    dx /= 2.0f;
                
                lay_line->lineOrigin.x += dx;
            }
        }
    }

private:
    static void addRun (TextViewLayout::Line& glyphLine, TextViewLayout::Run* glyphRun,
                        const Token& t, int start, int end)
    {
        glyphRun->stringRange = { start, end };
        glyphRun->font = t.font;
        glyphRun->colour = t.colour;
        glyphLine.ascent  = juce::jmax(glyphLine.ascent,  t.font.getAscent());
        glyphLine.descent = juce::jmax(glyphLine.descent, t.font.getDescent());
        glyphLine.runs.add (glyphRun);
    }
    
    static int getCharacterType (juce::juce_wchar c) noexcept
    {
        if (c == '\r' || c == '\n')
            return 0;
        
        return juce::CharacterFunctions::isWhitespace (c) ? 2 : 1;
    }
    
    void appendText (const juce::String& stringText, const juce::Font& font, juce::Colour colour)
    {
        auto t = stringText.getCharPointer();
        juce::String currentString;
        int lastCharType = 0;
        
        for (;;)
        {
            auto c = t.getAndAdvance();
            
            if (c == 0)
                break;
            
            auto charType = getCharacterType (c);
            
            if (charType == 0 || charType != lastCharType)
            {
                if (currentString.isNotEmpty())
                    tokens.add (new Token (currentString, font, colour,
                                           lastCharType == 2 || lastCharType == 0));
                
                currentString = juce::String::charToString (c);
                
                if (c == '\r' && *t == '\n')
                    currentString += t.getAndAdvance();
            }
            else
            {
                currentString += c;
            }
            
            lastCharType = charType;
        }
        
        if (currentString.isNotEmpty())
            tokens.add (new Token (currentString, font, colour, lastCharType == 2));
    }
    
    void layoutRuns(float extraLineSpacing)
    {
        float x = 0, y = 0, h = 0;
        int i;
        
        for (i = 0; i < tokens.size(); ++i)
        {
            auto& t = *tokens.getUnchecked (i);
            t.area.setPosition (x, y);
            t.line = totalLines;
            x += t.area.getWidth();
            h = juce::jmax(h, t.area.getHeight() + extraLineSpacing);
            
            auto* nextTok = tokens[i + 1];
            
            if (nextTok == nullptr)
                break;
            
            if (t.isNewLine)
            {
                setLastLineHeight (i + 1, h);
                x = 0;
                y += h;
                h = 0;
                ++totalLines;
            }
        }
        
        setLastLineHeight (juce::jmin(i + 1, tokens.size()), h);
        ++totalLines;
    }
    
    void setLastLineHeight (int i, float height) noexcept
    {
        while (--i >= 0)
        {
            auto& tok = *tokens.getUnchecked (i);
            
            if (tok.line == totalLines)
                tok.lineHeight = height;
            else
                break;
        }
    }
    
    void addTextRuns (const TextView& text, const TextView::Line &line)
    {
        auto numAttributes = line.getNumTokens();
        tokens.ensureStorageAllocated(juce::jmax(64, numAttributes));
        
        juce::Font font = text.getFont();
        
        for (int i = 0; i < numAttributes; ++i)
        {
            const TextView::Token     &token    = line.getToken(i);
            const TextView::TokenType &type     = text.getTokenType(token.id);
            const int                 range_end = (i == (numAttributes - 1) ? line.getText().length()
                                                                            : line.getToken(i + 1).startPos);
            
            font.setStyleFlags(type.styleFlags);
            appendText(line.getText().substring(token.startPos, range_end), font, type.colour);
        }
    }
    
    static juce::String getTrimmedEndIfNotAllWhitespace (const juce::String& s)
    {
        auto trimmed = s.trimEnd();
        
        if (trimmed.isEmpty() && s.isNotEmpty())
            trimmed = s.replaceCharacters ("\r\n\t", "   ");
        
        return trimmed;
    }
    
    juce::OwnedArray<Token> tokens;
    int totalLines = 0;
    
    JUCE_DECLARE_NON_COPYABLE (TokenList)
};

TextViewLayout::Glyph::Glyph (int glyph, juce::Point<float> anch, float w) noexcept
    : anchor(anch), width(w), glyphCode(glyph)
{
}

//==============================================================================
TextViewLayout::Run::Run(juce::Range<int> range, int numGlyphsToPreallocate)
    : stringRange (range)
{
    glyphs.ensureStorageAllocated (numGlyphsToPreallocate);
}

juce::Range<float> TextViewLayout::Run::getRunBoundsX() const noexcept
{
    juce::Range<float> range;
    bool isFirst = true;
    
    for (auto& glyph : glyphs)
    {
        juce::Range<float> r(glyph.anchor.x, glyph.anchor.x + glyph.width);
        
        if (isFirst)
        {
            isFirst = false;
            range = r;
        }
        else
        {
            range = range.getUnionWith (r);
        }
    }
    
    return range;
}

//==============================================================================
TextViewLayout::Line::Line(juce::Range<int> range, juce::Point<float> o, float asc, float desc,
                           float lead, int numRunsToPreallocate)
    : stringRange (range), lineOrigin (o),
      ascent (asc), descent (desc), leading (lead)
{
    runs.ensureStorageAllocated (numRunsToPreallocate);
}

TextViewLayout::Line::Line(const Line& other)
    : stringRange (other.stringRange), lineOrigin (other.lineOrigin),
      ascent (other.ascent), descent (other.descent), leading (other.leading)
{
    runs.addCopiesOf (other.runs);
}

TextViewLayout::Line& TextViewLayout::Line::operator= (const Line& other)
{
    auto copy = other;
    swap (copy);
    return *this;
}

juce::Range<float> TextViewLayout::Line::getLineBoundsX() const noexcept
{
    juce::Range<float> range;
    bool isFirst = true;
    
    for (auto* run : runs)
    {
        auto runRange = run->getRunBoundsX();
        
        if (isFirst)
        {
            isFirst = false;
            range = runRange;
        }
        else
        {
            range = range.getUnionWith (runRange);
        }
    }
    
    return range + lineOrigin.x;
}

juce::Range<float> TextViewLayout::Line::getLineBoundsY() const noexcept
{
    return { lineOrigin.y - ascent,
             lineOrigin.y + descent };
}

juce::Rectangle<float> TextViewLayout::Line::getLineBounds() const noexcept
{
    auto x = getLineBoundsX();
    auto y = getLineBoundsY();
    
    return { x.getStart(), y.getStart(), x.getLength(), y.getLength() };
}

void TextViewLayout::Line::swap (Line& other) noexcept
{
    std::swap (other.runs,          runs);
    std::swap (other.stringRange,   stringRange);
    std::swap (other.lineOrigin,    lineOrigin);
    std::swap (other.ascent,        ascent);
    std::swap (other.descent,       descent);
    std::swap (other.leading,       leading);
}

//==============================================================================
TextViewLayout::TextViewLayout()
    : width (0), height (0), justification (juce::Justification::centredLeft)
{
}

void TextViewLayout::ensureStorageAllocated (int numLinesNeeded)
{
    lines.ensureStorageAllocated (numLinesNeeded);
}

void TextViewLayout::addLine (std::unique_ptr<Line> line)
{
    lines.add (line.release());
}

void TextViewLayout::createLine(const TextView &view, const TextView::Line &line, float maxWidth)
{
    width = maxWidth;
    height = 1.0e7f;
    justification = juce::Justification::centredLeft;
    
    createStandardLayout(text, line);
    
    recalculateSize();
}

void TextViewLayout::draw(juce::Graphics& g, juce::Rectangle<float> area) const
{
    auto origin = justification.appliedToRectangle(juce::Rectangle<float> (width, height), area).getPosition();
    
    auto& context   = g.getInternalContext();
    context.saveState();
    
    auto clip       = context.getClipBounds();
    auto clipTop    = (float) clip.getY()      - origin.y;
    auto clipBottom = (float) clip.getBottom() - origin.y;
    
    for (auto* line : lines)
    {
        auto lineRangeY = line->getLineBoundsY();
        
        if (lineRangeY.getEnd() < clipTop)
            continue;
        
        if (lineRangeY.getStart() > clipBottom)
            break;
        
        auto lineOrigin = origin + line->lineOrigin;
        
        for (auto* run : line->runs)
        {
            context.setFont (run->font);
            context.setFill (run->colour);
            
            for (auto& glyph : run->glyphs)
                context.drawGlyph (glyph.glyphCode, juce::AffineTransform::translation (lineOrigin.x + glyph.anchor.x,
                                                                                        lineOrigin.y + glyph.anchor.y));
            
            if (run->font.isUnderlined())
            {
                auto runExtent = run->getRunBoundsX();
                auto lineThickness = run->font.getDescent() * 0.3f;
                
                context.fillRect ({ runExtent.getStart() + lineOrigin.x, lineOrigin.y + lineThickness * 2.0f,
                                    runExtent.getLength(), lineThickness });
            }
        }
    }
    
    context.restoreState();
}

void TextViewLayout::createLayout(const TextView& text, const TextView::Line &line, float maxWidth)
{
    lines.clear();
    width = maxWidth;
    height = 1.0e7f;
    justification = juce::Justification::centredLeft;
    
    createStandardLayout(text, line);
    
    recalculateSize();
}

void TextViewLayout::recalculateSize()
{
    if (! lines.isEmpty())
    {
        auto bounds = lines.getFirst()->getLineBounds();
        
        for (auto* line : lines)
            bounds = bounds.getUnion (line->getLineBounds());
        
        for (auto* line : lines)
            line->lineOrigin.x -= bounds.getX();
        
        width  = bounds.getWidth();
        height = bounds.getHeight();
    }
    else
    {
        width = 0;
        height = 0;
    }
}

void TextViewLayout::createStandardLayout(const TextView& text, const TextView::Line &line)
{
    (TokenList()).createLayout(text, line, *this);
}

TextViewLayout::Line &TextViewLayout::getLine(int index) const noexcept
{
    return *lines.getUnchecked (index);
}
