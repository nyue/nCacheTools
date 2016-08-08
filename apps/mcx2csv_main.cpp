#include "MCXMemoryReader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <boost/algorithm/string/predicate.hpp>

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr,"Usage : %s <mcx-input-file> <csv-output-file>\n",argv[0]);
		return 1;
	}
	google::InitGoogleLogging(argv[0]);
	std::string mcx_filename(argv[1]);
	std::string csv_filename(argv[2]);
	nCache::MCXMemoryReader mcx_reader(mcx_filename);
	std::ofstream csv_file;
	csv_file.open (csv_filename.c_str());

	const nCache::ChannelDataContainer& channels_data = mcx_reader.get_channels_data();
	if (!channels_data.empty())
	{
		nCache::ChannelDataContainer::const_iterator iter;
		nCache::ChannelDataContainer::const_iterator eIter = channels_data.end();
		int64_t num_rows = -1;
		bool num_rows_obtained = false;
		// Print CSV header
		for (iter = channels_data.begin();iter!=eIter;++iter)
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
		csv_file << std::endl;
		if (num_rows < 0)
			throw std::runtime_error("Negative rows");

		// Print output data
		for (int64_t row_index=0;row_index<num_rows;row_index++)
		{
			// Iterate the column
			for (iter = channels_data.begin();iter!=eIter;++iter)
			{
				switch (iter->second._type)
				{
				case nCache::DBLA :
					if (boost::algorithm::ends_with(iter->first, "_id"))
						csv_file << boost::format("%1%,") % int(iter->second._dbla[row_index]);
					else
						csv_file << boost::format("%1%,") % iter->second._dbla[row_index];
					break;
				case nCache::FVCA :
					csv_file << boost::format("%1%,%2%,%3%,") % iter->second._fvca[row_index].x % iter->second._fvca[row_index].y % iter->second._fvca[row_index].z ;
					break;
				case nCache::FBCA :
					if (boost::algorithm::ends_with(iter->first, "_id"))
						csv_file << boost::format("%1%,") % int(iter->second._fbca[row_index]);
					else
						csv_file << boost::format("%1%,") % iter->second._fbca[row_index];
					break;
				}

			}
			csv_file << std::endl;
		}
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
