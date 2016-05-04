#pragma once

#include <string>
#include <stdexcept>
#include <iostream>
#include <map>
#include <boost/format.hpp>

namespace nCache
{


	struct Header
	{
		std::string VRSN; // Version Number (char*)
		int STIM;		  // Start Time of the Cache File (int)
		int ETIM;		  // End Time of the Cache File (int)
	};
	struct FloatVector
	{
		float x;
		float y;
		float z;
	};
	enum ChannelDataType
	{
		DBLA,
		FVCA,
		FBCA,
		UNKNOWN
	};
	ChannelDataType string2ChannelDataType(const std::string& i_channel_data_type_string);
	struct ChannelData {
		std::string _type;
		// ChannelDataType _type;
		std::vector <double> _dbla;
		std::vector <float> _fbca;
		std::vector <FloatVector> _fvca;
	};
	typedef std::map<std::string,ChannelData> ChannelDataContainer;


	struct ChannelInfo
	{
		enum SamplingType { SAMPLING_REGULAR, SAMPLING_IRREGULAR };
		// std::string  _channel_name;
		std::string  _channel_type;
		std::string  _channel_interpretation;
		SamplingType _sampling_type;
		size_t       _sampling_rate;
		size_t       _start_time;
		size_t       _end_time;
		friend std::ostream& operator << (std::ostream& os, const ChannelInfo& rhs)
		{
			// os << boost::format("Name = '%1%'") % rhs._channel_name << std::endl;
			os << boost::format("Type = %1%") % rhs._channel_type << std::endl;
			os << boost::format("Interpretation = %1%") % rhs._channel_interpretation << std::endl;
			os << boost::format("Sampling type = %1%") % (rhs._sampling_type==SAMPLING_REGULAR?"Regular":"Irregular") << std::endl;
			os << boost::format("Sampling rate = %1%") % rhs._sampling_rate << std::endl;
			os << boost::format("Start time = %1%") % rhs._start_time << std::endl;
			os << boost::format("End time = %1%") % rhs._end_time << std::endl;
		    return os;
		}
	};

	// typedef std::vector<ChannelInfo> ChannelInfoContainer;
	typedef std::map<std::string,ChannelInfo> ChannelInfoContainer;
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
