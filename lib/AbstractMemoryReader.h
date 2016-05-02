#pragma once

#include "InterfaceReader.h"
#include <string>
#include <vector>

namespace nCache
{

	class AbstractMemoryReader  : public InterfaceReader
	{
		typedef unsigned char DataBufferType;
		typedef DataBufferType* DataBufferPtr;
		typedef std::vector<DataBufferType> DataBufferContainer;
	public:
		AbstractMemoryReader(const std::string& i_ncache_filename);
		virtual ~AbstractMemoryReader();
	protected:
		// HEADER
		bool read_header_tag(std::string& o_tag);
		bool read_header_int32(int32_t& o_value);
		bool read_header_int64(int64_t& o_value);
		bool read_header_pascal_string_32(std::string& o_string);
		bool read_header_pascal_string_64(std::string& o_string);
		bool can_read_more_header_data(size_t i_bytes_to_read) const;

		// CHANNEL
		bool read_channel_tag(std::string& o_tag);
		bool read_channel_int8(int8_t& o_value);
		bool read_channel_int16(int16_t& o_value);
		bool read_channel_int32(int32_t& o_value);
		bool read_channel_int64(int64_t& o_value);
		bool read_channel_pascal_string_32(std::string& o_string);
		bool read_channel_pascal_string_64(std::string& o_string);
		bool read_channel_blob(size_t i_bytes_to_read, void* o_blob);
		bool can_read_more_channel_data(size_t i_bytes_to_read) const;

		template<typename T>
		bool read_blob(FILE* i_fp, size_t i_blob_size, T* o_blob) const;
		float reverse_float( const float inFloat ) const;
		float reverse_double( const double inDouble ) const;

	protected:
		DataBufferContainer _header_data_unsigned_char_buffer;
		DataBufferContainer _channel_data_unsigned_char_buffer;
		DataBufferPtr _header_data_current_ptr;
		DataBufferPtr _channel_data_current_ptr;
		DataBufferPtr _header_data_end_ptr;
		DataBufferPtr _channel_data_end_ptr;
		std::string _cache_filename;
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
