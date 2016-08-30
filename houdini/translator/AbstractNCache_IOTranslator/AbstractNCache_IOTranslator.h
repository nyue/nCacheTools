#pragma once

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include "AbstractMemoryReader.h"

class AbstractNCache_IOTranslator
{
protected:
    typedef std::map<std::string,std::string> NCacheChannelNamePostFixToHoudiniAttributeNameMap;
	typedef boost::shared_ptr<nCache::AbstractMemoryReader> AbstractMemoryReaderSharedPtr;
	AbstractMemoryReaderSharedPtr _memory_reader_ptr;
	std::string _file_extension;
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
