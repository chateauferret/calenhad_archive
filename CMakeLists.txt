cmake_minimum_required(VERSION 3.6)
project(calenhad)
include(CMakeFindDependencyMacro)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_AUTOMOC ON)
set(OSG_DIR "/usr/local/lib64")
set(SOURCE_FILES
        main.cpp
        preferences.h
        preferences.cpp
        geoutils.h
        geoutils.cpp
        nodeedit/qneblock.h
        nodeedit/qneblock.cpp
        nodeedit/qneconnection.h
        nodeedit/qneconnection.cpp
        nodeedit/qnemainwindow.h
        nodeedit/qnemainwindow.cpp
        nodeedit/qneport.h
        nodeedit/qneport.cpp
        nodeedit/CalenhadController.h
        nodeedit/CalenhadController.cpp
        nodeedit/ui_qnemainwindow.h
        nodeedit/qneblockhandle.cpp
        nodeedit/qneblockhandle.h
        messagefactory.cpp
        messagefactory.h
        nodeedit/qnemessagebox.cpp
        nodeedit/qnemessagebox.h
        nodeedit/CalenhadView.cpp
        nodeedit/CalenhadView.h
        nodeedit/componentproxywidget.cpp
        nodeedit/componentproxywidget.h
        nodeedit/qnetoolbox.cpp
        nodeedit/qnetoolbox.h
        pipeline/ModuleFactory.cpp
        pipeline/ModuleFactory.h
        qmodule/QModule.cpp
        qmodule/QModule.h
        controls/QExpander.cpp
        controls/QExpander.h
        qmodule/QNoiseModule.h
        qmodule/QNoiseModule.cpp
        qmodule/QSpheresModule.cpp
        qmodule/QSpheresModule.h
        qmodule/QClampModule.cpp
        qmodule/QClampModule.h
        qmodule/QConstModule.cpp
        qmodule/QConstModule.h
        qmodule/QCylindersModule.h
        qmodule/QCylindersModule.cpp
        qmodule/QExponentModule.h
        qmodule/QExponentModule.cpp
        qmodule/QRotateModule.h
        qmodule/QRotateModule.cpp
        qmodule/QTranslateModule.h
        qmodule/QTranslateModule.cpp
        qmodule/QScaleModule.h
        qmodule/QScaleModule.cpp
        qmodule/QScaleBiasModule.h
        qmodule/QScaleBiasModule.cpp
        qmodule/QSelectModule.h
        qmodule/QSelectModule.cpp
        qmodule/QTurbulenceModule.h
        qmodule/QTurbulenceModule.cpp
        qmodule/QVoronoiModule.h
        qmodule/QVoronoiModule.cpp
        qmodule/QBasicModule.cpp
        qmodule/QBasicModule.h
        controls/QNoiseMapViewer.cpp
        controls/QNoiseMapViewer.h
        qmodule/QNode.h
        qmodule/QNode.cpp
        qmodule/QRangeModule.h
        qmodule/QRangeModule.cpp
        controls/QLogSpinBox.cpp
        controls/QLogSpinBox.h
        pipeline/RenderJob.cpp
        pipeline/RenderJob.h
        controls/QNoiseMapExplorer.cpp
        controls/QNoiseMapExplorer.h
        httpserver/CalenhadRequestHandler.cpp
        httpserver/CalenhadRequestHandler.h
        pipeline/CalenhadModel.cpp
        pipeline/CalenhadModel.h
        pipeline/TileProducer.cpp
        pipeline/TileProducer.h
        controls/CalenhadMarbleWidget.cpp
        controls/CalenhadMarbleWidget.h
        actions/ZoomCommand.cpp
        actions/ZoomCommand.h
        actions/DeleteConnectionCommand.cpp
        actions/DeleteConnectionCommand.h
        actions/DeleteModuleCommand.cpp
        actions/DeleteModuleCommand.h
        mapping/CalenhadLayer.cpp
        mapping/CalenhadLayer.h
        libnoiseutils/NoiseMap.cpp
        libnoiseutils/NoiseMap.h
        libnoiseutils/NoiseMapBuilder.cpp
        libnoiseutils/NoiseMapBuilder.h
        libnoiseutils/NoiseMapBuilderCylinder.cpp
        libnoiseutils/NoiseMapBuilderCylinder.h
        libnoiseutils/NoiseMapBuilderPlane.cpp
        libnoiseutils/NoiseMapBuilderPlane.h
        libnoiseutils/NoiseMapBuilderSphere.cpp
        libnoiseutils/NoiseMapBuilderSphere.h
        libnoiseutils/RendererImage.cpp
        libnoiseutils/RendererImage.h
        libnoiseutils/RendererNormalMap.cpp
        libnoiseutils/RendererNormalMap.h
        libnoiseutils/Interpolation.h
        libnoiseutils/LittleEndian.h
        libnoiseutils/diff.h
        libnoiseutils/diff.cpp
        libnoiseutils/nullmodule.h
        libnoiseutils/nullmodule.cpp
        libnoiseutils/NoiseContstants.h
        libnoiseutils/legend.h
        libnoiseutils/legend.cpp
        libnoiseutils/intervallegend.h
        libnoiseutils/intervallegend.cpp
        libnoiseutils/GradientLegend.cpp
        libnoiseutils/GradientLegend.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
add_executable(calenhad ${SOURCE_FILES})

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL)
find_package(LibProj4 REQUIRED)
find_package(LibNoise REQUIRED)
#find_package (OpenThreads REQUIRED)
find_package(GeographicLib 1.34 REQUIRED)
#find_package (OpenSceneGraph COMPONENTS osg osgUtil osgGA osgDB osgTerrain osgViewer osgText osgWidget osgManipulator osgShadow REQUIRED)
#find_package (OsgEarth REQUIRED)
find_package(QtWebApp REQUIRED COMPONENTS HttpServer)
find_package(Marble REQUIRED)

add_definitions(${GeographicLib_DEFINITIONS})
qt5_use_modules(calenhad Gui Widgets OpenGL)

#set (OSG_LIBRARIES
#    libosg.so libosgUtil.so  libosgGA.so libosgDB.so libosgUI.so libosgTerrain.so libosgViewer.so libosgText.so libosgWidget.so libosgManipulator.so libosgShadow.so)

#set (OSGEARTH_LIBRARIES libosgEarthd.so
#    libosgEarthUtild.so libosgEarthQtd.so libosgEarthUtild.so libosgEarthAnnotationd.so libosgEarthFeaturesd.so libosgEarthSymbologyd.so )

include_directories(
        ${LIBNOISE_LIBRARIES_INCLUDE_DIRS}
        #${OPENTHREADS_INCLUDE_DIRS}
        ${GeographicLib_INCLUDE_DIRS}
        ${QtWebApp_INCLUDE_DIRS}
        #${OPENSCENEGRAPH_INCLUDE_DIR} ${OSGEARTH_INCLUDE_DIRS}
)
target_link_libraries(calenhad Marble
        #${OSG_LIBRARIES}
        #osgEarthQt
        #${OPENTHREADS_LIBRARIES} ${OSGEARTH_LIBRARIES}
        ${LIBNOISE_LIBRARIES} ${GeographicLib_LIBRARIES} ${QtWebApp_LIBRARIES})