#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#endif // WIN32
#include <errno.h>

#include <boost/format.hpp>

#include "nCacheLoader.h"
#include "MCFileReader.h"
#include "MCXFileReader.h"

#include "MCMemoryReader.h"
#include "MCXMemoryReader.h"

using namespace nCache;

nCacheLoader::nCacheLoader(const std::string& i_ncache_xml_filename)
{
	_xml_reader_ptr.reset(new XMLReader(i_ncache_xml_filename));
}

nCacheLoader::~nCacheLoader()
{
}

void nCacheLoader::process()
{
	size_t num_channels          = _xml_reader_ptr->getChannels().size();
	// size_t num_frames            = _xml_reader_ptr->get_num_frames();
	size_t cache_start_time_tick = _xml_reader_ptr->get_time_range_start();
	size_t cache_end_time_tick   = _xml_reader_ptr->get_time_range_end();
	size_t cache_per_frame_tick  = _xml_reader_ptr->get_cacheTimePerFrame_TimePerFrame();
	size_t start_frame           = cache_start_time_tick / cache_per_frame_tick;
	size_t end_frame             = cache_end_time_tick / cache_per_frame_tick;
	size_t sampling_rate         = _xml_reader_ptr->get_particle_count_sampling_rate();
//	std::cout << boost::format("cache_per_frame_tick = %1%") % cache_per_frame_tick << std::endl;
//	std::cout << boost::format("sampling_rate = %1%") % sampling_rate << std::endl;
	std::cout << boost::format("cache type = '%1%'") % _xml_reader_ptr->getCacheType() << std::endl;
	std::cout << boost::format("cache format = '%1%'") % _xml_reader_ptr->getCacheFormat() << std::endl;
	bool is_mcx = _xml_reader_ptr->getCacheFormat().compare("mcx") == 0;
	std::string cache_extension("mc");
	if (is_mcx)
		cache_extension = "mcx";
	for (size_t frame_index = start_frame; frame_index <= end_frame; frame_index++)
	{
		std::string per_initial_frame_mcx_full_path = (boost::format("%1%/%2%Frame%3%.%4%") % _xml_reader_ptr->get_cache_directory() % _xml_reader_ptr->get_cache_name() % frame_index % cache_extension).str();
		std::cout << boost::format("START processing %1%") % per_initial_frame_mcx_full_path << std::endl;

#ifdef FILE_BASED
		boost::shared_ptr<AbstractFileReader> cache_reader_ptr;
		if (is_mcx)
			cache_reader_ptr.reset(new MCXFileReader(_xml_reader_ptr->getChannels(),per_initial_frame_mcx_full_path));
		else
			cache_reader_ptr.reset(new MCFileReader(_xml_reader_ptr->getChannels(),per_initial_frame_mcx_full_path));
#else // FILE_BASED
		boost::shared_ptr<AbstractMemoryReader> cache_reader_ptr;
		if (is_mcx)
			cache_reader_ptr.reset(new MCXMemoryReader(per_initial_frame_mcx_full_path
														//, _xml_reader_ptr->getChannels()
														)
									);
		else
			cache_reader_ptr.reset(new MCMemoryReader(per_initial_frame_mcx_full_path
														//,_xml_reader_ptr->getChannels()
														)
									);
#endif // FILE_BASED
		std::cout << boost::format("DONE processing %1%") % per_initial_frame_mcx_full_path << std::endl;
		std::cout << boost::format("sampling_rate = %1%, cache_per_frame_tick = %2%, frame_index = %3%, end_frame = %4%")
				% sampling_rate
				% cache_per_frame_tick
				% frame_index
				% end_frame
				<< std::endl;
		if ((sampling_rate < cache_per_frame_tick) && (frame_index!=end_frame))
		{
			size_t accumulated_ticks = sampling_rate;
			while ((accumulated_ticks < cache_per_frame_tick) && ((cache_per_frame_tick-accumulated_ticks)>=sampling_rate) )
			{
				std::string per_sub_frame_mcx_full_path = (boost::format("%1%/%2%Frame%3%Tick%4%.%5%") % _xml_reader_ptr->get_cache_directory() % _xml_reader_ptr->get_cache_name() % frame_index % accumulated_ticks % cache_extension).str();
				std::cout << boost::format("\t%1%") % per_sub_frame_mcx_full_path << std::endl;
				accumulated_ticks += sampling_rate;
			}
		}
	}
	// _xml_reader_ptr->debugDump();

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
