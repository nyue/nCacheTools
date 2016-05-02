#pragma once

#include "AbstractMemoryReader.h"
#include <string>

namespace nCache
{

	class MCXMemoryReader : public AbstractMemoryReader
	{
	public:
		MCXMemoryReader(const std::string& i_mcx_filename, const ChannelInfoContainer* i_channels_info = 0);
		virtual ~MCXMemoryReader();
	protected:
		// std::string& o_VRSN, int& o_STIM, int& o_ETIM
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
