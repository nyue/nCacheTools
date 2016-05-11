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

ADD_EXECUTABLE ( mcc2csv
  mcc2csv_main.cpp
  )

ADD_EXECUTABLE ( nCache2alembic
  nCache2alembic_main.cpp
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

TARGET_LINK_LIBRARIES ( mcc2csv
  ${NCACHE_REQUIRED_LIBRARIES}
  )

SET  ( ALEMBIC_REQUIRED_LIBRARIES 
  ${Alembic_Alembic_LIBRARY}
 )

TARGET_LINK_LIBRARIES ( nCache2alembic
  ${NCACHE_REQUIRED_LIBRARIES}
  ${ALEMBIC_REQUIRED_LIBRARIES} 
  )

