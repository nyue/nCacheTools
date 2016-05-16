#pragma once
#include <string>
#include "nCacheLoader.h"

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>
#include <Alembic/AbcCoreOgawa/All.h>
#include <Alembic/Util/All.h>
#include <Alembic/Abc/All.h>
#include <Alembic/AbcCoreAbstract/All.h>

#include "MCMemoryReader.h"
#include "MCXMemoryReader.h"

namespace nCache
{

	class nCacheAlembic
	{
		typedef boost::shared_ptr<AbstractMemoryReader> CacheReaderSharedPointerType;
	public:
		nCacheAlembic();
		virtual ~nCacheAlembic();
		bool process(const std::string& i_alembic_filename, const nCacheLoader& i_ncache_loader);
	protected:
		void process_single_sample(const CacheReaderSharedPointerType& i_cache_reader_ptr,
				const std::string& i_position_channel_name,
				const std::string& i_id_channel_name,
				Alembic::AbcGeom::OPointsSchema &o_pSchema
				);
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
