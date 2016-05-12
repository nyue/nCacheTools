#pragma once

#include "XMLReader.h"
#include <boost/shared_ptr.hpp>

namespace nCache
{

	class nCacheLoader
	{
	public:
		nCacheLoader();
		~nCacheLoader();
		void process(const std::string& i_ncache_xml_filename);
		const XMLReader& get_xml_reader() const { return _xml_reader; }
		const std::string& get_ncache_xml_filename() const { return _ncache_xml_filename; }
	protected:
		XMLReader _xml_reader;
		std::string _ncache_xml_filename;
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
