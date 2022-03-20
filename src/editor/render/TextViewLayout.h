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
    @file   TextViewLayout.h
    @date   23, January 2022

    ===============================================================
 */

#pragma once

#include "../TextView.h"
#include <juce_graphics/juce_graphics.h>

class TextViewLayout
{
public:
    class JUCE_API  Glyph
    {
    public:
        Glyph(int glyphCode, juce::Point<float> anchor, float width) noexcept;
        
        juce::Point<float> anchor;
        float              width;
        int                glyphCode;
    
    private:
        JUCE_LEAK_DETECTOR (Glyph)
    };
    
    class JUCE_API Run
    {
    public:
        Run() = default;
        Run(juce::Range<int> stringRange, int numGlyphsToPreallocate);
        
        juce::Range<float> getRunBoundsX() const noexcept;
        
        juce::Font font;
        juce::Colour colour { 0xff000000 };
        juce::Array<Glyph> glyphs;
        juce::Range<int> stringRange;
    
    private:
        JUCE_LEAK_DETECTOR (Run)
    };
    
    class JUCE_API  Line
    {
    public:
        Line() = default;
        Line (juce::Range<int> stringRange, juce::Point<float> lineOrigin,
              float ascent, float descent, float leading, int numRunsToPreallocate);
        
        Line (const Line&);
        Line& operator= (const Line&);
        
        Line (Line&&) noexcept = default;
        Line& operator= (Line&&) noexcept = default;
        
        ~Line() noexcept = default;
        
        juce::Range<float> getLineBoundsX() const noexcept;
        juce::Range<float> getLineBoundsY() const noexcept;
        juce::Rectangle<float> getLineBounds() const noexcept;
        
        void swap (Line& other) noexcept;
        
        juce::OwnedArray<Run> runs;
        juce::Range<int> stringRange;
        
        juce::Point<float> lineOrigin;
        float ascent  = 0.0f,
            descent = 0.0f,
            leading = 0.0f;
        
    private:
        JUCE_LEAK_DETECTOR (Line)
    };
    
    //==================================================================================================================
    TextViewLayout();
    
    //==================================================================================================================
    void createLayout (const TextView&, const TextView::Line &line, float maxWidth);
    void draw (juce::Graphics&, juce::Rectangle<float> area) const;
    
    //==================================================================================================================
    void addLine(std::unique_ptr<Line>);
    void createLine(const TextView&, const TextView::Line &line, float maxWidth);
    
    //==================================================================================================================
    void ensureStorageAllocated (int numLinesNeeded);
    void recalculateSize();
    
    //==================================================================================================================
    Line& getLine (int index) const noexcept;

private:
    friend struct TokenList;
    
    juce::OwnedArray<Line> lines;
    float width, height;
    juce::Justification justification;
    
    void createStandardLayout(const TextView&, const TextView::Line &line);
    
    JUCE_LEAK_DETECTOR (TextViewLayout)
};
