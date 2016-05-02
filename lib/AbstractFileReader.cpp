#include "AbstractFileReader.h"
#include <iostream>
#include <stdexcept>
#include <boost/format.hpp>
#include <stdio.h>

using namespace nCache;

AbstractFileReader::AbstractFileReader(const std::string& i_ncache_filename)
{
	_fp = fopen(i_ncache_filename.c_str(),"rb");
	if (_fp == 0)
		throw std::runtime_error((boost::format("Failed to open file '%1%'")%i_ncache_filename).str());
}

AbstractFileReader::~AbstractFileReader()
{
	if(_fp)
		fclose(_fp);
}

bool AbstractFileReader::read_string(std::string& o_string) const
{
	char string_buffer[1024];
	size_t string_buffer_index=0;
	char c;
	do {
		fread(&c,sizeof(c),1,_fp);
		string_buffer[string_buffer_index] = c;
		string_buffer_index++;
	} while (c!=0);
	o_string = string_buffer;
	return true;
}

bool AbstractFileReader::read_nstring(size_t i_buffer_size, char* o_buffer) const
{
	fread(o_buffer,sizeof(char),i_buffer_size,_fp);
	return true;
}

bool AbstractFileReader::read_nstring(size_t i_buffer_size, std::string& o_string) const
{
	char* buffer = new char [i_buffer_size];
	if (buffer)
	{
		fread(buffer,sizeof(char),i_buffer_size,_fp);
		o_string = buffer;
		return true;
	}
	return false;
}

bool AbstractFileReader::read_tag(char o_tag[5]) const
{
	fread(o_tag,4,1,_fp);
	// terminate with null
	o_tag[4] = 0;
	return true;
}

bool AbstractFileReader::read_int64(int64_t& o_value) const
{
	fread(&o_value,sizeof(int64_t),1,_fp);
	return true;
}

bool AbstractFileReader::read_int32(int32_t& o_value) const
{
	int32_t value;
	fread(&value,sizeof(int32_t),1,_fp);
	o_value = ntohl(value);

	return true;
}

bool AbstractFileReader::read_int16(int16_t& o_value) const
{
	fread(&o_value,sizeof(int16_t),1,_fp);
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
