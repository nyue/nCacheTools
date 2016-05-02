#include "MCFileReader.h"
#include <iostream>
#include <boost/format.hpp>
#include <stdio.h>

using namespace nCache;

//  |---CACH (Group)	// Header
//  |     |---VRSN		// Version Number (char*)
//  |     |---STIM		// Start Time of the Cache File (int)
//  |     |---ETIM		// End Time of the Cache File (int)
//  |
//  |---MYCH (Group)	// 1st Time
//  |     |---TIME		// Time (int)
//  |     |---CHNM		// 1st Channel Name (char*)
//  |     |---SIZE		// 1st Channel Size
//  |     |---DVCA		// 1st Channel Data (Double Vector Array)
//  |     |---CHNM		// n-th Channel Name
//  |     |---SIZE		// n-th Channel Size
//  |     |---DVCA		// n-th Channel Data (Double Vector Array)
//  |     |..
//  |
//  |---MYCH (Group)	// 2nd Time
//  |     |---TIME		// Time
//  |     |---CHNM		// 1st Channel Name
//  |     |---SIZE		// 1st Channel Size
//  |     |---DVCA		// 1st Channel Data (Double Vector Array)
//  |     |---CHNM		// n-th Channel Name
//  |     |---SIZE		// n-th Channel Size
//  |     |---DVCA		// n-th Channel Data (Double Vector Array)
//  |     |..
//  |
//  |---..
//	|
//

MCFileReader::MCFileReader(const ChannelInfoContainer& i_channels_info, const std::string& i_mc_filename)
: AbstractFileReader(i_mc_filename)
{
	bool result;
	result = read_header(_header);
	result = read_channel();
}

MCFileReader::~MCFileReader()
{

}

bool MCFileReader::read_header(Header& o_header) const
{
	char ncache_tag[5];
	char ncache_tag_buffer[8];
	read_tag(ncache_tag);
	DLOG(INFO) << boost::format("ncache_tag '%1%'") % ncache_tag << std::endl;
	// int16_t dummy_int16;
	int32_t dummy_int32;
	int64_t dummy_int64;
//	int32_t start_time;
//	int32_t end_time;
	read_int32(dummy_int32);
	DLOG(INFO) << boost::format("dummy_int32 %1%") % dummy_int32 << std::endl;
	//read_int32(dummy_int32);
	//read_int32(dummy_int32);
	read_tag(ncache_tag);
	DLOG(INFO) << boost::format("CACH ncache_tag '%1%'") % ncache_tag << std::endl;

	read_tag(ncache_tag);
	DLOG(INFO) << boost::format("VRSN ncache_tag '%1%'") % ncache_tag << std::endl;

	read_nstring(sizeof(ncache_tag_buffer),ncache_tag_buffer);
	DLOG(INFO) << boost::format("VRSN buffer '%1%'") % ncache_tag_buffer << std::endl;

	read_tag(ncache_tag);
	DLOG(INFO) << boost::format("STIM ncache_tag '%1%'") % ncache_tag << std::endl;

	read_int32(dummy_int32);
	DLOG(INFO) << boost::format("STIM dummy_int32 %1%") % dummy_int32 << std::endl;
	read_int32(dummy_int32);
	o_header.STIM = dummy_int32;
	DLOG(INFO) << boost::format("_header.STIM start_time %1%") % _header.STIM << std::endl;

	read_tag(ncache_tag);
	DLOG(INFO) << boost::format("ETIM ncache_tag '%1%'") % ncache_tag << std::endl;

	read_int32(dummy_int32);
	DLOG(INFO) << boost::format("ETIM dummy_int32 %1%") % dummy_int32 << std::endl;
	read_int32(dummy_int32);
	o_header.ETIM = dummy_int32;
	DLOG(INFO) << boost::format("_header.ETIM end_time %1%") % _header.ETIM << std::endl;

	return true;
}

bool MCFileReader::read_channel()
{
	char ncache_tag[5];
	int32_t dummy_int32;
	int32_t byte_count;

	read_tag(ncache_tag);
	DLOG(INFO) << boost::format("ncache_tag '%1%'") % ncache_tag << std::endl;

	read_int32(byte_count);
	DLOG(INFO) << boost::format("Channels data byte count %1%") % byte_count << std::endl;

	read_tag(ncache_tag);
	DLOG(INFO) << boost::format("ncache_tag '%1%'") % ncache_tag << std::endl;

	read_tag(ncache_tag);
	DLOG(INFO) << boost::format("ncache_tag '%1%'") % ncache_tag << std::endl;

	read_int32(byte_count);
	DLOG(INFO) << boost::format("CHNM byte count %1%") % byte_count << std::endl;
	std::string channel_name;
	read_nstring(byte_count,channel_name);
	DLOG(INFO) << boost::format("CHNM name '%1%'") % channel_name << std::endl;
	return true;
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
