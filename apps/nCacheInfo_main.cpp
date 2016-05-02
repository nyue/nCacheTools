// /corp/projects/surfup2/work/show/tst010/310/fx/hraab/maya/cache/nCache/fx_hraab_wave_v001
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
	if (argc == 2)
	{
		std::string xml_filename(argv[1]);
		nCache::nCacheLoader cache_loader(xml_filename);
		cache_loader.process();
	}
	return 0;
}

