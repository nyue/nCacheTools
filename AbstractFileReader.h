#pragma once

#include "InterfaceReader.h"
#include <string>

namespace nCache
{

	class AbstractFileReader : public InterfaceReader
	{
	public:
		AbstractFileReader(const std::string& i_ncache_filename);
		virtual ~AbstractFileReader();
	protected:
		bool read_string(std::string& o_string) const;
		bool read_nstring(size_t i_buffer_size, char* o_buffer) const;
		bool read_nstring(size_t i_buffer_size, std::string& o_string) const;
		/*!
		 * \brief nCache are fix 4-byte string
		 */
		bool read_tag(char o_tag[4]) const;
		bool read_int64(int64_t& o_value) const;
		bool read_int32(int32_t& o_value) const;
		bool read_int16(int16_t& o_value) const;
	private:
		FILE *_fp;
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
