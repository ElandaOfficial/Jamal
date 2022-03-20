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
    @file   MessageOpenDocument.cpp
    @date   12, January 2022

    ===============================================================
 */
 
#pragma once

#include <jaut_message/jaut_message.h>

class MessageOpenDocument : public jaut::IMessage
{
public:
    MessageOpenDocument(juce::String documentId, juce::String document);
    
    //==================================================================================================================
    void handleMessage(jaut::IMessageHandler *context, jaut::MessageDirection messageDirection) override;

private:
    juce::String documentId;
    juce::String text;
};
