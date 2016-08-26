#pragma once

#include <translators/shape/ShapeTranslator.h>
#include <ai.h>
#include <OpenEXR/ImathVec.h>

class nCacheTranslator : public CShapeTranslator
{
	typedef std::vector<Imath::V3f> VectorContainer;
    typedef std::vector<int> IntContainer;
    typedef std::vector<float> FloatContainer;
	struct DataSet {
    	VectorContainer _position;
    	VectorContainer _velocity;
    	IntContainer    _id;
    	FloatContainer  _radius;
	};
public:
	// virtual AtNode* Init(CArnoldSession* session, MDagPath& dagPath, MString outputAttr="");
	bool RequiresMotionData();
	static void* creator();
	// static void NodeInitializer(CAbTranslator context);
protected:
	virtual AtNode* CreateArnoldNodes();
	virtual void Export(AtNode* blocker);
	void getDataFromPlug(const MFnDependencyNode& i_fnNode,
						 const char*              i_plug_name,
						 MString&                 o_string,
						 MStatus&                 o_status);
	void getDataFromPlug(const MFnDependencyNode& i_fnNode,
						 const char*              i_plug_name,
						 MTime&                   o_time,
						 MStatus&                 o_status);
	void getDataFromPlug(const MFnDependencyNode& i_fnNode,
						 const char*              i_plug_name,
						 double&                  o_double,
						 MStatus&                 o_status);
	void getDataFromPlug(const MFnDependencyNode& i_fnNode,
						 const char*              i_plug_name,
						 int&                     o_int,
						 MStatus&                 o_status);
	/*!
	 * \todo : Check m_motionDeform before doing motion blur stuff
	 */
	AtNode* Emit(int i_currentFrame, int i_startFrame, int i_endFrame, float i_fps_1,
			const std::string& i_cachePathString, const std::string& i_cacheNameString, const std::string& i_format);
	void ExtractDataSet(const std::string& i_dataCacheFileName, DataSet& o_dataSet);
};


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
