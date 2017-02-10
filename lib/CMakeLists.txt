IF ( BUILD_ALEMBIC_TOOLS )
  SET ( NCACHE_ALEMBIC_SOURCE nCacheAlembic.cpp )
ENDIF ()

IF (NOT WIN32)
  ADD_DEFINITIONS ( -fPIC )
ENDIF ()

ADD_LIBRARY ( nCacheParser STATIC
  XMLReader.cpp
  nCacheLoader.cpp
  ${NCACHE_ALEMBIC_SOURCE}
  ChannelInfo.cpp
  AbstractFileReader.cpp
  AbstractMemoryReader.cpp
  MCXFileReader.cpp
  MCFileReader.cpp
  MCMemoryReader.cpp
  MCXMemoryReader.cpp
  )
