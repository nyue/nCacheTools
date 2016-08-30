#pragma once

#include <AbstractNCache_IOTranslator.h>
#include <GU/GU_Detail.h>
#include <GU/GU_PrimVolume.h>
#include <GEO/GEO_AttributeHandle.h>
#include <GEO/GEO_IOTranslator.h>
#include <SOP/SOP_Node.h>
#include <UT/UT_Assert.h>
#include <UT/UT_IOTable.h>
#include <UT/UT_String.h>
#include <iostream>
#include <stdio.h>

class nCache_IOTranslator : public AbstractNCache_IOTranslator, public GEO_IOTranslator
{
public:
	nCache_IOTranslator() {}
	nCache_IOTranslator(const nCache_IOTranslator &src)
	{
		// Need explicit copy assignment as we are performing multiple inheritance ?
		_file_extension = src._file_extension;
	}
    virtual ~nCache_IOTranslator() {}
    virtual nCache_IOTranslator    *duplicate() const;
    virtual const char *formatName() const;
    virtual int         checkExtension(const char *name);
    virtual int         checkMagicNumber(unsigned magic);
    virtual GA_Detail::IOStatus  fileLoad(GEO_Detail *, UT_IStream &,
                                          bool ate_magic);
    virtual GA_Detail::IOStatus  fileSave(const GEO_Detail *, std::ostream &);
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
