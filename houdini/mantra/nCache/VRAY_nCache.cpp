

// #include <UT/UT_DSOVersion.h>
#include <GU/GU_Detail.h>
#include "VRAY_nCache.h"
#include <MCXMemoryReader.h>
#include <MCMemoryReader.h>

static VRAY_ProceduralArg	theArgs[] = {
    VRAY_ProceduralArg("file",		"string",	""),
    VRAY_ProceduralArg()
};

VRAY_Procedural *
allocProcedural(const char *)
{
    return new VRAY_nCache();
}

const VRAY_ProceduralArg *
getProceduralArgs(const char *)
{
    return theArgs;
}

VRAY_nCache::VRAY_nCache()
{
    myBox.initBounds(0, 0, 0);
}

VRAY_nCache::~VRAY_nCache()
{
}

const char *
VRAY_nCache::className() const
{
    return "VRAY_nCache";
}

int
VRAY_nCache::initialize(const UT_BoundingBox *box)
{
    // Import the filenames
    import("file", myFile);
    fprintf(stderr,"VRAY_nCache file = %s\n",myFile.c_str());

    if (myFile.endsWith(".mcx"))
    	_memory_reader_ptr.reset(new nCache::MCXMemoryReader(myFile.c_str()));
    else if (myFile.endsWith(".mc"))
    	_memory_reader_ptr.reset(new nCache::MCMemoryReader(myFile.c_str()));
    else
        return 0;

    // The user is required to specify the bounds of the geometry.
    // Alternatively, if the bounds aren't specified, we could forcibly load
    // the geometry at this point and compute the bounds ourselves.
    if (!box)
    {
    	const nCache::ChannelDataContainer& channels_data = _memory_reader_ptr->get_channels_data();
    	if (!channels_data.empty())
    	{
    		nCache::ChannelDataContainer::const_iterator iter;
    		nCache::ChannelDataContainer::const_iterator eIter = channels_data.end();
    		for (iter = channels_data.begin();iter!=eIter;++iter)
    		{
    		    UT_StringHolder channel_name(iter->first.c_str());
    			if (channel_name.endsWith("_position"))
    			{
    				size_t num_points = iter->second._fvca.size();
    				for (size_t ptIndex=0;ptIndex<num_points;ptIndex++)
    				{
        				UT_Vector3F P(iter->second._fvca[ptIndex].x,iter->second._fvca[ptIndex].y,iter->second._fvca[ptIndex].z);
        				_current_frame_points.append(P);
        				myBox.enlargeBounds(P);
    				}
        		    break; // Found, no need to continue
    			}
    		}
    	}
    	else
    		return 0;
    }
    else
    	// Stash away the box the user specified
    	myBox = *box;

    fpreal	fps = 24.0, shutter[2] = {0};
    import("global:fps", &fps, 1);
    import("camera:shutter", shutter, 2);

    return 1;
}

void
VRAY_nCache::getBoundingBox(UT_BoundingBox &box)
{
    box = myBox;
}

void VRAY_nCache::addSamples(const Vector3Container& i_points,
							 VRAY_ProceduralGeo&     o_geo)
{
    GA_RWAttributeRef id_attrib = o_geo->addIntTuple(GA_ATTRIB_POINT, "id", 1);
    if (!id_attrib.isValid())
    {
    	std::cerr << "Failed to add point id attribute" << std::endl;
    	return;
    }
	GA_Offset ptoff = o_geo->appendPointBlock(i_points.entries());

	UT_ValArray<int32> point_id;
	for (GA_Size i=0;i<i_points.entries();i++)
	{
		o_geo->setPos3(ptoff, i_points.getRawArray()[i]);
		point_id.append(i);
		ptoff++;
	}

	o_geo->setAttributeFromArray(id_attrib,o_geo->getPointRange(), point_id);
}
void
VRAY_nCache::render()
{
    // Allocate geometry.
    // Warning:  When allocating geometry for a procedural, do not simply
    // construct a GU_Detail, but call VRAY_Procedural::createGeometry().
    VRAY_ProceduralGeo	g0 = createGeometry();

    addSamples(this->_current_frame_points,g0);

    VRAY_ProceduralChildPtr	obj = createChild();
    obj->addGeometry(g0);
}
