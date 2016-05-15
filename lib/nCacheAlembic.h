#pragma once
#include <string>
#include "nCacheLoader.h"

namespace nCache
{

	class nCacheAlembic
	{
	public:
		nCacheAlembic();
		virtual ~nCacheAlembic();
		bool process(const std::string& i_alembic_filename, const nCacheLoader& i_ncache_loader);
	protected:
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
