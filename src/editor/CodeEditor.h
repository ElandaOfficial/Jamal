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
    @file   CodeEditor.h
    @date   09, January 2022

    ===============================================================
 */

#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

struct TextMateGrammar;
class CodeEditor : public juce::Component, public juce::CodeDocument::Listener
{
public:
    static constexpr int Line_Height_Padding   =  2;
    static constexpr int Scroll_Bar_Cross_Size = 10;
    
    //==================================================================================================================
    struct ColourId
    {
        enum
        {
            Background            = 0x420690,
            Caret                 = 0x420691,
            CurrentLineBackground = 0x420692,
            SelectionBackground   = 0x420693,
            Text                  = 0x420694,
            FoldRegion            = 0x420695
        };
    };
    
    //==================================================================================================================
    explicit CodeEditor(juce::CodeDocument &document);
    ~CodeEditor() override;
    
    //==================================================================================================================
    void paint(juce::Graphics &g) override;
    void resized() override;
    
    //==================================================================================================================
    juce::CodeDocument&       getDocument()       noexcept;
    const juce::CodeDocument& getDocument() const noexcept;
    
private:
    class Gutter : public juce::Component
    {
    public:
        int getNeededWidth() const noexcept { return 0; }
    };
    
    class Line
    {
    public:
        struct DescriptionToken
        {
            juce::String text;
            int          startPos;
        };
        
        struct SyntaxToken
        {
            juce::Range<int> tokenRange;
            int              tokenId;
        };
        
        struct FoldRegion
        {
            enum class Point
            {
                None,
                Open,
                Close
            };
            
            //==========================================================================================================
            juce::Range<int> foldRange;
            Point            point;
            int              startIndex;
            bool             collapsed;
        };
        
        //==============================================================================================================
        void drawLine(const CodeEditor &editor, juce::Rectangle<float> bounds, int startPos) const;
        
        //==============================================================================================================
        bool isExtendedLine() const noexcept;
        
        //==============================================================================================================
        const FoldRegion&   getFoldRegion() const noexcept;
        const juce::String& getLineText()   const noexcept;
        
        
    private:
        std::vector<DescriptionToken> descriptionTokens;
        std::vector<SyntaxToken>      tokens;
        juce::String                  lineText;
        FoldRegion                    foldRegion { {}, FoldRegion::Point::None, 0, false };
    };
    
    struct LineReference
    {
        int lineIndex { -1 };
    };
    
    struct DocumentCaret
    {
        juce::CodeDocument::Position position;
        juce::CaretComponent         caret;
    };
    
    struct SchemeEntry
    {
        juce::Colour colour;
        int          styleFlags;
    };
    
    enum class TextUpdateMode
    {
        Resized
    };
    
    //==================================================================================================================
    juce::CodeDocument *document;
    
    juce::ScrollBar scrollBarRight;
    juce::ScrollBar scrollBarBottom;
    DocumentCaret   mainCaret;
    Gutter          gutter;
    
    // Lines
    std::vector<Line>          lines;
    std::vector<SchemeEntry>   scheme;
    std::vector<DocumentCaret> carets;
    std::array<int, 4>         rulers {};
    
    juce::Rectangle<int> editorBounds;
    juce::Font           font;
    
    float charWidth;
    float lineSpacing;
    
    bool readOnly      { false };
    bool scrollPastEnd { false };
    
    //==================================================================================================================
    void drawFoldedLine(juce::Graphics &g, juce::Rectangle<float> bounds,
                        const Line &startLine, const Line &endLine, int startPos);
    
    //==================================================================================================================
    bool keyPressed(const juce::KeyPress &key) override;
    
    //==================================================================================================================
    void codeDocumentTextInserted(const juce::String&, int) override;
    void codeDocumentTextDeleted(int, int) override;
    
    //==================================================================================================================
    void updateScrollBars();
    
    //==================================================================================================================
    void fillSchemeList(const TextMateGrammar &grammar);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CodeEditor)
};
