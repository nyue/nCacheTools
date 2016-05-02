#include "AbstractMemoryReader.h"
#include <iostream>
#include <stdexcept>
#include <stdio.h>

using namespace nCache;

AbstractMemoryReader::AbstractMemoryReader(const std::string& i_ncache_filename)
: _header_data_current_ptr(0)
, _channel_data_current_ptr(0)
, _header_data_end_ptr(0)
, _channel_data_end_ptr(0)
, _cache_filename(i_ncache_filename)
{
	FILE *_fp = fopen(i_ncache_filename.c_str(),"rb");
	if (_fp == 0)
		throw std::runtime_error((boost::format("Failed to open file '%1%'")%i_ncache_filename).str());
	else {
		std::string tag;
		size_t blob_size;
		int32_t tag_value_int32;
		int64_t tag_value_int64;
		// Header
		InterfaceReader::read_tag(_fp,tag);
		if (tag.compare("FOR4")==0)
		{
			InterfaceReader::read_int32t(_fp,tag_value_int32);
			blob_size = tag_value_int32;
		}
		else if (tag.compare("FOR8")==0)
		{
			InterfaceReader::read_int64t(_fp,tag_value_int64); // some 64bit data
			InterfaceReader::read_int32t(_fp,tag_value_int32);
			blob_size = tag_value_int32;
			DLOG(INFO) << boost::format("FOR8 blob_size xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx %1%") % blob_size;
			DLOG(INFO) << "00000000";
		}
		else
			throw std::runtime_error((boost::format("Failed to open file '%1%', unknown format '%2%'")%i_ncache_filename%tag).str());

		DLOG(INFO) << "00000001";
		_header_data_unsigned_char_buffer.resize(blob_size);
		DLOG(INFO) << "00000002";
		_header_data_current_ptr = _header_data_unsigned_char_buffer.data();
		DLOG(INFO) << "00000003";
		_header_data_end_ptr = _header_data_current_ptr + _header_data_unsigned_char_buffer.size();
		DLOG(INFO) << "00000004";
		read_blob<DataBufferType>(_fp, blob_size, _header_data_unsigned_char_buffer.data());
		DLOG(INFO) << boost::format("tag = '%1%', byte count = %2%") % tag % blob_size;
		// Channels
		InterfaceReader::read_tag(_fp,tag);
		if (tag.compare("FOR4")==0)
		{
			InterfaceReader::read_int32t(_fp,tag_value_int32);
			blob_size = tag_value_int32;
		}
		else if (tag.compare("FOR8")==0)
		{
			InterfaceReader::read_int64t(_fp,tag_value_int64); // some 64bit data
			InterfaceReader::read_int32t(_fp,tag_value_int32);
			blob_size = tag_value_int32;
		}
		else
			throw std::runtime_error((boost::format("Failed to open file '%1%', unknown format '%2%'")%i_ncache_filename%tag).str());
		_channel_data_unsigned_char_buffer.resize(blob_size);
		_channel_data_current_ptr = _channel_data_unsigned_char_buffer.data();
		_channel_data_end_ptr = _channel_data_current_ptr + _channel_data_unsigned_char_buffer.size();
		read_blob<DataBufferType>(_fp, blob_size, _channel_data_unsigned_char_buffer.data());
		DLOG(INFO) << boost::format("tag = '%1%', byte count = %2%") % tag % blob_size;
		fclose(_fp);
	}
}

AbstractMemoryReader::~AbstractMemoryReader()
{
}

// HEADER
bool AbstractMemoryReader::read_header_tag(std::string& o_tag)
{
	const size_t bytes_to_read = 4;
	if (!can_read_more_header_data(bytes_to_read))
		return false;
	char tag_string[bytes_to_read+1];
	memcpy(tag_string,_header_data_current_ptr,bytes_to_read);
	tag_string[bytes_to_read] = 0; // null termination
	_header_data_current_ptr += bytes_to_read;

	o_tag = tag_string;

	return true;
}

bool AbstractMemoryReader::read_header_int32(int32_t& o_value)
{
	const size_t bytes_to_read = sizeof(o_value);
	if (!can_read_more_header_data(bytes_to_read))
		return false;
	int32_t dummy_value;
	memcpy(&dummy_value,_header_data_current_ptr,bytes_to_read);
	o_value = htonl(dummy_value);
	_header_data_current_ptr += bytes_to_read;

	return true;
}

bool AbstractMemoryReader::read_header_int64(int64_t& o_value)
{
	const size_t bytes_to_read = sizeof(o_value);
	if (!can_read_more_header_data(bytes_to_read))
		return false;
	int64_t dummy_value;
	memcpy(&dummy_value,_header_data_current_ptr,bytes_to_read);

#ifdef __linux__
	o_value = htobe64(dummy_value);
#else
	o_value = ntohll(dummy_value);
#endif // __linux__

	_header_data_current_ptr += bytes_to_read;

	return true;
}

bool AbstractMemoryReader::read_header_pascal_string_32(std::string& o_string)
{
	int32_t bytes_to_read;
	read_header_int32(bytes_to_read);
	char pascal_string_buffer[4096];
	memcpy(pascal_string_buffer,_header_data_current_ptr,bytes_to_read);
	pascal_string_buffer[bytes_to_read] = 0;
	o_string = pascal_string_buffer;

	_header_data_current_ptr += bytes_to_read;

	return true;
}

bool AbstractMemoryReader::read_header_pascal_string_64(std::string& o_string)
{
	int64_t dummy_value;
	int32_t bytes_to_read;
	read_header_int64(dummy_value);
	read_header_int32(bytes_to_read);
	char pascal_string_buffer[4096];
	memcpy(pascal_string_buffer,_header_data_current_ptr,bytes_to_read);
	pascal_string_buffer[bytes_to_read] = 0;
	o_string = pascal_string_buffer;

	_header_data_current_ptr += bytes_to_read;

	return true;
}
bool AbstractMemoryReader::can_read_more_header_data(size_t i_bytes_to_read) const
{
	return ((_header_data_current_ptr + i_bytes_to_read) <= _header_data_end_ptr);
}

bool AbstractMemoryReader::can_read_more_channel_data(size_t i_bytes_to_read) const
{
	DLOG(INFO) << boost::format("can_read_more_channel_data : _channel_data_current_ptr %1%, _channel_data_end_ptr = %2%") % static_cast<void*>(_channel_data_current_ptr) % static_cast<void*>(_channel_data_end_ptr) << std::endl;
	return ((_channel_data_current_ptr + i_bytes_to_read) <= _channel_data_end_ptr);
}

// CHANNEL
bool AbstractMemoryReader::read_channel_tag(std::string& o_tag)
{
	const size_t bytes_to_read = 4;
	if (!can_read_more_channel_data(bytes_to_read))
		return false;
	char tag_string[bytes_to_read+1];
	memcpy(tag_string,_channel_data_current_ptr,bytes_to_read);
	tag_string[bytes_to_read] = 0; // null termination
	_channel_data_current_ptr += bytes_to_read;

	o_tag = tag_string;

	return true;
}

bool AbstractMemoryReader::read_channel_int8(int8_t& o_value)
{
	const size_t bytes_to_read = sizeof(o_value);
	if (!can_read_more_channel_data(bytes_to_read))
		return false;
	int8_t dummy_value;
	memcpy(&dummy_value,_channel_data_current_ptr,bytes_to_read);
	o_value = htonl(dummy_value);
	_channel_data_current_ptr += bytes_to_read;

	return true;
}

bool AbstractMemoryReader::read_channel_int16(int16_t& o_value)
{
	const size_t bytes_to_read = sizeof(o_value);
	if (!can_read_more_channel_data(bytes_to_read))
		return false;
	int16_t dummy_value;
	memcpy(&dummy_value,_channel_data_current_ptr,bytes_to_read);
	o_value = htonl(dummy_value);
	_channel_data_current_ptr += bytes_to_read;

	return true;
}

bool AbstractMemoryReader::read_channel_int32(int32_t& o_value)
{
	const size_t bytes_to_read = sizeof(o_value);
	if (!can_read_more_channel_data(bytes_to_read))
		return false;
	int32_t dummy_value;
	memcpy(&dummy_value,_channel_data_current_ptr,bytes_to_read);
	o_value = htonl(dummy_value);
	_channel_data_current_ptr += bytes_to_read;

	return true;
}

bool AbstractMemoryReader::read_channel_int64(int64_t& o_value)
{
	const size_t bytes_to_read = sizeof(o_value);
	if (!can_read_more_channel_data(bytes_to_read))
		return false;
	int64_t dummy_value;
	memcpy(&dummy_value,_channel_data_current_ptr,bytes_to_read);
#ifdef __linux__
	o_value = htobe64(dummy_value);
#else
	o_value = ntohll(dummy_value);
#endif // __linux__
	_channel_data_current_ptr += bytes_to_read;

	return true;
}

bool AbstractMemoryReader::read_channel_pascal_string_32(std::string& o_string)
{
	const int32_t modulo = 4;
	int32_t bytes_to_read;
	int32_t padded_bytes_to_read;
	read_channel_int32(bytes_to_read);
	DLOG(INFO) << boost::format("read_channel_pascal_string_32 bytes_to_read %1%") % bytes_to_read << std::endl;
	int32_t  bytes_to_read_modulus = bytes_to_read%modulo;
	if (bytes_to_read_modulus)
		padded_bytes_to_read = bytes_to_read + (modulo-bytes_to_read_modulus);
	else
		padded_bytes_to_read = bytes_to_read;
	DLOG(INFO) << boost::format("read_channel_pascal_string_32 padded_bytes_to_read %1%") % padded_bytes_to_read << std::endl;
	char pascal_string_buffer[4096];
	memcpy(pascal_string_buffer,_channel_data_current_ptr,padded_bytes_to_read);

	o_string = pascal_string_buffer;

	_channel_data_current_ptr += padded_bytes_to_read;

	return true;
}

bool AbstractMemoryReader::read_channel_pascal_string_64(std::string& o_string)
{
	const int32_t modulo = 8;
	int32_t bytes_to_read;
	int32_t padded_bytes_to_read;
	read_channel_int32(bytes_to_read);
	int32_t  bytes_to_read_modulus = bytes_to_read%modulo;
	DLOG(INFO) << boost::format("read_channel_pascal_string_64 bytes_to_read = %1% bytes_to_read_modulus = %2%") % bytes_to_read % bytes_to_read_modulus<< std::endl;
	if (bytes_to_read_modulus)
		padded_bytes_to_read = bytes_to_read + (modulo-bytes_to_read_modulus);
	else
		padded_bytes_to_read = bytes_to_read;
	DLOG(INFO) << boost::format("read_channel_pascal_string_64 padded_bytes_to_read %1%") % padded_bytes_to_read << std::endl;
	char pascal_string_buffer[4096];
	memcpy(pascal_string_buffer,_channel_data_current_ptr,padded_bytes_to_read);

	o_string = pascal_string_buffer;

	_channel_data_current_ptr += padded_bytes_to_read;

	return true;
}

bool AbstractMemoryReader::read_channel_blob(size_t i_bytes_to_read, void* o_blob)
{
	memcpy(o_blob,_channel_data_current_ptr,i_bytes_to_read);

	_channel_data_current_ptr += i_bytes_to_read;
	return true;
}

//

template<typename T>
bool AbstractMemoryReader::read_blob(FILE* i_fp, size_t i_blob_size, T* o_blob) const
{
	fread(o_blob,sizeof(T),i_blob_size,i_fp);
	return true;
}

float AbstractMemoryReader::reverse_float( const float inFloat ) const
{
   float retVal;
   char *floatToConvert = ( char* ) & inFloat;
   char *returnFloat = ( char* ) & retVal;

   // swap the bytes into a temporary buffer
   returnFloat[0] = floatToConvert[3];
   returnFloat[1] = floatToConvert[2];
   returnFloat[2] = floatToConvert[1];
   returnFloat[3] = floatToConvert[0];

   return retVal;
}

float AbstractMemoryReader::reverse_double( const double inDouble ) const
{
   double retVal;
   char *doubleToConvert = ( char* ) & inDouble;
   char *returnDouble = ( char* ) & retVal;

   // swap the bytes into a temporary buffer
   returnDouble[0] = doubleToConvert[7];
   returnDouble[1] = doubleToConvert[6];
   returnDouble[2] = doubleToConvert[5];
   returnDouble[3] = doubleToConvert[4];
   returnDouble[4] = doubleToConvert[3];
   returnDouble[5] = doubleToConvert[2];
   returnDouble[6] = doubleToConvert[1];
   returnDouble[7] = doubleToConvert[0];

   return retVal;
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
