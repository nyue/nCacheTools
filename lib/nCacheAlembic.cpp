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

}

nCacheAlembic::~nCacheAlembic()
{

}
