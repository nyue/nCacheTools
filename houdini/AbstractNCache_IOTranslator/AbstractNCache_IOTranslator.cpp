#include "AbstractNCache_IOTranslator.h"
#include <boost/algorithm/string/predicate.hpp>

AbstractNCache_IOTranslator::NCacheChannelNamePostFixToHoudiniAttributeNameMap
AbstractNCache_IOTranslator::initializeChannelAttributeMap()
{
	//	Channel['nParticleShape1_id']  = id
	//	Channel['nParticleShape1_position'] =  P
	//	Channel['nParticleShape1_velocity']      = v
	//	Channel['nParticleShape1_age']  = age
	//
	//	Channel['nParticleShape1_acceleration']  = acceleration
	//	Channel['nParticleShape1_ageNormalized']  = ageN
	//	Channel['nParticleShape1_birthTime']  = birthtime
	//	Channel['nParticleShape1_finalLifespanPP'] = age2
	//	Channel['nParticleShape1_lifespanPP'] =  life
	//	Channel['nParticleShape1_mass']  = mass
	//	Channel['nParticleShape1_opacityPP']  =    Alpha
	//	Channel['nParticleShape1_radiusPP']     = pscale
	//	Channel['nParticleShape1_rgbPP']   = Cd
	//	Channel['nParticleShape1_spriteScaleXPP']  = spriteX
	//	Channel['nParticleShape1_spriteScaleYPP']  = spriteY
	//	Channel['nParticleShape1_spriteTwistPP'] = twist
	//	Channel['nParticleShape1_worldPosition']  = P2
	//	Channel['nParticleShape1_worldVelocity']   = v2
	//
	//	Channel['nParticleShape1_count']  =      ---> not needed
	//	Channel['nParticleShape1_worldVelocityInObjectSpace'] type   ---> not needed
	//

	NCacheChannelNamePostFixToHoudiniAttributeNameMap caMap;
	// Minimal required to be useful in production
	caMap.insert(std::pair<std::string,std::string>("_id","id"));
	caMap.insert(std::pair<std::string,std::string>("_position","P"));
	caMap.insert(std::pair<std::string,std::string>("_velocity","v"));
	caMap.insert(std::pair<std::string,std::string>("_age","age"));
	// optional
	caMap.insert(std::pair<std::string,std::string>("_acceleration","acceleration"));
	caMap.insert(std::pair<std::string,std::string>("_ageNormalized","ageN"));
	caMap.insert(std::pair<std::string,std::string>("_birthTime","birthtime"));
	caMap.insert(std::pair<std::string,std::string>("_finalLifespanPP","age2"));
	caMap.insert(std::pair<std::string,std::string>("_lifespanPP","life"));
	caMap.insert(std::pair<std::string,std::string>("_mass","mass"));
	caMap.insert(std::pair<std::string,std::string>("_opacityPP","Alpha"));
	caMap.insert(std::pair<std::string,std::string>("_radiusPP","pscale"));
	caMap.insert(std::pair<std::string,std::string>("_rgbPP","Cd"));
	caMap.insert(std::pair<std::string,std::string>("_spriteScaleXPP","spriteX"));
	caMap.insert(std::pair<std::string,std::string>("_spriteScaleYPP","spriteY"));
	caMap.insert(std::pair<std::string,std::string>("_spriteTwistPP","twist"));
	caMap.insert(std::pair<std::string,std::string>("_worldPosition","P2"));
	caMap.insert(std::pair<std::string,std::string>("_worldVelocity","v2"));

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
