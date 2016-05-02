#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#endif // WIN32
#include <errno.h>

#include "GetConfig.hpp"

using namespace xercesc;
using namespace std;

/**
 *  Constructor initializes xerces-C libraries.
 *  The XML tags and attributes which we seek are defined.
 *  The xerces-C DOM parser infrastructure is initialized.
 */

GetConfig::GetConfig()
: m_ConfigFileParser(0)
, m_OptionA(0)
, m_OptionB(0)
{
   try
   {
      XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
   }
   catch( XMLException& e )
   {
      char* message = XMLString::transcode( e.getMessage() );
      cerr << "XML toolkit initialization error: " << message << endl;
      XMLString::release( &message );
      // throw exception here to return ERROR_XERCES_INIT
   }

   // Tags and attributes used in XML file.
   // Can't call transcode till after Xerces Initialize()
   TAG_root        = XMLString::transcode("root");
   TAG_ApplicationSettings = XMLString::transcode("ApplicationSettings");
   ATTR_OptionA = XMLString::transcode("option_a");
   ATTR_OptionB = XMLString::transcode("option_b");

   m_ConfigFileParser = new XercesDOMParser;
}

/**
 *  Class destructor frees memory used to hold the XML tag and
 *  attribute definitions. It als terminates use of the xerces-C
 *  framework.
 */

GetConfig::~GetConfig()
{
   // Free memory

   delete m_ConfigFileParser;
   if(m_OptionA)   XMLString::release( &m_OptionA );
   if(m_OptionB)   XMLString::release( &m_OptionB );

   try
   {
      XMLString::release( &TAG_root );

      XMLString::release( &TAG_ApplicationSettings );
      XMLString::release( &ATTR_OptionA );
      XMLString::release( &ATTR_OptionB );
   }
   catch( ... )
   {
      cerr << "Unknown exception encountered in TagNamesdtor" << endl;
   }

   // Terminate Xerces

   try
   {
      XMLPlatformUtils::Terminate();  // Terminate after release of memory
   }
   catch( xercesc::XMLException& e )
   {
      char* message = xercesc::XMLString::transcode( e.getMessage() );

      cerr << "XML ttolkit teardown error: " << message << endl;
      XMLString::release( &message );
   }
}

/**
 *  This function:
 *  - Tests the access and availability of the XML configuration file.
 *  - Configures the xerces-c DOM parser.
 *  - Reads and extracts the pertinent information from the XML config file.
 *
 *  @param in configFile The text string name of the HLA configuration file.
 */

void GetConfig::readConfigFile(const string& configFile)
        throw( std::runtime_error )
{
   // Test to see if the file is ok.

   struct stat fileStatus;

   errno = 0;
   if(stat(configFile.c_str(), &fileStatus) == -1) // ==0 ok; ==-1 error
   {
       if( errno == ENOENT )      // errno declared by include file errno.h
          throw ( std::runtime_error("Path file_name does not exist, or path is an empty string.") );
       else if( errno == ENOTDIR )
          throw ( std::runtime_error("A component of the path is not a directory."));
       else if( errno == ELOOP )
          throw ( std::runtime_error("Too many symbolic links encountered while traversing the path."));
       else if( errno == EACCES )
          throw ( std::runtime_error("Permission denied."));
       else if( errno == ENAMETOOLONG )
          throw ( std::runtime_error("File can not be read\n"));
   }

   // Configure DOM parser.

   m_ConfigFileParser->setValidationScheme( XercesDOMParser::Val_Never );
   m_ConfigFileParser->setDoNamespaces( false );
   m_ConfigFileParser->setDoSchema( false );
   m_ConfigFileParser->setLoadExternalDTD( false );

   try
   {
      m_ConfigFileParser->parse( configFile.c_str() );

      // no need to free this pointer - owned by the parent parser object
      DOMDocument* xmlDoc = m_ConfigFileParser->getDocument();

      // Get the top-level element: NAme is "root". No attributes for "root"

      DOMElement* elementRoot = xmlDoc->getDocumentElement();
      if( !elementRoot ) throw(std::runtime_error( "empty XML document" ));

      // Parse XML file for tags of interest: "ApplicationSettings"
      // Look one level nested within "root". (child of root)

      DOMNodeList*      children = elementRoot->getChildNodes();
      const  XMLSize_t nodeCount = children->getLength();

      // For all nodes, children of "root" in the XML tree.

      for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
      {
         DOMNode* currentNode = children->item(xx);
         if( currentNode->getNodeType() &&  // true is not NULL
             currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
         {
            // Found node which is an Element. Re-cast node as element
            DOMElement* currentElement
                        = dynamic_cast< xercesc::DOMElement* >( currentNode );
            if( XMLString::equals(currentElement->getTagName(), TAG_ApplicationSettings))
            {
               // Already tested node as type element and of name "ApplicationSettings".
               // Read attributes of element "ApplicationSettings".
               const XMLCh* xmlch_OptionA
                     = currentElement->getAttribute(ATTR_OptionA);
               m_OptionA = XMLString::transcode(xmlch_OptionA);

               const XMLCh* xmlch_OptionB
                     = currentElement->getAttribute(ATTR_OptionB);
               m_OptionB = XMLString::transcode(xmlch_OptionB);

               break;  // Data found. No need to look at other elements in tree.
            }
         }
      }
   }
   catch( xercesc::XMLException& e )
   {
      char* message = xercesc::XMLString::transcode( e.getMessage() );
      ostringstream errBuf;
      errBuf << "Error parsing file: " << message << flush;
      XMLString::release( &message );
   }
}

#ifdef MAIN_TEST
/* This main is provided for unit test of the class. */

int main()
{
   string configFile="sample.xml"; // stat file. Get ambigious segfault otherwise.

   GetConfig appConfig;

   appConfig.readConfigFile(configFile);

   cout << "Application option A="  << appConfig.getOptionA()  << endl;
   cout << "Application option B="  << appConfig.getOptionB()  << endl;

   return 0;
}
#endif
