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

#include "MessageOpenDocument.h"
#include "../XmlAnalyser.h"

#include <xercesc/parsers/XercesDOMParser.hpp>

//======================================================================================================================
MessageOpenDocument::MessageOpenDocument(juce::String parDocumentId, juce::String parText)
    : documentId(std::move(parDocumentId)), text(std::move(parText))
{}

//======================================================================================================================
void MessageOpenDocument::handleMessage(jaut::IMessageHandler *context, jaut::MessageDirection messageDirection)
{
    auto *const analyser = static_cast<XmlAnalyser*>(context);
    
    xercesc::XercesDOMParser parser;
    parser.setValidationScheme(xercesc::XercesDOMParser::Val_Always);
    parser.setDoNamespaces(true);
}

