#include "nCacheTranslator.h"
#include <extension/Extension.h>
#include <maya/MFnStringArrayData.h>
#include <maya/MFnStringData.h>
#include <maya/MGlobal.h>
#include <stdlib.h>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/multi_array.hpp>
#include <MCXMemoryReader.h>
#include <XMLReader.h>

//AtNode* nCacheTranslator::Init(CArnoldSession* session,
//							   MDagPath& dagPath,
//							   MString outputAttr)
//{
//    AiMsgInfo("[nCacheTranslator extension] Init()");
//
//	MStatus status;
//	CShapeTranslator::Init(session, dagPath, outputAttr);
//    MFnDagNode fnGPUCache(dagPath);
//    MPlug cachePathPlug = fnGPUCache.findPlug("cachePath");
//    MObject cachePathStringObject;
//    status = cachePathPlug.getValue(cachePathStringObject);
//    MFnStringData cachePathStringData(cachePathStringObject);
//    MString cachePathString = cachePathStringData.string(&status);
//
//    MGlobal::displayInfo("cachePathString START");
//    MGlobal::displayInfo(cachePathString);
//    MGlobal::displayInfo("cachePathString END");
//
//	return m_atNode;
//}

bool nCacheTranslator::RequiresMotionData()
{
	return false;
}

void* nCacheTranslator::creator()
{
	return new nCacheTranslator();
}

//void nCacheTranslator::NodeInitializer(CAbTranslator context)
//{
//    AiMsgInfo("[nCacheTranslator extension] NodeInitializer()");
//
//	CExtensionAttrHelper helper("cacheFile","points");
//	CShapeTranslator::MakeCommonAttributes(helper);
//}


void nCacheTranslator::getDataFromPlug(const MFnDependencyNode& i_fnNode,
									   const char*              i_plug_name,
									   MString&                 o_string,
									   MStatus&                 o_status)
{
	MPlug stringPlug = i_fnNode.findPlug(i_plug_name, &o_status);
	o_status = stringPlug.getValue(o_string);
}

void nCacheTranslator::getDataFromPlug(const MFnDependencyNode& i_fnNode,
									   const char*              i_plug_name,
									   MTime&                   o_time,
									   MStatus&                 o_status)
{
	MPlug timePlug = i_fnNode.findPlug(i_plug_name, &o_status);
	o_status = timePlug.getValue(o_time);
}

void nCacheTranslator::getDataFromPlug(const MFnDependencyNode& i_fnNode,
									   const char*              i_plug_name,
									   double&                  o_double,
									   MStatus&                 o_status)
{

}

void nCacheTranslator::getDataFromPlug(const MFnDependencyNode& i_fnNode,
									   const char*              i_plug_name,
									   int&                     o_int,
									   MStatus&                 o_status)
{
	MPlug intPlug = i_fnNode.findPlug(i_plug_name, &o_status);
	o_status = intPlug.getValue(o_int);
}

/*!
 * \remark This method is called before ::Export()
 */
AtNode* nCacheTranslator::CreateArnoldNodes()
{
	MStatus status;
    AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes()");
    AtNode* arnold_points = 0;
    {
    	MFnDagNode fnGPUCache(m_dagPath);
    	MPlug playFromCachePlug = fnGPUCache.findPlug("playFromCache");
    	bool connected = playFromCachePlug.isConnected(&status);
    	AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache is connected %s", (connected?"true":"false"));
    	if (connected)
    	{
    		MPlugArray connections;
    		bool asDst = true;
    		bool asSrc = false;
    		playFromCachePlug.connectedTo(connections, asDst, asSrc, &status);
    		unsigned int num_connections = connections.length();
        	AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache connection count %d", num_connections);
        	for (unsigned int i = 0 ; i < num_connections ; i++)
        	{
            	AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache connected name '%s'",connections[i].className());
            	MObject node = connections[i].node(&status);
            	MFnDependencyNode fnNode(node);
            	{
            		MString cachePathString;
            		getDataFromPlug(fnNode,"cachePath",cachePathString,status);
            		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache cache path '%s'",cachePathString.asChar());

            		MString cacheNameString;
            		getDataFromPlug(fnNode,"cacheName",cacheNameString,status);
            		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache cache name '%s'",cacheNameString.asChar());

            		MString xmlCacheFileNameString = cachePathString + cacheNameString + MString(".xml");
            		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache xmlCacheFileNameString '%s'",xmlCacheFileNameString.asChar());

            		nCache::XMLReader xml_reader;
            		if ( boost::filesystem::exists( xmlCacheFileNameString.asChar() ) )
            		{
            			xml_reader.read(xmlCacheFileNameString.asChar());
                		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache cache format = '%s'",xml_reader.get_cache_format().c_str());
                		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache cache type = '%s'",xml_reader.get_cache_type().c_str());

                		MTime sourceStart;
                		getDataFromPlug(fnNode,"sourceStart",sourceStart,status);
                		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache source start = %g",sourceStart.value());

                		MTime sourceEnd;
                		getDataFromPlug(fnNode,"sourceEnd",sourceEnd,status);
                		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache source end = %g",sourceEnd.value());

                		MTime startFrame;
                		getDataFromPlug(fnNode,"startFrame",startFrame,status);
                		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache start frame = %g",startFrame.value());

                		MTime currentFrame;
                		getDataFromPlug(fnNode,"time",currentFrame,status);
                		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache current frame = %g",currentFrame.value());

        				float fps_1 = xml_reader.get_cacheTimePerFrame_TimePerFrame()/6000.f;

                		arnold_points = Emit(currentFrame.value(),
                				sourceStart.value(),
								sourceEnd.value(),fps_1,
                				cachePathString.asChar(),
								cacheNameString.asChar(),
								xml_reader.get_cache_format());
            		}

            	}
        	}
    	}
    }
    return arnold_points;
}

void nCacheTranslator::Export(AtNode* atNode)
{
    AiMsgInfo("[nCacheTranslator extension] Exporting() %s", GetMayaNodeName().asChar());
}

void nCacheTranslator::ExtractDataSet(const std::string& i_dataCacheFileName, DataSet& o_dataSet)
{
	const float default_radius = 1.0f;
	nCache::MCXMemoryReader current_frame_mcx_reader(i_dataCacheFileName);
	nCache::ChannelDataContainer::const_iterator chIter = current_frame_mcx_reader.get_channels_data().begin();
	nCache::ChannelDataContainer::const_iterator chEIter = current_frame_mcx_reader.get_channels_data().end();
	for (;chIter!=chEIter;++chIter)
	{
		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache channel key '%s'",chIter->first.c_str());
		if (boost::algorithm::ends_with(chIter->first, "_position"))
		{
    		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache particle position count %d",chIter->second._fvca.size());

    		size_t num_particles = chIter->second._fvca.size();
    		for (size_t pid = 0; pid < num_particles; pid++)
    		{
    			o_dataSet._position.push_back(Imath::V3f(chIter->second._fvca[pid].x,chIter->second._fvca[pid].y,chIter->second._fvca[pid].z));
    			o_dataSet._radius.push_back(default_radius);
    		}
		}
		if (boost::algorithm::ends_with(chIter->first, "_velocity"))
		{
    		size_t num_particles = chIter->second._fvca.size();
    		for (size_t pid = 0; pid < num_particles; pid++)
    		{
    			o_dataSet._velocity.push_back(Imath::V3f(chIter->second._fvca[pid].x,chIter->second._fvca[pid].y,chIter->second._fvca[pid].z));
    		}
		}

		if (boost::algorithm::ends_with(chIter->first, "_id"))
		{
    		size_t num_particles = chIter->second._dbla.size();
    		for (size_t pid = 0; pid < num_particles; pid++)
    		{
    			o_dataSet._id.push_back(chIter->second._dbla[pid]);
    		}
		}

	}
}

AtNode* nCacheTranslator::Emit(int i_currentFrame, int i_startFrame, int i_endFrame, float i_fps_1,
		const std::string& i_cachePathString, const std::string& i_cacheNameString, const std::string& i_format)
{
//    std::vector<Imath::V3f> arnold_points_position;
//    std::vector<Imath::V3f> arnold_points_velocity;
//    std::vector<int> arnold_points_particle_id;
//    std::vector<float> arnold_points_radius;
//    const float default_radius = 1.0f;
    AtNode* arnold_points = 0;
	std::string currentFrameDataCacheFileNameString = i_cachePathString + i_cacheNameString + (boost::format("Frame%d.%s") % i_currentFrame % i_format.c_str()).str();
	AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache dataCacheFileNameString '%s'",currentFrameDataCacheFileNameString.c_str());

	if ( boost::filesystem::exists( currentFrameDataCacheFileNameString ) )
	{
		DataSet currentDataSet;

		ExtractDataSet(currentFrameDataCacheFileNameString, currentDataSet);

		if (!currentDataSet._position.empty())
		{
			arnold_points = AddArnoldNode("points");

			if (!currentDataSet._velocity.empty() && (currentDataSet._velocity.size() == currentDataSet._position.size()))
			{
	    		size_t num_particles = currentDataSet._velocity.size();

				typedef boost::multi_array<float, 3> MotionBlurredPositionType;
				typedef MotionBlurredPositionType::index index;
				MotionBlurredPositionType arnold_points_motion_blurred_position(boost::extents[2][num_particles][3]);
	    		for (size_t pid = 0; pid < num_particles; pid++)
	    		{
	    			// start
	    			arnold_points_motion_blurred_position[0][pid][0] = currentDataSet._position[pid].x;
	    			arnold_points_motion_blurred_position[0][pid][1] = currentDataSet._position[pid].y;
	    			arnold_points_motion_blurred_position[0][pid][2] = currentDataSet._position[pid].z;

	    			// end (blurred)
	    			arnold_points_motion_blurred_position[1][pid][0] = currentDataSet._position[pid].x + (i_fps_1 * currentDataSet._velocity[pid].x);
	    			arnold_points_motion_blurred_position[1][pid][1] = currentDataSet._position[pid].y + (i_fps_1 * currentDataSet._velocity[pid].y);
	    			arnold_points_motion_blurred_position[1][pid][2] = currentDataSet._position[pid].z + (i_fps_1 * currentDataSet._velocity[pid].z);
	    		}

				AiNodeSetArray(arnold_points, "points", AiArrayConvert(currentDataSet._position.size(), 2, AI_TYPE_POINT, arnold_points_motion_blurred_position.data()));

			}
			else
				AiNodeSetArray(arnold_points, "points", AiArrayConvert(currentDataSet._position.size(), 1, AI_TYPE_POINT, currentDataSet._position.data()));
		    AiNodeSetArray(arnold_points, "radius", AiArrayConvert(currentDataSet._radius.size(), 1, AI_TYPE_FLOAT, currentDataSet._radius.data()));

		}


	}
	return arnold_points;
}

extern "C"
{

	/* DLLEXPORT */ void initializeExtension(CExtension& extension)
	{
		MStatus status;
		// extension.Requires("cacheFile");
		status = extension.RegisterTranslator( "nParticle", // name of Maya node which this translator is associated with
											   "nicholas",
											   nCacheTranslator::creator
											   //,nCacheTranslator::NodeInitializer
											   );
		{
		       char *mtoa_extensions_path = getenv("MTOA_EXTENIONS_PATH");
		       if (mtoa_extensions_path)
		       {
					fprintf(stderr,"nCacheTranslator::initializeExtension() MTOA_EXTENIONS_PATH = '%s'\n",mtoa_extensions_path);
		       }
		}
		{
		MStringArray loaded_plugins = extension.GetAllLoadedArnoldPlugins();
		unsigned int num_plugins = loaded_plugins.length();
		for (unsigned int pid = 0; pid < num_plugins ; ++pid)
		{
			MString plugin_name = loaded_plugins[pid];
			fprintf(stderr,"nCacheTranslator::initializeExtension() '%s'\n",plugin_name.asChar());
		}
		}
	}

	/* DLLEXPORT */ void deinitializeExtension(CExtension& extension)
	{
	}

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
