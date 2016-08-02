# Installation/deployment
# copy to some $HOUDINI_PATH/dso location and it will be picked up
# You will see in File SOP and additional entry for *.voxel files

HDK_ADD_LIBRARY ( MCX SHARED
  MCX_IOTranslator.cpp
  )

TARGET_LINK_LIBRARIES ( MCX
  ${NCACHE_REQUIRED_LIBRARIES}
  NCacheTranslatorUtils
  )
