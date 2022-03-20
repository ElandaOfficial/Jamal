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
    @file   MainComponent.cpp
    @date   29, Decembre 2021
    
    ===============================================================
 */

#include "MainComponent.h"

//======================================================================================================================
// region MainComponent
//**********************************************************************************************************************
MainComponent::MainComponent()
    : editor(document)
{
    document.insertText(0, juce::File("/home/elanda/Desktop/test.xml").loadFileAsString());
    
    setSize(900, 600);
    addAndMakeVisible(editor);
}

//======================================================================================================================
void MainComponent::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    editor.setBounds(getLocalBounds());
}
//**********************************************************************************************************************
// endregion MainComponent
//======================================================================================================================
