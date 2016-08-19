IF (WIN32)
	IF ( NOT CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS )
		MESSAGE ( FATAL_ERROR "Enable CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS otherwise, module will not be loadable in Maya" )
	ENDIF ()
ENDIF ()

ADD_LIBRARY ( nCacheTranslator SHARED
  nCacheTranslator.cpp
  )

TARGET_LINK_LIBRARIES ( nCacheTranslator
  ${MtoA_mtoa_api_LIBRARY}
)

IF (APPLE)
  SET_TARGET_PROPERTIES ( nCacheTranslator
    PROPERTIES
    LINK_FLAGS "-undefined dynamic_lookup"
    PREFIX ""
    )
ELSE (APPLE)
  SET_TARGET_PROPERTIES ( nCacheTranslator
    PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    )
ENDIF (APPLE)

INSTALL ( TARGETS
  nCacheTranslator
  DESTINATION
  arnold/extensions
)
