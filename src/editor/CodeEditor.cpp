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
    @file   CodeEditor.cpp
    @date   09, January 2022

    ===============================================================
 */

#include "CodeEditor.h"

#include "syntax/textmate/TextMateCache.h"
#include "syntax/textmate/TextMateGrammar.h"
#include "small_vector/small_vector.h"

//**********************************************************************************************************************
// region Namespace
//======================================================================================================================
namespace
{
    int getMaxPossibleLines(const juce::Rectangle<int> &bounds, float lineHeight, double scrollOffset)
    {
        return static_cast<int>(std::ceil(static_cast<double>(bounds.getHeight()) / lineHeight  + scrollOffset));
    }
    
    juce::Rectangle<float> getLineBounds(const juce::Rectangle<int> &editorBounds, float charPos, float linePos,
                                         float lineHeight) noexcept
    {
        return {
            charPos,
            linePos,
            static_cast<float>(editorBounds.getWidth()) - charPos,
            lineHeight
        };
    }
}
//======================================================================================================================
// endregion Namespace
//**********************************************************************************************************************
// region CodeEditor
//======================================================================================================================
//======================================================================================================================
//**********************************************************************************************************************
// region Line
//======================================================================================================================
void CodeEditor::Line::drawLine(const CodeEditor &editor, juce::Rectangle<float> bounds, int startPos) const
{
    juce::AttributedString text_brush;
    
    for (const auto &token : tokens)
    {
        
    }
}

//======================================================================================================================
bool CodeEditor::Line::isExtendedLine() const noexcept
{
    return !descriptionTokens.empty();
}

//======================================================================================================================
const CodeEditor::Line::FoldRegion& CodeEditor::Line::getFoldRegion() const noexcept { return foldRegion; }
const juce::String&                 CodeEditor::Line::getLineText()   const noexcept { return lineText;   }

//======================================================================================================================
// endregion Line
//**********************************************************************************************************************
// region CodeEditor
//======================================================================================================================
CodeEditor::CodeEditor(juce::CodeDocument &parDocument)
    : document(&parDocument),
      scrollBarRight(true), scrollBarBottom(false),
      mainCaret{{ *document, 0 }, { this }},
      font("Droid Sans", 14.0f, 0),
      charWidth(font.getStringWidthFloat("0")),
      lineSpacing(1.2f)
{
    addChildComponent(scrollBarRight);
    addChildComponent(scrollBarBottom);
    addChildComponent(gutter);
    mainCaret.position.setPositionMaintained(true);
    mainCaret.caret.setSize(2, static_cast<int>(lineSpacing * 0.8f));
    addAndMakeVisible(mainCaret.caret);
    
    updateScrollBars();
}

CodeEditor::~CodeEditor() = default;

//======================================================================================================================
void CodeEditor::paint(juce::Graphics &g)
{
    const double scroll_val  = scrollBarRight.getCurrentRangeStart();
    const double line_offset = scroll_val - static_cast<int>(scroll_val);
    const float  line_height = font.getHeight() * lineSpacing;
    const int    first_line  = scrollBarRight.isVisible() * static_cast<int>(scroll_val);
    
    float line_pos = static_cast<float>(editorBounds.getY()) - line_height * static_cast<float>(line_offset);
    g.setFont(font);
    
    const double char_scroll_val = scrollBarBottom.getCurrentRangeStart();
    const double char_offset     = char_scroll_val - static_cast<int>(char_scroll_val);
    const int    first_char      = scrollBarBottom.isVisible() * static_cast<int>(char_scroll_val);
    
    float char_pos = static_cast<float>(editorBounds.getX()) - charWidth * static_cast<float>(char_offset);
    
    for (auto i = static_cast<std::size_t>(first_line); i < lines.size(); ++i)
    {
        if (line_pos >= static_cast<float>(editorBounds.getBottom()))
        {
            break;
        }
        
        const Line             &line   = lines[i];
        const Line::FoldRegion &region = line.getFoldRegion();
        
        if (region.point != Line::FoldRegion::Point::Open || !region.collapsed)
        {
            if (line.isExtendedLine())
            {
                line_pos += line_height;
            }
            
            if (line_pos >= static_cast<float>(editorBounds.getBottom()))
            {
                break;
            }
            
            line.drawLine(*this, ::getLineBounds(editorBounds, char_pos, line_pos, line_height), first_char);
        }
        else
        {
            i = static_cast<std::size_t>(region.foldRange.getEnd());
            const Line &end_line = lines[i];
            drawFoldedLine(g, ::getLineBounds(editorBounds, char_pos, line_pos, line_height),
                           line, lines[i], first_char);
        }
        
        line_pos += line_height;
    }
}

void CodeEditor::resized()
{
    editorBounds = getLocalBounds();
    gutter.setBounds(editorBounds.removeFromLeft(gutter.getNeededWidth()));
    
    juce::Rectangle temp(editorBounds);
    
    if (scrollBarRight.isVisible())
    {
        scrollBarRight.setBounds(temp.removeFromRight(10).withTrimmedBottom(Scroll_Bar_Cross_Size));
    }
    
    if (scrollBarBottom.isVisible())
    {
        scrollBarBottom.setBounds(temp.removeFromBottom(10).withTrimmedRight(Scroll_Bar_Cross_Size));
    }
}

//======================================================================================================================
juce::CodeDocument&       CodeEditor::getDocument()       noexcept { return *document; }
const juce::CodeDocument& CodeEditor::getDocument() const noexcept { return *document; }

//======================================================================================================================
void CodeEditor::drawFoldedLine(juce::Graphics &g, juce::Rectangle<float> bounds,
                                const Line &startLine, const Line &endLine, int startPos)
{
    const Line::FoldRegion &start_region = startLine.getFoldRegion();
    const Line::FoldRegion &end_region   = endLine  .getFoldRegion();
    
    juce::String fold_text;
    fold_text << juce::String(startLine.getLineText()[start_region.startIndex]).paddedLeft(' ', start_region.startIndex)
              << "..."
              << endLine.getLineText()[end_region.startIndex];
    
    const int fold_text_length = fold_text.length();
    fold_text = fold_text.substring(startPos);
    
    if (!fold_text.isEmpty())
    {
        g.setColour(findColour(ColourId::Text));
        g.drawText(fold_text, bounds, juce::Justification::centredLeft);
    }
    
    startPos -= fold_text_length;
    endLine.drawLine(*this, bounds.removeFromLeft(static_cast<float>(fold_text.length()) * charWidth),
                     end_region.startIndex + 1 + startPos);
}

//======================================================================================================================
bool CodeEditor::keyPressed(const juce::KeyPress &key)
{
    return false;
}

//======================================================================================================================
void CodeEditor::codeDocumentTextInserted(const juce::String&, int)
{
    updateScrollBars();
    repaint();
}

void CodeEditor::codeDocumentTextDeleted(int, int)
{
    updateScrollBars();
    repaint();
}

//======================================================================================================================
void CodeEditor::updateScrollBars()
{
    const int max_line_length = document->getMaximumLineLength();
    
    if (scrollPastEnd)
    {
        scrollBarRight.setRangeLimits(0, static_cast<double>(lines.size() - 1));
    }
    else
    {
        scrollBarRight.setRangeLimits(0, static_cast<int>(lines.size() - 1)
                                         - static_cast<int>(static_cast<float>(editorBounds.getHeight())
                                                            / (font.getHeight() * lineSpacing)));
    }
    
    scrollBarBottom.setRangeLimits(0, max_line_length
                                      - static_cast<int>(static_cast<float>(editorBounds.getWidth()) / charWidth));
}

//======================================================================================================================
void CodeEditor::fillSchemeList(const TextMateGrammar &grammar)
{
    
}
//======================================================================================================================
// endregion CodeEditor
//**********************************************************************************************************************
//======================================================================================================================
//======================================================================================================================
// endregion CodeEditor
//**********************************************************************************************************************
