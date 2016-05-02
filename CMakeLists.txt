CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )

PROJECT ( nCacheTools )

FIND_PACKAGE ( XercesC REQUIRED )
FIND_PACKAGE ( Boost 1.51 REQUIRED COMPONENTS system filesystem )
FIND_PACKAGE ( GoogleLog REQUIRED )

INCLUDE_DIRECTORIES ( ${XERCESC_INCLUDE_DIR} )
INCLUDE_DIRECTORIES ( ${Boost_INCLUDE_DIR} )
INCLUDE_DIRECTORIES ( ${GLOG_INCLUDE_DIR} )

IF (WIN32)
  ADD_DEFINITIONS ( -DGOOGLE_GLOG_DLL_DECL= )
  SET ( ADDITIONAL_LIBRARIES Ws2_32 )
ENDIF ()

ADD_LIBRARY ( nCacheParser STATIC
  nCacheLoader.cpp
  AbstractFileReader.cpp
  AbstractMemoryReader.cpp
  MCXFileReader.cpp
  MCFileReader.cpp
  MCMemoryReader.cpp
  MCXMemoryReader.cpp
  XMLReader.cpp
  )

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

