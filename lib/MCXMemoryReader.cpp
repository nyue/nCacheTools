#include "MCXMemoryReader.h"
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

MCXMemoryReader::MCXMemoryReader(const std::string& i_mcx_filename, const ChannelInfoContainer* i_channels_info)
: AbstractMemoryReader(i_mcx_filename)
{
	bool result;
	result = read_header(_header);
	result = read_channel();
}

MCXMemoryReader::~MCXMemoryReader()
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
				// std::cout << boost::format("\t""%1%,%2%,%3%") % float(htonl(iter->_fvca[i].x)) % float(htonl(iter->_fvca[i].y)) % float(htonl(iter->_fvca[i].z)) << std::endl;
				std::cout << boost::format("\t""%1%,%2%,%3%") % iter->_fvca[i].x % iter->_fvca[i].y % iter->_fvca[i].z << std::endl;
				// std::cout << boost::format("\t""%1%,%2%,%3%") % float(ntohl(iter->_fvca[i].x)) % float(ntohl(iter->_fvca[i].y)) % float(ntohl(iter->_fvca[i].z)) << std::endl;
			}

		}
	}
#endif // PRINT_OUT
}

// std::string& o_VRSN, int& o_STIM, int& o_ETIM
bool MCXMemoryReader::read_header(Header& o_header)
{
	std::string tag;
	int32_t value_int32;
	int64_t value_int64;
	read_header_tag(tag);
	DLOG(INFO) << boost::format("HEADER : 01 tag '%1%'") % tag << std::endl;
	read_header_tag(tag);
	DLOG(INFO) << boost::format("HEADER : 02 tag '%1%'") % tag << std::endl;
	read_header_pascal_string_64(tag);
	DLOG(INFO) << boost::format("HEADER : 03 version '%1%'") % tag << std::endl;

	read_header_int32(value_int32);
	read_header_tag(tag);
	DLOG(INFO) << boost::format("HEADER : 04 tag '%1%'") % tag << std::endl;

	read_header_int64(value_int64);
	read_header_int32(value_int32);
	read_header_int32(value_int32);
	o_header.STIM = value_int32;
	DLOG(INFO) << boost::format("HEADER : 05 o_header.STIM %1%") % o_header.STIM << std::endl;

	return true;
}

bool MCXMemoryReader::read_channel()
{
	std::string tag;
	int8_t value_int8;
	int16_t value_int16;
	int32_t value_int32;
	int64_t value_int64;
	read_channel_tag(tag);
	DLOG(INFO) << boost::format("CHANNEL : 01 tag '%1%'") % tag << std::endl;

	size_t channel_index = 0;
	do {

		DLOG(INFO) << "CHANNEL : XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;

		read_channel_tag(tag);
		DLOG(INFO) << boost::format("CHANNEL : 02 tag '%1%'") % tag << std::endl;

		read_channel_int64(value_int64);
		// DLOG(INFO) << boost::format("CHANNEL : 03 value %1%") % value_int64 << std::endl;

		read_channel_pascal_string_64(tag);
		std::string channel_name = tag;
		DLOG(INFO) << boost::format("CHANNEL : 03 channel name '%1%'") % tag << std::endl;

		// _channels_data.push_back(ChannelData());
		std::pair<ChannelDataContainer::iterator,bool> ret;
		ret = _channels_data.insert(ChannelDataContainer::value_type(channel_name,ChannelData()));
		if (!ret.second)
			return false;
		ChannelDataContainer::iterator iter = ret.first;

		read_channel_tag(tag);
		DLOG(INFO) << boost::format("CHANNEL : 04 tag '%1%'") % tag << std::endl;

		read_channel_int64(value_int64);
		DLOG(INFO) << boost::format("CHANNEL : 05 value %1%") % value_int64 << std::endl;

		read_channel_pascal_string_64(tag);
		std::string object_name = tag;
		DLOG(INFO) << boost::format("CHANNEL : 05.5 object_name '%1%'") % object_name << std::endl;
#ifdef ABC
		read_channel_int32(value_int32);
		// read_channel_int32(value_int32);
		size_t array_element_count = value_int32;
		DLOG(INFO) << boost::format("CHANNEL : 06 array_element_count %1%") % array_element_count << std::endl;
#endif 
		size_t array_element_count = 0;//value_int32;

		// read_channel_int16(value_int16);
		read_channel_tag(tag);
		DLOG(INFO) << boost::format("CHANNEL : NICHOLAS 01 tag '%1%'") % tag << std::endl;

		read_channel_int32(value_int32);
		DLOG(INFO) << boost::format("CHANNEL : NICHOLAS 02") << std::endl;
		read_channel_int32(value_int32);
		DLOG(INFO) << boost::format("CHANNEL : NICHOLAS 03") << std::endl;
		read_channel_int32(value_int32);
		DLOG(INFO) << boost::format("CHANNEL : NICHOLAS 04") << std::endl;
		read_channel_int32(value_int32);
		DLOG(INFO) << boost::format("CHANNEL : NICHOLAS 05") << std::endl;
		size_t array_buffer_size = value_int32;
		read_channel_int32(value_int32);
		DLOG(INFO) << boost::format("CHANNEL : 08 array_buffer_size %1%") % array_buffer_size << std::endl;

		read_channel_tag(tag);
		std::string array_data_type = tag;
		DLOG(INFO) << boost::format("CHANNEL : 07 cache = '%1%' name = '%2%' array_data_type '%3%' channel_index = %4%") % _cache_filename % channel_name % array_data_type % channel_index << std::endl;

		// _channels_data.back()._name = channel_name;
		// _channels_data.back()._type = array_data_type;
		iter->second._type = string2ChannelDataType(array_data_type);

		if (array_data_type.compare("DBLA")==0)
		{
			DLOG(INFO) << boost::format("DBLA data type") << std::endl;

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
			DLOG(INFO) << boost::format("FCVA data type") << std::endl;

			const int32_t modulo = 8;
			int32_t  array_buffer_size_modulus = array_buffer_size%modulo;
			int32_t padded_array_buffer_size = array_buffer_size;

			if (array_buffer_size_modulus)
				padded_array_buffer_size = array_buffer_size + (modulo-array_buffer_size_modulus);

			// _channels_data.back()._fvca.resize(array_element_count);
			iter->second._fvca.resize(array_element_count);

			DLOG(INFO) << boost::format("MCX READ CHANNEL array_buffer_size = %1% array_element_count = %2% padded_array_buffer_size = %3%") % array_buffer_size % array_element_count % padded_array_buffer_size;

#ifdef OLD
			std::vector<float> fvca_buffer(array_element_count*3);
			read_channel_blob(array_buffer_size,fvca_buffer.data());
			for (size_t i=0;i<array_element_count;i++)
			{
				_channels_data.back()._fvca[i].x =  reverse_float(fvca_buffer[i*3+0]);
				_channels_data.back()._fvca[i].y =  reverse_float(fvca_buffer[i*3+1]);
				_channels_data.back()._fvca[i].z =  reverse_float(fvca_buffer[i*3+2]);

			}
#else
			std::vector<char> padded_fvca_buffer(padded_array_buffer_size);
			float* fvca_buffer_ptr = reinterpret_cast<float*>(padded_fvca_buffer.data());
			read_channel_blob(padded_array_buffer_size,padded_fvca_buffer.data());
			for (size_t i=0;i<array_element_count;i++)
			{
//				_channels_data.back()._fvca[i].x =  reverse_float(fvca_buffer_ptr[i*3+0]);
//				_channels_data.back()._fvca[i].y =  reverse_float(fvca_buffer_ptr[i*3+1]);
//				_channels_data.back()._fvca[i].z =  reverse_float(fvca_buffer_ptr[i*3+2]);

				iter->second._fvca[i].x =  reverse_float(fvca_buffer_ptr[i*3+0]);
				iter->second._fvca[i].y =  reverse_float(fvca_buffer_ptr[i*3+1]);
				iter->second._fvca[i].z =  reverse_float(fvca_buffer_ptr[i*3+2]);

			}
#endif
		}
		else
			// DLOG(INFO) << boost::format("CHANNEL : UNHANDLED array type '%1%'") % array_data_type << std::endl;
			throw std::runtime_error((boost::format("CHANNEL : UNHANDLED array type '%1%'") % array_data_type).str());
		channel_index++;
	} while(can_read_more_channel_data(1));

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
