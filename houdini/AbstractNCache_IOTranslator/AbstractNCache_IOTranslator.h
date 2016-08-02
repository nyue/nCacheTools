#pragma once

#include <map>
#include <string>

class AbstractNCache_IOTranslator
{
protected:
    typedef std::map<std::string,std::string> NCacheChannelNamePostFixToHoudiniAttributeNameMap;
public:
    AbstractNCache_IOTranslator() {}
    virtual ~AbstractNCache_IOTranslator() {}
    static std::string postfixMatched(const std::string& i_channelName);
private:
    static NCacheChannelNamePostFixToHoudiniAttributeNameMap _ncnpf2han_map;
    static NCacheChannelNamePostFixToHoudiniAttributeNameMap initializeChannelAttributeMap();
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
