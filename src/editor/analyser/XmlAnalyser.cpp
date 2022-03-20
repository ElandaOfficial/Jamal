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
    @file   XmlAnalyser.cpp
    @date   12, January 2022
    
    ===============================================================
 */

#include "XmlAnalyser.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>

//======================================================================================================================
XmlAnalyser::XmlAnalyser()
    : juce::Thread("XAML-ANALYSER")
{
    try
    {
        xercesc::XMLPlatformUtils::Initialize();
    }
    catch (const xercesc::XMLException &ex)
    {
        // TODO Initing failure
    }
    
    startThread();
}

XmlAnalyser::~XmlAnalyser()
{
    if (stopThread(30000))
    {
        DBG("CLEAN EXIT");
    }
    
    xercesc::XMLPlatformUtils::Terminate();
}

//======================================================================================================================
void XmlAnalyser::run()
{
    xercesc::XercesDOMParser parser;
    parser.setValidationScheme(xercesc::XercesDOMParser::Val_Always);
    parser.setDoNamespaces(true);
    parser.setValidationSchemaFullChecking(true);
    
    xercesc::HandlerBase handler;
    parser.setErrorHandler(&handler);
    
    XMLCh ch;
    
    try
    {
        parser.parse
    }
}
