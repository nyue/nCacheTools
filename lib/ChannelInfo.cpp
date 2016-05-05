#include "ChannelInfo.h"
#include <stdexcept>
#include <boost/format.hpp>

nCache::ChannelDataType nCache::string2ChannelDataType(const std::string& i_channel_data_type_string)
{
	if (i_channel_data_type_string.compare("DBLA")==0)
		return DBLA;
	else if (i_channel_data_type_string.compare("FVCA")==0)
		return FVCA;
	else if (i_channel_data_type_string.compare("FBCA")==0)
		return FBCA;
	else
		throw std::runtime_error((boost::format("Unknown channel data type string '%1%'") % i_channel_data_type_string).str());
		// return UNKNOWN;
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
