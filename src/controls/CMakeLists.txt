INCLUDE (${CMAKE_CURRENT_LIST_DIR}/globe/CMakeLists.txt)
INCLUDE (${CMAKE_CURRENT_LIST_DIR}/altitudemap/CMakeLists.txt)
INCLUDE (${CMAKE_CURRENT_LIST_DIR}/legend/CMakeLists.txt)
SET(CONTROLS_SOURCE_FILES
        ${GLOBE_SOURCE_FILES}
        ${ALTITUDEMAP_SOURCE_FILES}
        ${LEGEND_EDITOR_SOURCE_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/QExpander.cpp
        ${CMAKE_CURRENT_LIST_DIR}/QExpander.h
        ${CMAKE_CURRENT_LIST_DIR}/QxtGlobal.h
        ${CMAKE_CURRENT_LIST_DIR}/QIconPalette.cpp
        ${CMAKE_CURRENT_LIST_DIR}/QIconPalette.h
        ${CMAKE_CURRENT_LIST_DIR}/SizeGripItem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SizeGripItem.h
        ${CMAKE_CURRENT_LIST_DIR}/FlowLayout.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FlowLayout.h
        ${CMAKE_CURRENT_LIST_DIR}/TextEdit.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TextEdit.h
        ${CMAKE_CURRENT_LIST_DIR}/ColorButton.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ColorButton.h
        ${CMAKE_CURRENT_LIST_DIR}/CalenhadLegendDialog.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CalenhadLegendDialog.h
        ${CMAKE_CURRENT_LIST_DIR}/QAngleControl.h
        ${CMAKE_CURRENT_LIST_DIR}/QAngleControl.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SplashDialog.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SplashDialog.h
        ${CMAKE_CURRENT_LIST_DIR}/ModuleTree.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ModuleTree.h
        ${CMAKE_CURRENT_LIST_DIR}/CubeRasterWidget.h
        ${CMAKE_CURRENT_LIST_DIR}/CubeRasterWidget.cpp

        )
