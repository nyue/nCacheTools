#pragma once

#include "ChannelInfo.h"
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
#include <iostream>

#include <glog/logging.h>
#include <boost/format.hpp>

namespace nCache
{
	// Xerces Error codes

	enum {
	   ERROR_ARGS = 1,
	   ERROR_XERCES_INIT,
	   ERROR_PARSE,
	   ERROR_EMPTY_DOCUMENT
	};

	class XMLReader
	{
	public:
		XMLReader();
		~XMLReader();
		void read(const std::string& i_ncache_xml_filename);
		std::string getCacheType() const { return _cache_type; }
		std::string getCacheFormat() const { return _cache_format; }
		const ChannelInfoContainer& getChannels() const {return _channels; }
		std::string get_cache_name() const { return _base_cache_name;};
		std::string get_cache_directory() const { return _cache_directory;};
		// size_t get_num_frames() const { return _num_frames;};

		size_t get_time_range_start() const { return _time_range_start;};
		size_t get_time_range_end() const { return _time_range_end;};
		size_t get_cacheTimePerFrame_TimePerFrame() const { return _cacheTimePerFrame_TimePerFrame;};
		size_t get_particle_count_sampling_rate() const { return _particle_count_sampling_rate;};

		bool find_channel_ends_with(const std::string& i_matching_endswith_string, std::string& o_channel_name) const;

		void debugDump() const;
	protected:
		void readConfigFile(const std::string&) throw(std::runtime_error);
		void handle_cacheType(xercesc::DOMElement* currentElement);
		void handle_time(xercesc::DOMElement* currentElement);
		void handle_cacheTimePerFram(xercesc::DOMElement* currentElement);
		void handle_Channels(xercesc::DOMElement* currentElement);
		void handle_Channel(xercesc::DOMElement* i_channel_element, std::string& o_channel_name, ChannelInfo& o_channel);
		void get_string_attribute(xercesc::DOMElement* i_element, const std::string& i_attribute, std::string& o_string) const;
		/*!
		 * \todo Switch to returning bool and actual result as an output
		 *       parameter
		 */
		const ChannelInfo* find_channel_by_name(const std::string& i_channel_name) const;
		/*!
		 * \todo Switch to returning bool and actual result as an output
		 *       parameter so that we can return a list of channels that has the
		 *       same interpretation as they are not unique
		 */
		const ChannelInfo* find_channel_by_interpretation(const std::string& i_channel_interpretation) const;
	protected:
		std::string _cache_type;
		std::string _cache_format;
		size_t _time_range_start;
		size_t _time_range_end;
		size_t _cacheTimePerFrame_TimePerFrame;
		std::string _cacheVersion_Version;

		size_t _particle_count_sampling_rate;

		ChannelInfoContainer _channels;
		// size_t _num_frames;
		/*!
		 * \brief Extracted from input *.xml and used for generating Frame/Subframe *.mcx file names
		 */
		std::string _base_cache_name;
		std::string _cache_directory;
	private:
		xercesc::XercesDOMParser *m_ConfigFileParser;

		// Internal class use only. Hold Xerces data in UTF-16 SMLCh type.

		XMLCh* TAG_Autodesk_CacheFile;
		XMLCh* TAG_cacheType;
		XMLCh* TAG_time;
		XMLCh* TAG_cacheTimePerFrame;

		XMLCh* TAG_Channels;
	};
} // namespace nCache
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
