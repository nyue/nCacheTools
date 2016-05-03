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

#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "XMLReader.h"

using namespace xercesc;
// using namespace std;
using namespace nCache;

/**
 *  Constructor initializes xerces-C libraries.
 *  The XML tags and attributes which we seek are defined.
 *  The xerces-C DOM parser infrastructure is initialized.
 */

XMLReader::XMLReader(const std::string& i_ncache_xml_filename)
: m_ConfigFileParser(0)
// , _num_frames(0)
{
   try
   {
      XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
   }
   catch( XMLException& e )
   {
      char* message = XMLString::transcode( e.getMessage() );
      std::cerr << "XML toolkit initialization error: " << message << std::endl;
      XMLString::release( &message );
      // throw exception here to return ERROR_XERCES_INIT
   }

   // Tags and attributes used in XML file.
   // Can't call transcode till after Xerces Initialize()
   TAG_Autodesk_CacheFile	= XMLString::transcode("Autodesk_CacheFile");
   TAG_cacheType			= XMLString::transcode("cacheType");
   TAG_time			        = XMLString::transcode("time");
   TAG_cacheTimePerFrame    = XMLString::transcode("cacheTimePerFrame");
   TAG_Channels             = XMLString::transcode("Channels");

   m_ConfigFileParser = new XercesDOMParser;
   readConfigFile(i_ncache_xml_filename);
}

/**
 *  Class destructor frees memory used to hold the XML tag and
 *  attribute definitions. It als terminates use of the xerces-C
 *  framework.
 */

XMLReader::~XMLReader()
{
   // Free memory

   delete m_ConfigFileParser;

   try
   {
      XMLString::release( &TAG_Autodesk_CacheFile );
      XMLString::release( &TAG_cacheType );
      XMLString::release( &TAG_time );
      XMLString::release( &TAG_cacheTimePerFrame );

      XMLString::release( &TAG_Channels );
   }
   catch( ... )
   {
	   std::cerr << "Unknown exception encountered in TagNamesdtor" << std::endl;
   }

   // Terminate Xerces

   try
   {
      XMLPlatformUtils::Terminate();  // Terminate after release of memory
   }
   catch( xercesc::XMLException& e )
   {
      char* message = xercesc::XMLString::transcode( e.getMessage() );

      std::cerr << "XML toolkit teardown error: " << message << std::endl;
      XMLString::release( &message );
   }
}

void XMLReader::get_string_attribute(xercesc::DOMElement* i_element, const std::string& i_attribute, std::string& o_string) const
{
	XMLCh* ATTR_attribute	= XMLString::transcode(i_attribute.c_str());
    const XMLCh* xmlch_attribute = i_element->getAttribute(ATTR_attribute);
    char *attribute_ptr = XMLString::transcode(xmlch_attribute);
    o_string = attribute_ptr;
    XMLString::release( &attribute_ptr );
    XMLString::release( &ATTR_attribute );
}

void XMLReader::handle_cacheType(xercesc::DOMElement* currentElement)
{
	XMLCh* ATTR_Type	= XMLString::transcode("Type");
	XMLCh* ATTR_Format	= XMLString::transcode("Format");

    const XMLCh* xmlch_Type
          = currentElement->getAttribute(ATTR_Type);
    char *cache_type = XMLString::transcode(xmlch_Type);
    _cache_type = cache_type;

    const XMLCh* xmlch_Format
          = currentElement->getAttribute(ATTR_Format);
    char *cache_format = XMLString::transcode(xmlch_Format);
    _cache_format = cache_format;

    XMLString::release( &cache_type );
    XMLString::release( &cache_format );

    XMLString::release( &ATTR_Type );
    XMLString::release( &ATTR_Format );

}

void XMLReader::handle_time(xercesc::DOMElement* currentElement)
{
	std::string time_range;
	get_string_attribute(currentElement, "Range", time_range);

	typedef std::vector< std::string > split_vector_type;

	split_vector_type SplitVec; // #2: Search for tokens
	boost::split( SplitVec, time_range, boost::is_any_of("-") );
	if (SplitVec.size() == 2)
	{
		_time_range_start = atoi(SplitVec[0].c_str());
		_time_range_end = atoi(SplitVec[1].c_str());
		// std::cout << boost::format("Frame range %1% - %2%") % _time_range_start % _time_range_end << std::endl;
	}
}

void XMLReader::handle_cacheTimePerFram(xercesc::DOMElement* currentElement)
{
	std::string time_per_frame;
	get_string_attribute(currentElement, "TimePerFrame", time_per_frame);
	_cacheTimePerFrame_TimePerFrame = atoi(time_per_frame.c_str());
	// std::cout << boost::format("_cacheTimePerFrame_TimePerFrame range %1%") % _cacheTimePerFrame_TimePerFrame << std::endl;
}

void XMLReader::handle_Channels(xercesc::DOMElement* currentElement)
{
    DOMNodeList*      children = currentElement->getChildNodes();
    const  XMLSize_t nodeCount = children->getLength();
    // size_t elementNodeCount = 0;
    for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
    {
    	DOMNode* currentNode = children->item(xx);
    	if( currentNode->getNodeType() &&  // true is not NULL
    			currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
    	{
    		ChannelInfo channel_info;
    		std::string channel_name;
    		// handle_Channel(dynamic_cast<xercesc::DOMElement*>(currentNode),channel_info);
    		handle_Channel(dynamic_cast<xercesc::DOMElement*>(currentNode),channel_name,channel_info);
    		// _channels.push_back(channel_info);
    		_channels.insert(ChannelInfoContainer::value_type(channel_name,channel_info));
    	}
    }
    // std::cout << boost::format("Channels children element nodeCount = %1%") % elementNodeCount << std::endl;
}

void XMLReader::handle_Channel(xercesc::DOMElement* i_channel_element, std::string& o_channel_name, ChannelInfo& o_channel)
{

	// get_string_attribute(i_channel_element, "ChannelName", o_channel._channel_name);
	get_string_attribute(i_channel_element, "ChannelName", o_channel_name);
	get_string_attribute(i_channel_element, "ChannelType", o_channel._channel_type);
	get_string_attribute(i_channel_element, "ChannelInterpretation", o_channel._channel_interpretation);
	//
	std::string _sampling_type;
	get_string_attribute(i_channel_element, "SamplingType", _sampling_type);
	if (_sampling_type.compare("Regular")==0)
		o_channel._sampling_type = ChannelInfo::SAMPLING_REGULAR;
	else
		o_channel._sampling_type = ChannelInfo::SAMPLING_IRREGULAR;
	//
	std::string _sampling_rate;
	get_string_attribute(i_channel_element, "SamplingRate", _sampling_rate);
	o_channel._sampling_rate = atoi(_sampling_rate.c_str());
	//
	std::string _start_time;
	get_string_attribute(i_channel_element, "StartTime", _start_time);
	o_channel._start_time = atoi(_start_time.c_str());
	//
	std::string _end_time;
	get_string_attribute(i_channel_element, "EndTime", _end_time);
	o_channel._end_time = atoi(_end_time.c_str());
}

/**
 *  This function:
 *  - Tests the access and availability of the XML configuration file.
 *  - Configures the xerces-c DOM parser.
 *  - Reads and extracts the pertinent information from the XML config file.
 *
 *  @param in configFile The text string name of the HLA configuration file.
 */

void XMLReader::readConfigFile(const std::string& configFile)
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

      // std::cout << boost::format("nodeCount = %1%") % nodeCount << std::endl;

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
#ifdef DEBUG
            char *element_name = XMLString::transcode(currentElement->getTagName());
            if (element_name)
            {
            	std::cout << boost::format("element[%1%] name = '%2%'") % xx % element_name << std::endl;
                XMLString::release( &element_name );
            }
#endif // DEBUG
            if( XMLString::equals(currentElement->getTagName(), TAG_cacheType))
            {
            	handle_cacheType(currentElement);
            }
            if( XMLString::equals(currentElement->getTagName(), TAG_time))
            {
            	handle_time(currentElement);
            }

            if( XMLString::equals(currentElement->getTagName(), TAG_cacheTimePerFrame))
            {
            	handle_cacheTimePerFram(currentElement);
            }
            if( XMLString::equals(currentElement->getTagName(), TAG_Channels))
            {
            	handle_Channels(currentElement);
            }
         }
      }

      // Do some post processing
      if (find_channel_by_interpretation("count"))
      {
          const ChannelInfo* count_channel = find_channel_by_interpretation("count");
    	  _particle_count_sampling_rate = count_channel->_sampling_rate;
      }
      else if (find_channel_by_interpretation("hairCounts"))
      {
          const ChannelInfo* count_channel = find_channel_by_interpretation("hairCounts");
    	  _particle_count_sampling_rate = count_channel->_sampling_rate;
      }
      else
    	  throw std::runtime_error("XML parsing error, unable to find a channel with 'count' like interpretation");
      // Determine the base name so we can generate the Per frame *.mcx filenames
      boost::filesystem::path p(configFile);
      _base_cache_name = p.stem().string();
      _cache_directory = p.parent_path().string();

//      std::cout << boost::format("_base_cache_name = '%1%'") % _base_cache_name << std::endl;
//      std::cout << boost::format("_cache_directory = '%1%'") % _cache_directory << std::endl;
   }
   catch( xercesc::XMLException& e )
   {
      char* message = xercesc::XMLString::transcode( e.getMessage() );
      std::ostringstream errBuf;
      errBuf << "Error parsing file: " << message << std::flush;
      XMLString::release( &message );
   }
}

const ChannelInfo* XMLReader::find_channel_by_name(const std::string& i_channel_name) const
{
#ifdef NON_MAP
	size_t num_channels = _channels.size();
	for (size_t channel_index = 0;channel_index<num_channels;++channel_index)
	{
		if (_channels[channel_index]._channel_name.compare(i_channel_name) == 0)
			return &_channels[channel_index];
	}
#else // NON_MAP
	std::pair<ChannelInfoContainer::iterator,bool> result;
	ChannelInfoContainer::const_iterator iter = _channels.find(i_channel_name);
	if (iter == _channels.end())
		return 0;
	else
		return &(result.first->second);
#endif // NON_MAP

	return 0;
}

const ChannelInfo* XMLReader::find_channel_by_interpretation(const std::string& i_channel_interpretation) const
{
#ifdef NON_MAP
	size_t num_channels = _channels.size();
	for (size_t channel_index = 0;channel_index<num_channels;++channel_index)
	{
		if (_channels[channel_index]._channel_interpretation.compare(i_channel_interpretation) == 0)
			return &_channels[channel_index];
	}
#else // NON_MAP
	ChannelInfoContainer::const_iterator iter = _channels.begin();
	ChannelInfoContainer::const_iterator eIter = _channels.end();
	for (;iter!=eIter;++iter)
	{
		if (iter->second._channel_interpretation.compare(i_channel_interpretation) == 0)
			return &(iter->second);
	}
#endif // NON_MAP

	return 0;
}

void XMLReader::debugDump() const
{
	std::cout << "XMLReader::debugDump() : " << std::endl;
	// std::cout << boost::format("Number of frames = %1%") % _num_frames << std::endl;
	std::cout << boost::format("Number of channels = %1%") % _channels.size() << std::endl;

	ChannelInfoContainer::const_iterator channels_iter = _channels.begin();
	ChannelInfoContainer::const_iterator channels_eIter = _channels.end();
	for (;channels_iter!=channels_eIter;++channels_iter)
	{
		std::cout << channels_iter->second << std::endl;
	}

}

// == Emacs ================
// -------------------------
// Local variables:
// tab-width: 4
// indent-tabs-mode: t
// c-basic-offset: 4
// end:
//
// == vi ===================
// -------------------------
// Format block
// ex:ts=4:sw=4:expandtab
// -------------------------
