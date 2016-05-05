#include "MCMemoryReader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr,"Usage : %s <mcc-input-file> <csv-output-file>\n",argv[0]);
		return 1;
	}
	google::InitGoogleLogging(argv[0]);
	std::string mcc_filename(argv[1]);
	std::string csv_filename(argv[2]);
	nCache::MCMemoryReader mc_reader(mcc_filename);
	std::ofstream csv_file;
	csv_file.open (csv_filename.c_str());

	const nCache::ChannelDataContainer& channels_data = mc_reader.get_channels_data();
	if (!channels_data.empty())
	{
	nCache::ChannelDataContainer::const_iterator iter = channels_data.begin();
	nCache::ChannelDataContainer::const_iterator eIter = channels_data.end();
	int64_t num_rows = -1;
	bool num_rows_obtained = false;
	// Print CSV header
	for (;iter!=eIter;++iter)
	{
		// std::cout << boost::format("Channel['%1%'] type = %2%") % iter->first % nCache::ChannelDataType2string(iter->second._type) << std::endl;
		switch (iter->second._type)
		{
		case nCache::DBLA :
			csv_file << boost::format("%1%,") % iter->first;
			if (!num_rows_obtained)
			{
				num_rows = iter->second._dbla.size();
				num_rows_obtained = true;
			}
			break;
		case nCache::FVCA :
			csv_file << boost::format("%1%.x,%1%.y,%1%.z,") % iter->first;
			if (!num_rows_obtained)
			{
				num_rows = iter->second._fvca.size();
				num_rows_obtained = true;
			}
			break;
		case nCache::FBCA :
			csv_file << boost::format("%1%,") % iter->first;
			if (!num_rows_obtained)
			{
				num_rows = iter->second._fbca.size();
				num_rows_obtained = true;
			}
			break;
		}
	}
	if (num_rows < 0)
		throw std::runtime_error("Negative rows");

	// Print output data
	for (;iter!=eIter;++iter)
	{
		switch (iter->second._type)
		{
		case nCache::DBLA :
			csv_file << boost::format("%1%,") % iter->first;
			break;
		case nCache::FVCA :
			csv_file << boost::format("%1%.x,%1%.y,%1%.z,") % iter->first;
			break;
		case nCache::FBCA :
			csv_file << boost::format("%1%,") % iter->first;
			break;
		}

	}

	csv_file << std::endl;
	csv_file.close();
	}
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
