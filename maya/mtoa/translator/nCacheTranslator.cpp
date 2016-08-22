#include "nCacheTranslator.h"
#include <extension/Extension.h>
#include <maya/MFnStringArrayData.h>
#include <maya/MFnStringData.h>
#include <maya/MGlobal.h>
#include <stdlib.h>

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

            		MTime sourceStart;
            		getDataFromPlug(fnNode,"sourceStart",sourceStart,status);
            		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache source start = %g",sourceStart.value());

            		MTime sourceEnd;
            		getDataFromPlug(fnNode,"sourceEnd",sourceEnd,status);
            		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache source end = %g",sourceEnd.value());

            		MTime startFrame;
            		getDataFromPlug(fnNode,"startFrame",startFrame,status);
            		AiMsgInfo("[nCacheTranslator extension] CreateArnoldNodes() playFromCache start frame = %g",startFrame.value());
            	}
        	}
    	}
    }
	return AddArnoldNode("sphere");
}

void nCacheTranslator::Export(AtNode* atNode)
{
    AiMsgInfo("[nCacheTranslator extension] Exporting() %s", GetMayaNodeName().asChar());
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
