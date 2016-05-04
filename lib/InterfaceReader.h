#pragma once

#include "ChannelInfo.h"
#include <glog/logging.h>
#include <boost/format.hpp>
#include <string>
#include <vector>
#include <map>
#ifdef WIN32
#include <Winsock2.h>
#else // WIN32
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#endif // WIN32
#include <stdio.h>

namespace nCache
{
	class InterfaceReader {
	public:
		InterfaceReader()
		{
		};
		virtual ~InterfaceReader() {};
		const ChannelDataContainer& get_channels_data() { return _channels_data; };
	protected:
		Header _header;
		ChannelDataContainer _channels_data;
	protected:
		virtual bool read_header(Header& o_header) = 0;
		virtual bool read_channel() = 0;

		static bool read_tag(FILE* i_fp, std::string& o_tag)
		{
			char buffer[5];
			fread(buffer,4,1,i_fp);
			buffer[4] = 0;
			o_tag = buffer;
			return true;
		}
		static bool read_int32t(FILE* i_fp, int32_t& o_int32_value)
		{
			int32_t _tempvalue;
			fread(&_tempvalue,sizeof(_tempvalue),1,i_fp);
			o_int32_value = ntohl(_tempvalue);
			return true;
		}
		bool read_int64t(FILE* i_fp, int64_t& o_int64_value) const
		{
			int64_t _tempvalue;
			fread(&_tempvalue,sizeof(_tempvalue),1,i_fp);
#ifdef __linux__
			o_int64_value = htobe64(_tempvalue);
#else
			o_int64_value = ntohll(_tempvalue);
#endif // __linux__
			return true;
		}
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
