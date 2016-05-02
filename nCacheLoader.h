#pragma once

#include "XMLReader.h"
#include <boost/shared_ptr.hpp>

namespace nCache
{

	class nCacheLoader
	{
	public:
		nCacheLoader(const std::string& i_ncache_xml_filename);
		~nCacheLoader();
		void process();
	protected:
		boost::shared_ptr<XMLReader> _xml_reader_ptr;
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
