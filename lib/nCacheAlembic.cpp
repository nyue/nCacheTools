#include "nCacheAlembic.h"

using namespace nCache;

Alembic::AbcGeom::OXform
addXform(Alembic::Abc::OObject parent, std::string name)
{
    Alembic::AbcGeom::OXform xform(parent, name.c_str());

    return xform;
}

nCacheAlembic::nCacheAlembic()
{
}

nCacheAlembic::~nCacheAlembic()
{

}

bool nCacheAlembic::process(const std::string& i_alembic_filename,
							 const nCacheLoader& i_ncache_loader)
{
	Alembic::Abc::OArchive archive(Alembic::Abc::CreateArchiveWithInfo(Alembic::AbcCoreHDF5::WriteArchive(),
                                                                       std::string(i_alembic_filename.c_str()),
                                                                       std::string("Procedural Insight Pty. Ltd."),
                                                                       std::string("Multiple Samples Random points"),
                                                                       Alembic::Abc::ErrorHandler::kThrowPolicy));

    Alembic::AbcGeom::OObject iParent( archive, Alembic::AbcGeom::kTop );
    Alembic::AbcGeom::OXform xform = addXform(iParent,"Xform");

    const nCache::XMLReader& xml_reader = i_ncache_loader.get_xml_reader();

    size_t ncache_start = xml_reader.get_time_range_start();
    size_t ncache_end = xml_reader.get_time_range_end();
    size_t ncache_ticks_per_frame = xml_reader.get_cacheTimePerFrame_TimePerFrame();
    size_t ncache_sampling_rate = xml_reader.get_particle_count_sampling_rate();
    double fps = 6000.0/ncache_ticks_per_frame;
    std::cout << boost::format("ncache_start = %1%") % ncache_start << std::endl;
    std::cout << boost::format("ncache_end = %1%") % ncache_end << std::endl;
    std::cout << boost::format("ncache_ticks_per_frame = %1%") % ncache_ticks_per_frame << std::endl;
    std::cout << boost::format("FPS (calculated) = %1%") % fps << std::endl;
    std::cout << boost::format("ncache_sampling_rate = %1%") % ncache_sampling_rate << std::endl;

    Alembic::Util::uint32_t num_time_samples = (i_ncache_loader.get_xml_reader().get_time_range_end() - i_ncache_loader.get_xml_reader().get_time_range_start())/i_ncache_loader.get_xml_reader().get_particle_count_sampling_rate() + 1;
    std::cout << boost::format("num_time_samples = %1%") % num_time_samples << std::endl;
    // Create the time sampling type.
    Alembic::AbcCoreAbstract::TimeSampling ts(ncache_sampling_rate/6000.0, ncache_start/(ncache_ticks_per_frame*fps));
    std::cout << boost::format("getNumStoredTimes = %1%") % ts.getNumStoredTimes() << std::endl;
    Alembic::Util::uint32_t tsidx = iParent.getArchive().addTimeSampling(ts);

    // Find the required channels
    std::string position_channel_name;
    std::string id_channel_name;
    if (!xml_reader.find_channel_ends_with("_position",position_channel_name))
    {
    	DLOG(INFO) << "BAD" << std::endl;
    	return false;
    }
    if (!xml_reader.find_channel_ends_with("_id",id_channel_name))
    {
    	DLOG(INFO) << "BAD" << std::endl;
    	return false;
    }
    std::cout << boost::format("position_channel_name = '%1%'") % position_channel_name << std::endl;

    // Create the Alembic OPoints schema
    // Create our object.
    Alembic::AbcGeom::OPoints partsOut( xform, "nCacheParticles", tsidx );

    // Add attributes
    Alembic::AbcGeom::OPointsSchema &pSchema = partsOut.getSchema();
    if (false)
    {
    	Alembic::AbcGeom::MetaData mdata;
    	SetGeometryScope( mdata, Alembic::AbcGeom::kVaryingScope );
    	Alembic::AbcGeom::OV3fArrayProperty velOut( pSchema, "velocity", mdata, tsidx );
    	Alembic::AbcGeom::OC3fArrayProperty rgbOut( pSchema, "Cs", tsidx );
    	Alembic::AbcGeom::OFloatArrayProperty ageOut( pSchema, "age", tsidx );
    }


    // Now process the individual *.mc/*.mcx files
	bool is_mcx = xml_reader.getCacheFormat().compare("mcx") == 0;
	std::string cache_extension("mc");
	if (is_mcx)
		cache_extension = "mcx";
	size_t start_frame           = ncache_start / ncache_ticks_per_frame;
	size_t end_frame             = ncache_end / ncache_ticks_per_frame;
	CacheReaderSharedPointerType cache_reader_ptr;
	for (size_t frame_index = start_frame; frame_index <= end_frame; frame_index++)
	{
		std::string per_initial_frame_full_path = (boost::format("%1%/%2%Frame%3%.%4%") % xml_reader.get_cache_directory() % xml_reader.get_cache_name() % frame_index % cache_extension).str();
	    // std::cout << boost::format("per_initial_frame_full_path = '%1%'") % per_initial_frame_full_path << std::endl;

		if (is_mcx)
			cache_reader_ptr.reset(new MCXMemoryReader(per_initial_frame_full_path));
		else
			cache_reader_ptr.reset(new MCMemoryReader(per_initial_frame_full_path));

		if (cache_reader_ptr)
		{
			const nCache::ChannelDataContainer& cdc = cache_reader_ptr->get_channels_data();

			nCache::ChannelDataContainer::const_iterator cdcPositionIter = cdc.find(position_channel_name);
		    std::vector<Alembic::AbcGeom::V3f> m_position;
		    std::vector<Alembic::Util::uint64_t> m_id;
			if (cdcPositionIter!=cdc.end())
			{
				if (cdcPositionIter->second._type == FVCA)
				{
					size_t i_num_points = cdcPositionIter->second._fvca.size();
					m_position.resize(i_num_points);
				    for (size_t pIndex=0;pIndex<i_num_points;pIndex++)
				    {
				    	m_position[pIndex].x = cdcPositionIter->second._fvca[pIndex].x;
				    	m_position[pIndex].y = cdcPositionIter->second._fvca[pIndex].y;
				    	m_position[pIndex].z = cdcPositionIter->second._fvca[pIndex].z;
				    }

				}
			}
			nCache::ChannelDataContainer::const_iterator cdcIdIter = cdc.find(id_channel_name);
			if (cdcIdIter!=cdc.end())
			{
				if (cdcIdIter->second._type == DBLA)
				{
					size_t i_num_points = cdcIdIter->second._dbla.size();
					m_id.resize(i_num_points);
				    for (size_t pIndex=0;pIndex<i_num_points;pIndex++)
				    {
				    	m_id[pIndex] = static_cast<Alembic::Util::uint64_t>(cdcIdIter->second._dbla[pIndex]);
				    }

				}

			}
		    Alembic::AbcGeom::V3fArraySample position_data ( m_position );
		    Alembic::AbcGeom::UInt64ArraySample id_data ( m_id );
		    Alembic::AbcGeom::OPointsSchema::Sample psamp(position_data,id_data);
		    pSchema.set( psamp );
			std::cout << boost::format("position_data.size() = %1%, id_data.size() = %2%") % position_data.size() % id_data.size() << std::endl;

		}
	    // sub frame data
		if ((ncache_sampling_rate < ncache_ticks_per_frame) && (frame_index!=end_frame))
		{
			size_t accumulated_ticks = ncache_sampling_rate;
			while ((accumulated_ticks < ncache_ticks_per_frame) && ((ncache_ticks_per_frame-accumulated_ticks)>=ncache_sampling_rate) )
			{
				std::string per_sub_frame_full_path = (boost::format("%1%/%2%Frame%3%Tick%4%.%5%") % xml_reader.get_cache_directory() % xml_reader.get_cache_name() % frame_index % accumulated_ticks % cache_extension).str();
				// std::cout << boost::format("\t""per_sub_frame_full_path '%1%'") % per_sub_frame_full_path << std::endl;
				accumulated_ticks += ncache_sampling_rate;
			}
		}
	}
	return true;
}

void nCacheAlembic::process_single_sample(const CacheReaderSharedPointerType& i_cache_reader_ptr,
										  const std::string& i_position_channel_name,
										  const std::string& i_id_channel_name,
											Alembic::AbcGeom::OPointsSchema &o_pSchema)
{
	const nCache::ChannelDataContainer& cdc = i_cache_reader_ptr->get_channels_data();

	nCache::ChannelDataContainer::const_iterator cdcPositionIter = cdc.find(i_position_channel_name);
    std::vector<Alembic::AbcGeom::V3f> m_position;
    std::vector<Alembic::Util::uint64_t> m_id;
	if (cdcPositionIter!=cdc.end())
	{
		if (cdcPositionIter->second._type == FVCA)
		{
			size_t i_num_points = cdcPositionIter->second._fvca.size();
			m_position.resize(i_num_points);
		    for (size_t pIndex=0;pIndex<i_num_points;pIndex++)
		    {
		    	m_position[pIndex].x = cdcPositionIter->second._fvca[pIndex].x;
		    	m_position[pIndex].y = cdcPositionIter->second._fvca[pIndex].y;
		    	m_position[pIndex].z = cdcPositionIter->second._fvca[pIndex].z;
		    }

		}
	}
	nCache::ChannelDataContainer::const_iterator cdcIdIter = cdc.find(i_id_channel_name);
	if (cdcIdIter!=cdc.end())
	{
		if (cdcIdIter->second._type == DBLA)
		{
			size_t i_num_points = cdcIdIter->second._dbla.size();
			m_id.resize(i_num_points);
		    for (size_t pIndex=0;pIndex<i_num_points;pIndex++)
		    {
		    	m_id[pIndex] = static_cast<Alembic::Util::uint64_t>(cdcIdIter->second._dbla[pIndex]);
		    }

		}

	}
    Alembic::AbcGeom::V3fArraySample position_data ( m_position );
    Alembic::AbcGeom::UInt64ArraySample id_data ( m_id );
    Alembic::AbcGeom::OPointsSchema::Sample psamp(position_data,id_data);
    o_pSchema.set( psamp );
	std::cout << boost::format("position_data.size() = %1%, id_data.size() = %2%") % position_data.size() % id_data.size() << std::endl;

}
