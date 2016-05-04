#include "ChannelInfo.h"

using namespace nCache;

ChannelDataType string2ChannelDataType(const std::string& i_channel_data_type_string)
{
	if (i_channel_data_type_string.compare("DBLA")==0)
		return DBLA;
	else if (i_channel_data_type_string.compare("FVCA")==0)
		return FVCA;
	else if (i_channel_data_type_string.compare("FBCA")==0)
		return FBCA;
	else
		return UNKNOWN;
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
