#include "MCXFileReader.h"
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

MCXFileReader::MCXFileReader(const ChannelInfoContainer& i_channels_info, const std::string& i_mcx_filename)
: AbstractFileReader(i_mcx_filename)
{
	bool result = read_header(_header);
}

MCXFileReader::~MCXFileReader()
{

}

// std::string& o_VRSN, int& o_STIM, int& o_ETIM
bool MCXFileReader::read_header(Header& o_header) const
{
	char ncache_tag[5];
	char ncache_tag_buffer[20];
	read_tag(ncache_tag);
	std::cout << boost::format("ncache_tag '%1%'") % ncache_tag << std::endl;
	int32_t dummy_int32;
	int32_t start_time;
	int32_t end_time;
	read_int32(dummy_int32);
	read_int32(dummy_int32);
	read_int32(dummy_int32);
	read_tag(ncache_tag);
	std::cout << boost::format("CACH ncache_tag '%1%'") % ncache_tag << std::endl;
	read_tag(ncache_tag);
	std::cout << boost::format("VRSN ncache_tag '%1%'") % ncache_tag << std::endl;

	read_nstring(sizeof(ncache_tag_buffer),ncache_tag_buffer);
	std::cout << boost::format("VRSN buffer '%1%'") % ncache_tag_buffer << std::endl;

	read_tag(ncache_tag);
	std::cout << boost::format("STIM ncache_tag '%1%'") % ncache_tag << std::endl;

	read_nstring(sizeof(ncache_tag_buffer),ncache_tag_buffer);

	read_tag(ncache_tag);
	std::cout << boost::format("ETIM ncache_tag '%1%'") % ncache_tag << std::endl;

	read_nstring(sizeof(ncache_tag_buffer),ncache_tag_buffer);

	read_tag(ncache_tag);
	std::cout << boost::format("FOR8 ncache_tag '%1%'") % ncache_tag << std::endl;


	return true;
}

bool MCXFileReader::read_channel()
{
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
