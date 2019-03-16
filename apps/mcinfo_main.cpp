#include "MCMemoryReader.h"
#include <string>
#include <iostream>
#include <boost/format.hpp>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << boost::format("Author: Nicholas Yue (yue.nicholas@gmail.com)\nUsage : %1% <mcc file>") % argv[0] << std::endl;
		return 1;
	}
	google::InitGoogleLogging(argv[0]);
	std::string mcc_filename(argv[1]);
	nCache::MCMemoryReader mc_reader(mcc_filename);
	const nCache::ChannelDataContainer& channels_data = mc_reader.get_channels_data();
	nCache::ChannelDataContainer::const_iterator iter = channels_data.begin();
	nCache::ChannelDataContainer::const_iterator eIter = channels_data.end();
	for (;iter!=eIter;++iter)
	{
		std::cout << boost::format("Channel['%1%'] type = %2%") % iter->first % iter->second._type << std::endl;
	}
	return 0;
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
