#pragma once

/**
 *  @file
 *  Class "GetConfig" provides the functions to read the XML data.
 *  @version 1.0
 */
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <string>
#include <stdexcept>

// Error codes

enum {
   ERROR_ARGS = 1,
   ERROR_XERCES_INIT,
   ERROR_PARSE,
   ERROR_EMPTY_DOCUMENT
};

class GetConfig
{
public:
   GetConfig();
  ~GetConfig();
   void readConfigFile(const std::string&) throw(std::runtime_error);

   char *getOptionA() { return m_OptionA; };
   char *getOptionB() { return m_OptionB; };

private:
   xercesc::XercesDOMParser *m_ConfigFileParser;
   char* m_OptionA;
   char* m_OptionB;

   // Internal class use only. Hold Xerces data in UTF-16 SMLCh type.

   XMLCh* TAG_root;

   XMLCh* TAG_ApplicationSettings;
   XMLCh* ATTR_OptionA;
   XMLCh* ATTR_OptionB;
};
