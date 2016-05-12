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

    std::cout << boost::format("START = %1%") % i_ncache_loader.get_xml_reader().get_time_range_start() << std::endl;
    std::cout << boost::format("END = %1%") % i_ncache_loader.get_xml_reader().get_time_range_end() << std::endl;

    std::cout << boost::format("TIME PER FRAME = %1%") % i_ncache_loader.get_xml_reader().get_cacheTimePerFrame_TimePerFrame() << std::endl;
    std::cout << boost::format("SAMPLING RATE = %1%") % i_ncache_loader.get_xml_reader().get_particle_count_sampling_rate() << std::endl;

}

nCacheAlembic::~nCacheAlembic()
{

}
