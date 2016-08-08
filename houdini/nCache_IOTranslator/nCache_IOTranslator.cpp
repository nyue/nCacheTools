#include "nCache_IOTranslator.h"
#include <MCXMemoryReader.h>
#include <MCMemoryReader.h>

nCache_IOTranslator    *nCache_IOTranslator::duplicate() const
{
    return new nCache_IOTranslator(*this);

}

const char *nCache_IOTranslator::formatName() const
{
    return "Autodesk MCX Cache Format (Read-Only)\n\tExtensions: .mc .mcx";
}

int         nCache_IOTranslator::checkExtension(const char *name)
{
    UT_String           sname(name);

    _file_extension = sname.fileExtension();
    if (sname.fileExtension() &&
    		(!strcmp(sname.fileExtension(), ".mcx")
			||
    		!strcmp(sname.fileExtension(), ".mc"))
			)
    {
        return true;
    }
    return false;

}

int         nCache_IOTranslator::checkMagicNumber(unsigned magic)
{
	return 0;
}

GA_Detail::IOStatus  nCache_IOTranslator::fileLoad(GEO_Detail *gdp,
												UT_IStream &is,
												bool        ate_magic)
{
    UT_IStreamAutoBinary        forceascii(is, false);
    std::string ncache_filename = is.getFilename();

    if (_file_extension.compare(".mcx")==0)
    	_memory_reader_ptr.reset(new nCache::MCXMemoryReader(ncache_filename));
    else if (_file_extension.compare(".mc")==0)
    	_memory_reader_ptr.reset(new nCache::MCMemoryReader(ncache_filename));
    else
        return GA_Detail::IOStatus(false);

	const nCache::ChannelDataContainer& channels_data = _memory_reader_ptr->get_channels_data();
	if (!channels_data.empty())
	{
		nCache::ChannelDataContainer::const_iterator iter;
		nCache::ChannelDataContainer::const_iterator eIter = channels_data.end();

		/*
		 * Find the number of rows which correspond to the number of particles
		 * We iterate the container because we cannot tell in advance which
		 * cache data type can be found i.e. DBLA or FVCA or FBCA
		 */
		int64_t num_rows = -1;
		bool num_rows_obtained = false;
		for (iter = channels_data.begin();iter!=eIter;++iter)
		{
			switch (iter->second._type)
			{
			case nCache::DBLA :
				if (!num_rows_obtained)
				{
					num_rows = iter->second._dbla.size();
					num_rows_obtained = true;
				}
				break;
			case nCache::FVCA :
				if (!num_rows_obtained)
				{
					num_rows = iter->second._fvca.size();
					num_rows_obtained = true;
				}
				break;
			case nCache::FBCA :
				if (!num_rows_obtained)
				{
					num_rows = iter->second._fbca.size();
					num_rows_obtained = true;
				}
				break;
			default:
				break;
			}
		}

		// Early return is valid if no suitable particle cache data is found
		if (!num_rows_obtained)
		    return GA_Detail::IOStatus(true);

        size_t numParticles = num_rows;

        // MUST handle "P" first as it sets the point range for all the other
        // attributes
        bool P_processed = false;
		for (iter = channels_data.begin();iter!=eIter;++iter)
		{
			std::string attribute_name =
					AbstractNCache_IOTranslator::postfixMatched(iter->first);
			if (!attribute_name.empty())
			{
				switch (iter->second._type)
				{
				case nCache::FVCA :
					// P is special case for Houdini points
					if (attribute_name.compare("P")==0)
					{
						gdp->appendPointBlock(numParticles);
						GA_Range p_range = gdp->getPointRange();

						UT_ValArray<UT_Vector3> v3_array(numParticles);
						for (size_t i = 0; i<numParticles;i++)
							v3_array.array()[i].assign(iter->second._fvca[i].x,
													   iter->second._fvca[i].y,
													   iter->second._fvca[i].z);
						gdp->setPos3FromArray(p_range,v3_array);
					}
					P_processed = true;
					break;
				default:
					break;
				}
			}
		}
        // Other channels/attributes
        if (!P_processed)
        	return GA_Detail::IOStatus(false);
		for (iter = channels_data.begin();iter!=eIter;++iter)
		{
			std::string attribute_name =
					AbstractNCache_IOTranslator::postfixMatched(iter->first);
			if (!attribute_name.empty())
			{
				switch (iter->second._type)
				{
				case nCache::DBLA :
				{
					// ID is special case, 32 bit int
					if (attribute_name.compare("id")==0)
					{
						GA_RWHandleI int32_attrib(gdp->findAttribute(GA_ATTRIB_POINT,attribute_name));
						if (!int32_attrib.isValid())
						{
							int32_attrib.bind(gdp->addIntTuple(GA_ATTRIB_POINT, attribute_name, 1));
						}

						int32_attrib.getAttribute()->setTypeInfo(GA_TYPE_VOID);

						UT_ValArray<GA_RWHandleI::BASETYPE> int32_array(numParticles);
						for (size_t i = 0; i<numParticles;i++)
						{
							// float_array.array()[i] = channel_data_array[i];
							int32_array.array()[i] = iter->second._dbla[i];
						}
						gdp->setAttributeFromArray(int32_attrib.getAttribute(),gdp->getPointRange(),int32_array);
					}
					else
					{
						GA_RWHandleD double_attrib(gdp->findAttribute(GA_ATTRIB_POINT,attribute_name));
						if (!double_attrib.isValid())
						{
							double_attrib.bind(gdp->addFloatTuple(GA_ATTRIB_POINT, attribute_name, 1));
						}

						double_attrib.getAttribute()->setTypeInfo(GA_TYPE_VOID);

						UT_ValArray<GA_RWHandleD::BASETYPE> double_array(numParticles);
						for (size_t i = 0; i<numParticles;i++)
							// float_array.array()[i] = channel_data_array[i];
						double_array.array()[i] = iter->second._dbla[i];
						gdp->setAttributeFromArray(double_attrib.getAttribute(),gdp->getPointRange(),double_array);
					}
				}
				break;
				case nCache::FVCA :
					{
                        GA_RWHandleV3 v3_attrib(gdp->findAttribute(GA_ATTRIB_POINT,attribute_name));
                        if (!v3_attrib.isValid())
                        {
                            v3_attrib.bind(gdp->addFloatTuple(GA_ATTRIB_POINT, attribute_name, 3));
                        }

                        v3_attrib.getAttribute()->setTypeInfo(GA_TYPE_VECTOR);

                        UT_ValArray<UT_Vector3> v3_array(numParticles);
                        for (size_t i = 0; i<numParticles;i++)
                            v3_array.array()[i].assign(iter->second._fvca[i].x,
                            						   iter->second._fvca[i].y,
													   iter->second._fvca[i].z);
                        gdp->setAttributeFromArray(v3_attrib.getAttribute(),gdp->getPointRange(),v3_array);
					}
					break;
				case nCache::FBCA :
					break;
				default:
					break;
				}
			}
		}

	}

    // All done successfully
    return GA_Detail::IOStatus(true);
}

GA_Detail::IOStatus  nCache_IOTranslator::fileSave(const GEO_Detail *gdp,
												   std::ostream     &is)
{
    return GA_Detail::IOStatus(false);
}

void
newGeometryIO(void *)
{
    GU_Detail::registerIOTranslator(new nCache_IOTranslator());

    // Note due to the just-in-time loading of GeometryIO, the mc/mcx
    // won't be added until after your first mc/mcx save/load.
    // Thus this is replicated in the newDriverOperator.
    UT_ExtensionList        *geoextension;
    geoextension = UTgetGeoExtensions();
    if (!geoextension->findExtension("mcx"))
    	geoextension->addExtension("mcx");
    if (!geoextension->findExtension("mc"))
    	geoextension->addExtension("mc");
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
