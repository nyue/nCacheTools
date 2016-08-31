#pragma once

#include <UT/UT_BoundingBox.h>
#include <UT/UT_String.h>
#include <VRAY/VRAY_Procedural.h>
#include <AbstractMemoryReader.h>

/// @brief A procedural which does a deferred load of Maya nCache data from disk
class VRAY_nCache : public VRAY_Procedural {
protected:
	typedef boost::shared_ptr<nCache::AbstractMemoryReader> AbstractMemoryReaderSharedPtr;
	AbstractMemoryReaderSharedPtr _memory_reader_ptr;
    typedef UT_ValArray<UT_Vector3F> Vector3Container;
public:
	VRAY_nCache();
    virtual ~VRAY_nCache();

    virtual const char	*className() const;
    virtual int		 initialize(const UT_BoundingBox *);
    virtual void	 getBoundingBox(UT_BoundingBox &box);
    virtual void	 render();
protected:
    void addSamples(const Vector3Container& i_points,
    				VRAY_ProceduralGeo&     o_geo);

private:
    UT_BoundingBox	 myBox;
    UT_StringHolder	 myFile;
    Vector3Container _current_frame_points;
};
