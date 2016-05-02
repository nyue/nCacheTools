#pragma once

#include "AbstractMemoryReader.h"
#include <string>

namespace nCache
{

	class MCMemoryReader : public AbstractMemoryReader
	{
	public:
		MCMemoryReader(const std::string& i_mc_filename, const ChannelInfoContainer* i_channels_info = 0);
		virtual ~MCMemoryReader();
	protected:
		virtual bool read_header(Header& o_header);
		virtual bool read_channel();
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
