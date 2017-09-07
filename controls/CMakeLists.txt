INCLUDE (${CMAKE_CURRENT_LIST_DIR}/globe/CMakeLists.txt)
INCLUDE (${CMAKE_CURRENT_LIST_DIR}/altitudemap/CMakeLists.txt)

SET(CONTROLS_SOURCE_FILES
        ${GLOBE_SOURCE_FILES}
        ${ALTITUDEMAP_SOURCE_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/QExpander.cpp
        ${CMAKE_CURRENT_LIST_DIR}/QExpander.h
        #  ${CMAKE_CURRENT_LIST_DIR}/globe/QNoiseMapExplorer.cpp
      #  ${CMAKE_CURRENT_LIST_DIR}/globe/QNoiseMapExplorer.h
       # ${CMAKE_CURRENT_LIST_DIR}/globe/CalenhadGlobeDialog.cpp
       # ${CMAKE_CURRENT_LIST_DIR}/globe/CalenhadGlobeDialog.h
        ${CMAKE_CURRENT_LIST_DIR}/QxtGlobal.h
        ${CMAKE_CURRENT_LIST_DIR}/QIconPalette.cpp
        ${CMAKE_CURRENT_LIST_DIR}/QIconPalette.h
        ${CMAKE_CURRENT_LIST_DIR}/SizeGripItem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SizeGripItem.h
        ${CMAKE_CURRENT_LIST_DIR}/FlowLayout.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FlowLayout.h
        ${CMAKE_CURRENT_LIST_DIR}/QColoredIcon.h
        ${CMAKE_CURRENT_LIST_DIR}/QColoredIcon.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TextEdit.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TextEdit.h
        )
