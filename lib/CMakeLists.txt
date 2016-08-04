IF ( BUILD_XML_TOOLS )
  ADD_LIBRARY ( nCacheXMLParser STATIC
    XMLReader.cpp
    nCacheLoader.cpp
    nCacheAlembic.cpp
	)
ENDIF ()

ADD_LIBRARY ( nCacheParser STATIC
  ChannelInfo.cpp
  AbstractFileReader.cpp
  AbstractMemoryReader.cpp
  MCXFileReader.cpp
  MCFileReader.cpp
  MCMemoryReader.cpp
  MCXMemoryReader.cpp
  )
