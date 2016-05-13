#include "nCacheAlembic.h"

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>
#include <Alembic/AbcCoreOgawa/All.h>
#include <Alembic/Util/All.h>
#include <Alembic/Abc/All.h>
#include <Alembic/AbcCoreAbstract/All.h>

using namespace nCache;

Alembic::AbcGeom::OXform
addXform(Alembic::Abc::OObject parent, std::string name)
{
    Alembic::AbcGeom::OXform xform(parent, name.c_str());

    return xform;
}

nCacheAlembic::nCacheAlembic()
{
}

nCacheAlembic::~nCacheAlembic()
{

}

void nCacheAlembic::process(const std::string& i_alembic_filename,
							 const nCacheLoader& i_ncache_loader)
{
	Alembic::Abc::OArchive archive(Alembic::Abc::CreateArchiveWithInfo(Alembic::AbcCoreHDF5::WriteArchive(),
                                                                       std::string(i_alembic_filename.c_str()),
                                                                       std::string("Procedural Insight Pty. Ltd."),
                                                                       std::string("Multiple Samples Random points"),
                                                                       Alembic::Abc::ErrorHandler::kThrowPolicy));

    Alembic::AbcGeom::OObject iParent( archive, Alembic::AbcGeom::kTop );
    Alembic::AbcGeom::OXform xform = addXform(iParent,"Xform");

    const nCache::XMLReader& xml_reader = i_ncache_loader.get_xml_reader();

    size_t ncache_start = xml_reader.get_time_range_start();
    size_t ncache_end = xml_reader.get_time_range_end();
    size_t ncache_ticks_per_frame = xml_reader.get_cacheTimePerFrame_TimePerFrame();
    size_t ncache_sampling_rate = xml_reader.get_particle_count_sampling_rate();
    double fps = 6000.0/ncache_ticks_per_frame;
    std::cout << boost::format("ncache_start = %1%") % ncache_start << std::endl;
    std::cout << boost::format("ncache_end = %1%") % ncache_end << std::endl;
    std::cout << boost::format("ncache_ticks_per_frame = %1%") % ncache_ticks_per_frame << std::endl;
    std::cout << boost::format("FPS (calculated) = %1%") % fps << std::endl;
    std::cout << boost::format("ncache_sampling_rate = %1%") % ncache_sampling_rate << std::endl;

    Alembic::Util::uint32_t num_time_samples = (i_ncache_loader.get_xml_reader().get_time_range_end() - i_ncache_loader.get_xml_reader().get_time_range_start())/i_ncache_loader.get_xml_reader().get_particle_count_sampling_rate() + 1;
    std::cout << boost::format("num_time_samples = %1%") % num_time_samples << std::endl;
    // Create the time sampling type.
    Alembic::AbcCoreAbstract::TimeSampling ts(ncache_sampling_rate/(6000.0*fps), ncache_start/(ncache_ticks_per_frame*fps));
    std::cout << boost::format("getNumStoredTimes = %1%") % ts.getNumStoredTimes() << std::endl;
    Alembic::Util::uint32_t tsidx = iParent.getArchive().addTimeSampling(ts);


    // Now process the individual *.mc/*.mcx files
	bool is_mcx = xml_reader.getCacheFormat().compare("mcx") == 0;
	std::string cache_extension("mc");
	if (is_mcx)
		cache_extension = "mcx";
	size_t start_frame           = ncache_start / ncache_ticks_per_frame;
	size_t end_frame             = ncache_end / ncache_ticks_per_frame;
	for (size_t frame_index = start_frame; frame_index <= end_frame; frame_index++)
	{
		std::string per_initial_frame_full_path = (boost::format("%1%/%2%Frame%3%.%4%") % xml_reader.get_cache_directory() % xml_reader.get_cache_name() % frame_index % cache_extension).str();
	    std::cout << boost::format("per_initial_frame_full_path = '%1%'") % per_initial_frame_full_path << std::endl;

	    // sub frame data
		if ((ncache_sampling_rate < ncache_ticks_per_frame) && (frame_index!=end_frame))
		{
			size_t accumulated_ticks = ncache_sampling_rate;
			while ((accumulated_ticks < ncache_ticks_per_frame) && ((ncache_ticks_per_frame-accumulated_ticks)>=ncache_sampling_rate) )
			{
				std::string per_sub_frame_full_path = (boost::format("%1%/%2%Frame%3%Tick%4%.%5%") % xml_reader.get_cache_directory() % xml_reader.get_cache_name() % frame_index % accumulated_ticks % cache_extension).str();
				std::cout << boost::format("\t""per_sub_frame_full_path '%1%'") % per_sub_frame_full_path << std::endl;
				accumulated_ticks += ncache_sampling_rate;
			}
		}
	}
}
