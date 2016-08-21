ADD_SUBDIRECTORY ( procedural )
ADD_SUBDIRECTORY ( translator )

INSTALL ( CODE
  "
  FILE (WRITE ${CMAKE_INSTALL_PREFIX}/arnold/nCacheToolsArnold.mod \"+ nCacheToolsArnold any ${CMAKE_INSTALL_PREFIX}/arnold\nMTOA_EXTENSIONS_PATH +:= extensions\n\")
  "
  COMPONENT Runtime
)
