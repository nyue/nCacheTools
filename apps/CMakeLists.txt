IF ( BUILD_XML_TOOLS )
  ADD_EXECUTABLE ( nCacheInfo
	nCacheInfo_main.cpp
	)
ENDIF ()

ADD_EXECUTABLE ( binary_files_tutorial
  binary_files_tutorial_main.cpp
  )

ADD_EXECUTABLE ( mccinfo
  mccinfo_main.cpp
  )

ADD_EXECUTABLE ( mcxinfo
  mcxinfo_main.cpp
  )

IF ( BUILD_XML_TOOLS )
  ADD_EXECUTABLE ( xmlinfo
	xmlinfo_main.cpp
	)
ENDIF ()

ADD_EXECUTABLE ( mcc2csv
  mcc2csv_main.cpp
  )

ADD_EXECUTABLE ( mcx2csv
  mcx2csv_main.cpp
  )

IF ( BUILD_ALEMBIC_TOOLS )
  ADD_EXECUTABLE ( nCache2alembic
	nCache2alembic_main.cpp
	)
ENDIF ()

# MESSAGE ( "Boost_LIBRARIES = ${Boost_LIBRARIES}" )

IF ( BUILD_XML_TOOLS )
  TARGET_LINK_LIBRARIES ( nCacheInfo
	${NCACHE_REQUIRED_LIBRARIES}
	)
ENDIF ()

TARGET_LINK_LIBRARIES ( binary_files_tutorial
  ${NCACHE_REQUIRED_LIBRARIES}
  )

TARGET_LINK_LIBRARIES ( mccinfo
  ${NCACHE_REQUIRED_LIBRARIES}
  )

TARGET_LINK_LIBRARIES ( mcxinfo
  ${NCACHE_REQUIRED_LIBRARIES}
  )

IF ( BUILD_XML_TOOLS )
  TARGET_LINK_LIBRARIES ( xmlinfo
	${NCACHE_REQUIRED_LIBRARIES}
	)
ENDIF ()

TARGET_LINK_LIBRARIES ( mcc2csv
  ${NCACHE_REQUIRED_LIBRARIES}
  )

TARGET_LINK_LIBRARIES ( mcx2csv
  ${NCACHE_REQUIRED_LIBRARIES}
  )

IF ( BUILD_ALEMBIC_TOOLS )
  SET  ( ALEMBIC_REQUIRED_LIBRARIES 
	${Alembic_Alembic_LIBRARY}
	${Ilmbase_HALF_LIBRARY}
	${Ilmbase_IEX_LIBRARY}
	${HDF5_hdf5_LIBRARY}
	${HDF5_hdf5_hl_LIBRARY}
	${HDF5_z_LIBRARY}
	)
  
  TARGET_LINK_LIBRARIES ( nCache2alembic
	${NCACHE_REQUIRED_LIBRARIES}
	${ALEMBIC_REQUIRED_LIBRARIES} 
	)
ENDIF ()
