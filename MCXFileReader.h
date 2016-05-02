#pragma once

#include "AbstractFileReader.h"
#include <string>

namespace nCache
{

	class MCXFileReader : public AbstractFileReader
	{
	public:
		MCXFileReader(const ChannelInfoContainer& i_channels_info, const std::string& i_mcx_filename);
		virtual ~MCXFileReader();
	protected:
		// std::string& o_VRSN, int& o_STIM, int& o_ETIM
		virtual bool read_header(Header& o_header) const;
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
