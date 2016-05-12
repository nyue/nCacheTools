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

nCacheAlembic::nCacheAlembic(const std::string& i_alembic_filename,
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
    std::cout << boost::format("ncache_sampling_rate RATE = %1%") % ncache_sampling_rate << std::endl;

    std::vector< double > samps;
    // Alembic::Util::uint32_t num_time_samples = (i_ncache_loader.get_xml_reader().get_time_range_end() - i_ncache_loader.get_xml_reader().get_time_range_start())/i_ncache_loader.get_xml_reader().get_particle_count_sampling_rate();
    Alembic::Util::uint32_t num_time_samples = 0;
    // for (Alembic::Util::uint32_t sample_index = 0; sample_index < num_time_samples; sample_index++)
    for (size_t sample_tick = ncache_start; sample_tick <= ncache_end; sample_tick+=ncache_sampling_rate,num_time_samples++)
    {
        std::cout << boost::format("sample_tick = %1%, ncache_sampling_rate = %2%, fps = %3%") % sample_tick % ncache_sampling_rate % fps<< std::endl;
        double sampling_value = (sample_tick/ncache_sampling_rate)/fps;
        std::cout << boost::format("sampling_value = %1%") % sampling_value << std::endl;
    	samps.push_back(sampling_value);
    }
    std::cout << boost::format("num_time_samples = %1%") % num_time_samples << std::endl;
    // Create the time sampling type.
    Alembic::AbcCoreAbstract::TimeSampling ts(Alembic::AbcCoreAbstract::TimeSamplingType(num_time_samples, 1.0), samps);
    std::cout << boost::format("getNumStoredTimes = %1%") % ts.getNumStoredTimes() << std::endl;
    Alembic::Util::uint32_t tsidx = iParent.getArchive().addTimeSampling(ts);
}

nCacheAlembic::~nCacheAlembic()
{

}
