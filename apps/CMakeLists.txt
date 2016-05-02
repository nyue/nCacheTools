ADD_EXECUTABLE ( nCacheInfo
  nCacheInfo_main.cpp
  )

ADD_EXECUTABLE ( binary_files_tutorial
  binary_files_tutorial_main.cpp
  )

ADD_EXECUTABLE ( mccinfo
  mccinfo_main.cpp
  )

ADD_EXECUTABLE ( mcxinfo
  mcxinfo_main.cpp
  )

ADD_EXECUTABLE ( xmlinfo
  xmlinfo_main.cpp
  )

# MESSAGE ( "Boost_LIBRARIES = ${Boost_LIBRARIES}" )

SET  ( NCACHE_REQUIRED_LIBRARIES
  nCacheParser
  ${XERCESC_xercesc_LIBRARY}
  ${Boost_LIBRARIES}
  ${GLOG_glog_LIBRARY}
  ${ADDITIONAL_LIBRARIES}
  )

TARGET_LINK_LIBRARIES ( nCacheInfo
  ${NCACHE_REQUIRED_LIBRARIES}
  )

TARGET_LINK_LIBRARIES ( binary_files_tutorial
  ${NCACHE_REQUIRED_LIBRARIES}
  )

TARGET_LINK_LIBRARIES ( mccinfo
  ${NCACHE_REQUIRED_LIBRARIES}
  )

TARGET_LINK_LIBRARIES ( xmlinfo
  ${NCACHE_REQUIRED_LIBRARIES}
  )
