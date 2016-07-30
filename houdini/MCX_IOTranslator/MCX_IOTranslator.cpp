#include "MCX_IOTranslator.h"

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

GA_Detail::IOStatus  MCX_IOTranslator::fileLoad(GEO_Detail *, UT_IStream &is,
                                      bool ate_magic)
{
    UT_IStreamAutoBinary        forceascii(is, false);

    // All done successfully
    return GA_Detail::IOStatus(true);
}

GA_Detail::IOStatus  MCX_IOTranslator::fileSave(const GEO_Detail *, std::ostream &)
{
    return GA_Detail::IOStatus(false);
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
