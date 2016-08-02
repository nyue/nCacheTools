#include "AbstractNCache_IOTranslator.h"
#include <boost/algorithm/string/predicate.hpp>

AbstractNCache_IOTranslator::NCacheChannelNamePostFixToHoudiniAttributeNameMap
AbstractNCache_IOTranslator::initializeChannelAttributeMap()
{
	NCacheChannelNamePostFixToHoudiniAttributeNameMap caMap;
	// Minimal required to be useful in production
	caMap.insert(std::pair<std::string,std::string>("_id","id"));
	caMap.insert(std::pair<std::string,std::string>("_position","P"));
	caMap.insert(std::pair<std::string,std::string>("_velocity","v"));
	caMap.insert(std::pair<std::string,std::string>("_finalLifespanPP","age"));
	// optional
	caMap.insert(std::pair<std::string,std::string>("_birthTime","birth"));
	caMap.insert(std::pair<std::string,std::string>("_lifespanPP","life"));

	return caMap;
}

AbstractNCache_IOTranslator::NCacheChannelNamePostFixToHoudiniAttributeNameMap AbstractNCache_IOTranslator::_ncnpf2han_map = AbstractNCache_IOTranslator::initializeChannelAttributeMap();

std::string AbstractNCache_IOTranslator::postfixMatched(const std::string& i_channelName)
{

	NCacheChannelNamePostFixToHoudiniAttributeNameMap::const_iterator iter = _ncnpf2han_map.begin();
	NCacheChannelNamePostFixToHoudiniAttributeNameMap::const_iterator eIter = _ncnpf2han_map.end();
	for (;iter!=eIter;++iter)
	{
		if (boost::algorithm::ends_with(i_channelName, iter->first))
		{
			return iter->second;
		}
	}
	return "";
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
