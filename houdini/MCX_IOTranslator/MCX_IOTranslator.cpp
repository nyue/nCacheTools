#include "MCX_IOTranslator.h"
#include "MCXMemoryReader.h"

MCX_IOTranslator    *MCX_IOTranslator::duplicate() const
{
    return new MCX_IOTranslator(*this);

}

const char *MCX_IOTranslator::formatName() const
{
    return "Autodesk MCX Cache Format";
}

int         MCX_IOTranslator::checkExtension(const char *name)
{
    UT_String           sname(name);

    if (sname.fileExtension() && !strcmp(sname.fileExtension(), ".mcx"))
        return true;
    return false;

}

int         MCX_IOTranslator::checkMagicNumber(unsigned magic)
{
	return 0;
}

GA_Detail::IOStatus  MCX_IOTranslator::fileLoad(GEO_Detail *gdp,
												UT_IStream &is,
												bool        ate_magic)
{
    UT_IStreamAutoBinary        forceascii(is, false);
    std::string mcx_filename = is.getFilename();

	nCache::MCXMemoryReader mcx_reader(mcx_filename);

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
//				csv_file << boost::format("%1%,") % iter->first;
				if (!num_rows_obtained)
				{
					num_rows = iter->second._dbla.size();
					num_rows_obtained = true;
				}
				break;
			case nCache::FVCA :
//				csv_file << boost::format("%1%.x,%1%.y,%1%.z,") % iter->first;
				if (!num_rows_obtained)
				{
					num_rows = iter->second._fvca.size();
					num_rows_obtained = true;
				}
				break;
			case nCache::FBCA :
//				csv_file << boost::format("%1%,") % iter->first;
				if (!num_rows_obtained)
				{
					num_rows = iter->second._fbca.size();
					num_rows_obtained = true;
				}
				break;
			}
		}
//		csv_file << std::endl;
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
//					csv_file << boost::format("%1%,") % iter->second._dbla[row_index];
					break;
				case nCache::FVCA :
//					csv_file << boost::format("%1%,%2%,%3%,") % iter->second._fvca[row_index].x % iter->second._fvca[row_index].y % iter->second._fvca[row_index].z ;
					break;
				case nCache::FBCA :
//					csv_file << boost::format("%1%,") % iter->second._fbca[row_index];
					break;
				}

			}
//			csv_file << std::endl;
		}
//		csv_file.close();
	}



    // All done successfully
    return GA_Detail::IOStatus(true);
}

GA_Detail::IOStatus  MCX_IOTranslator::fileSave(const GEO_Detail *gdp,
												std::ostream     &is)
{
    return GA_Detail::IOStatus(false);
}

void
newGeometryIO(void *)
{
    GU_Detail::registerIOTranslator(new MCX_IOTranslator());

    // Note due to the just-in-time loading of GeometryIO, the mcx
    // won't be added until after your first mcx save/load.
    // Thus this is replicated in the newDriverOperator.
    UT_ExtensionList        *geoextension;
    geoextension = UTgetGeoExtensions();
    if (!geoextension->findExtension("mcx"))
    geoextension->addExtension("mcx");
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
