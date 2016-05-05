#include "MCMemoryReader.h"
#include <iostream>
#include <boost/format.hpp>
#include <boost/algorithm/string/predicate.hpp>
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

MCMemoryReader::MCMemoryReader(const std::string& i_mc_filename, const ChannelInfoContainer* i_channels_info)
: AbstractMemoryReader(i_mc_filename)
{
	bool result;
	result = read_header(_header);
	result = read_channel();
}

MCMemoryReader::~MCMemoryReader()
{
#ifdef PRINT_OUT
	ChannelDataContainer::const_iterator iter = _channels_data.begin();
	ChannelDataContainer::const_iterator eIter = _channels_data.end();
	for (;iter!=eIter;++iter)
	{
		if (iter->_type.compare("DBLA")==0)
		{
			size_t num_elements = iter->_dbla.size();
			std::cout << boost::format("~MCMemoryReader : channel['%1%'] has %2% elements") % iter->_name % num_elements<< std::endl;
			for (size_t i=0;i<num_elements;i++)
			{
				std::cout << boost::format("\t""%1%") % iter->_dbla[i] << std::endl;
			}
		}
		else if (iter->_type.compare("FVCA")==0)
		{
			size_t num_elements = iter->_fvca.size();
			std::cout << boost::format("~MCMemoryReader : channel['%1%'] has %2% elements") % iter->_name % num_elements<< std::endl;
			for (size_t i=0;i<num_elements;i++)
			{
				std::cout << boost::format("\t""%1%,%2%,%3%") % iter->_fvca[i].x % iter->_fvca[i].y % iter->_fvca[i].z << std::endl;
			}

		}
		else if (iter->_type.compare("FBCA")==0)
		{
			size_t num_elements = iter->_fbca.size();
			std::cout << boost::format("~MCMemoryReader : channel['%1%'] has %2% elements") % iter->_name % num_elements<< std::endl;
			for (size_t i=0;i<num_elements;i++)
			{
				std::cout << boost::format("\t""%1%") % iter->_fbca[i] << std::endl;
			}

		}
	}
	std::cout << "MCMemoryReader destructor" << std::endl;
#endif // PRINT_OUT
}

bool MCMemoryReader::read_header(Header& o_header)
{

	std::string tag;
	int32_t value_int32;
	read_header_tag(tag);
	DLOG(INFO) << boost::format("HEADER : 01 tag '%1%'") % tag << std::endl;
	read_header_tag(tag);
	DLOG(INFO) << boost::format("HEADER : 02 tag '%1%'") % tag << std::endl;
	read_header_pascal_string_32(tag);
	DLOG(INFO) << boost::format("HEADER : 03 version '%1%'") % tag << std::endl;
	// STIM
	read_header_tag(tag);
	DLOG(INFO) << boost::format("HEADER : 04 tag '%1%'") % tag << std::endl;
	read_header_int32(value_int32);
	read_header_int32(value_int32);
	o_header.STIM = value_int32;
	DLOG(INFO) << boost::format("HEADER : 05 o_header.STIM %1%") % o_header.STIM << std::endl;

	// ETIM
	read_header_tag(tag);
	DLOG(INFO) << boost::format("HEADER : 06 tag '%1%'") % tag << std::endl;
	read_header_int32(value_int32);
	read_header_int32(value_int32);
	o_header.ETIM = value_int32;
	DLOG(INFO) << boost::format("HEADER : 07 o_header.ETIM %1%") % o_header.ETIM << std::endl;

	return true;
}

bool MCMemoryReader::read_channel()
{
	std::string tag;
	int8_t value_int8;
	int16_t value_int16;
	int32_t value_int32;
	read_channel_tag(tag);
	DLOG(INFO) << boost::format("CHANNEL : 01 tag '%1%'") % tag << std::endl;

	do {

		read_channel_tag(tag);
		DLOG(INFO) << boost::format("CHANNEL : 02 tag '%1%'") % tag << std::endl;

		// Per channel data
		read_channel_pascal_string_32(tag);
		std::string channel_name = tag;
		DLOG(INFO) << boost::format("CHANNEL : 03 channel name '%1%'") % tag << std::endl;

		// _channels_data.push_back(ChannelData());
		std::pair<ChannelDataContainer::iterator,bool> ret;
		ret = _channels_data.insert(ChannelDataContainer::value_type(channel_name,ChannelData()));
		if (!ret.second)
			return false;
		ChannelDataContainer::iterator iter = ret.first;
//		if (boost::algorithm::ends_with(tag, "_count"))
//			read_channel_int16(value_int16);
//		else
//			read_channel_int8(value_int8);

		read_channel_tag(tag);
		DLOG(INFO) << boost::format("CHANNEL : 04 tag '%1%'") % tag << std::endl;
		read_channel_int32(value_int32);
		DLOG(INFO) << boost::format("CHANNEL : 05 value_int32 %1%") % value_int32 << std::endl;
		read_channel_int32(value_int32);
		size_t array_element_count = value_int32;
		DLOG(INFO) << boost::format("CHANNEL : 06 channel array_element_count %1%") % array_element_count << std::endl;

		read_channel_tag(tag);
		std::string array_data_type(tag);
		DLOG(INFO) << boost::format("CHANNEL : 07 tag array_data_type '%1%'") % array_data_type << std::endl;
		read_channel_int32(value_int32);
		size_t array_buffer_size = value_int32;
		DLOG(INFO) << boost::format("CHANNEL : 08 channel array_buffer_size %1%") % array_buffer_size << std::endl;

		// _channels_data.back()._name = channel_name;
		// _channels_data.back()._type = array_data_type;
		iter->second._type = string2ChannelDataType(array_data_type);

		if (array_data_type.compare("DBLA")==0)
		{
			// _channels_data.back()._dbla.resize(array_element_count);
			iter->second._dbla.resize(array_element_count);

			std::vector<double> dbla_buffer(array_element_count);
			read_channel_blob(array_buffer_size,dbla_buffer.data());

			for (size_t i=0;i<array_element_count;i++)
			{
				// _channels_data.back()._dbla[i] =  reverse_double(dbla_buffer[i]);
				iter->second._dbla[i] =  reverse_double(dbla_buffer[i]);
			}

		}
		else if (array_data_type.compare("FVCA")==0)
		{
			// _channels_data.back()._fvca.resize(array_element_count);
			iter->second._fvca.resize(array_element_count);

			std::vector<float> fvca_buffer(array_element_count*3);
			read_channel_blob(array_buffer_size,fvca_buffer.data());
			for (size_t i=0;i<array_element_count;i++)
			{
//				_channels_data.back()._fvca[i].x =  reverse_float(fvca_buffer[i*3+0]);
//				_channels_data.back()._fvca[i].y =  reverse_float(fvca_buffer[i*3+1]);
//				_channels_data.back()._fvca[i].z =  reverse_float(fvca_buffer[i*3+2]);

				iter->second._fvca[i].x =  reverse_float(fvca_buffer[i*3+0]);
				iter->second._fvca[i].y =  reverse_float(fvca_buffer[i*3+1]);
				iter->second._fvca[i].z =  reverse_float(fvca_buffer[i*3+2]);
			}

			//read_channel_blob(array_buffer_size,_channels_data.back()._fvca.data());
		}
		else if (array_data_type.compare("FBCA")==0)
		{
			// _channels_data.back()._fbca.resize(array_element_count);
			iter->second._fbca.resize(array_element_count);

			std::vector<float> fbca_buffer(array_element_count);
			read_channel_blob(array_buffer_size,fbca_buffer.data());

			for (size_t i=0;i<array_element_count;i++)
			{
				// _channels_data.back()._fbca[i] =  reverse_float(fbca_buffer[i]);
				iter->second._fbca[i] =  reverse_float(fbca_buffer[i]);

			}

		}
		else
			// DLOG(INFO) << boost::format("CHANNEL : UNHANDLED array type '%1%'") % array_data_type << std::endl;
			throw std::runtime_error((boost::format("CHANNEL : UNHANDLED array type '%1%'") % array_data_type).str());

		// bool more_data = can_read_more_channel_data(1);
	} while (can_read_more_channel_data(1));

	// DLOG(INFO) << boost::format("CHANNEL : 08 more_data %1%") % (more_data?"true":"false") << std::endl;

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
