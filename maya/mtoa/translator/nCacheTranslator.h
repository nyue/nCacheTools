#pragma once

// #include <translators/DagTranslator.h>
#include <translators/shape/ShapeTranslator.h>
#include <ai.h>

// class nCacheTranslator : public CDagTranslator
class nCacheTranslator : public CShapeTranslator
{
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
