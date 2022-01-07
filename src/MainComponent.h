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
    @file   MainComponent.h
    @date   29, Decembre 2021

    ===============================================================
 */

#pragma once

#include "editor/syntax/JamlTokeniser.h"

#include <juce_gui_extra/juce_gui_extra.h>
#include <jaut_core/jaut_core.h>


//======================================================================================================================
class MainComponent : public juce::Component, public juce::Timer
{
public:
    MainComponent();
    
    //==================================================================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    juce::CodeDocument        document;
    JamlTokeniser             tokeniser;
    juce::CodeEditorComponent editor;
    
    //==================================================================================================================
    void timerCallback() override
    {
        editor.retokenise(0, document.getNumCharacters() - 1);
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
