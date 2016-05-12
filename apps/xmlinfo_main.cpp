#include "XMLReader.h"
#include <string>

int main(int argc, char** argv)
{
	if (argc != 2)
		return 1;
	google::InitGoogleLogging(argv[0]);
	std::string xml_filename(argv[1]);
	nCache::XMLReader xml_reader;
	xml_reader.read(xml_filename);
	xml_reader.debugDump();
	size_t sampling_rate         = xml_reader.get_particle_count_sampling_rate();
	std::cout << boost::format("sampling_rate = %1%") % sampling_rate << std::endl;
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
