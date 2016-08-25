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
// #include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include "XMLReader.h"

using namespace nCache;

XMLReader::XMLReader()
: _time_range_start(0)
, _time_range_end(0)
, _cacheTimePerFrame_TimePerFrame(0)
, _particle_count_sampling_rate(0)
{
}

XMLReader::~XMLReader()
{
}

void XMLReader::read(const std::string& i_ncache_xml_filename)
{
	try {
		boost::property_tree::ptree pt;

		// Load XML file and put its contents in property tree.
		// No namespace qualification is needed, because of Koenig
		// lookup on the second argument. If reading fails, exception
		// is thrown.
		read_xml(i_ncache_xml_filename, pt);

		// Handle non Channels tags first
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("Autodesk_Cache_File"))
		{
			if (v.first.compare("Channels")!=0)
			{
				if (v.first.compare("cacheType")==0)
				{
					// <cacheType Type="OneFilePerFrame" Format="mcx"/>
					_cache_type = pt.get<std::string>("Autodesk_Cache_File.cacheType.<xmlattr>.Type");
					_cache_format = pt.get<std::string>("Autodesk_Cache_File.cacheType.<xmlattr>.Format");
				}
				else if (v.first.compare("cacheTimePerFrame")==0)
				{
					_cacheTimePerFrame_TimePerFrame = atoi(pt.get<std::string>("Autodesk_Cache_File.cacheTimePerFrame.<xmlattr>.TimePerFrame").c_str());
				}
				else
				{
					// std::cout << boost::format("%1% : %2%") % v.first % v.second.data() << std::endl;
				}
			}
		}

		// Now, focus on just the channel information
		BOOST_FOREACH(boost::property_tree::ptree::value_type &c, pt.get_child("Autodesk_Cache_File.Channels"))
		{
			std::cout << boost::format("Channels %1% : %2%") % c.first % c.second.data() << std::endl;

			ChannelInfo channel_info;
			std::string channel_name = pt.get<std::string>((boost::format("Autodesk_Cache_File.Channels.%1%.<xmlattr>.ChannelName")%c.first).str());
			channel_info._channel_type = pt.get<std::string>((boost::format("Autodesk_Cache_File.Channels.%1%.<xmlattr>.ChannelType")%c.first).str());
			channel_info._channel_interpretation = pt.get<std::string>((boost::format("Autodesk_Cache_File.Channels.%1%.<xmlattr>.ChannelInterpretation")%c.first).str());
			std::string sampling_type_string = pt.get<std::string>((boost::format("Autodesk_Cache_File.Channels.%1%.<xmlattr>.SamplingType")%c.first).str());
			if (sampling_type_string.compare("Regular") == 0)
				channel_info._sampling_type = ChannelInfo::SAMPLING_REGULAR;
			else if (sampling_type_string.compare("Irregular") == 0)
				channel_info._sampling_type = ChannelInfo::SAMPLING_IRREGULAR;
			channel_info._sampling_rate = atoi(pt.get<std::string>((boost::format("Autodesk_Cache_File.Channels.%1%.<xmlattr>.SamplingRate")%c.first).str()).c_str());

			channel_info._start_time = atoi(pt.get<std::string>((boost::format("Autodesk_Cache_File.Channels.%1%.<xmlattr>.StartTime")%c.first).str()).c_str());
			channel_info._end_time = atoi(pt.get<std::string>((boost::format("Autodesk_Cache_File.Channels.%1%.<xmlattr>.EndTime")%c.first).str()).c_str());

			_channels.insert(ChannelInfoContainer::value_type(channel_name,channel_info));
		}
	}
	catch (const boost::property_tree::ptree_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

const ChannelInfo* XMLReader::find_channel_by_name(const std::string& i_channel_name) const
{
	std::pair<ChannelInfoContainer::iterator,bool> result;
	ChannelInfoContainer::const_iterator iter = _channels.find(i_channel_name);
	if (iter == _channels.end())
		return 0;
	else
		return &(result.first->second);

	return 0;
}

const ChannelInfo* XMLReader::find_channel_by_interpretation(const std::string& i_channel_interpretation) const
{
	ChannelInfoContainer::const_iterator iter = _channels.begin();
	ChannelInfoContainer::const_iterator eIter = _channels.end();
	for (;iter!=eIter;++iter)
	{
		if (iter->second._channel_interpretation.compare(i_channel_interpretation) == 0)
			return &(iter->second);
	}

	return 0;
}

bool XMLReader::find_channel_ends_with(const std::string& i_matching_endswith_string, std::string& o_channel_name) const
{
	ChannelInfoContainer::const_iterator cicIter = _channels.begin();
	ChannelInfoContainer::const_iterator cicEIter = _channels.end();
	for (;cicIter!=cicEIter;++cicIter)
	{
		if (boost::algorithm::ends_with(cicIter->first, i_matching_endswith_string))
		{
			o_channel_name = cicIter->first;
			return true;
		}
	}
	return false;
}

void XMLReader::debugDump() const
{
	std::cout << "XMLReader::debugDump() : " << std::endl;
	// std::cout << boost::format("Number of frames = %1%") % _num_frames << std::endl;
	std::cout << boost::format("Type = %1%, Format = %2%") % _cache_type % _cache_format << std::endl;
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
