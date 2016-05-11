#include <iostream>
#include <string>
#include <boost/format.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <glog/logging.h>

#include "nCacheLoader.h"

int
main (int argc, char **argv)
{
	google::InitGoogleLogging(argv[0]);
	if (argc != 3)
	{
		std::cerr << boost::format("Usage : %1% <input-ncache-xml> <output-alembic-file>") % argv[0] << std::endl;
		return 1;
	}
	std::string xml_filename(argv[1]);
	std::string alembic_filename(argv[2]);
	nCache::nCacheLoader cache_loader(xml_filename);
	cache_loader.process();

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
