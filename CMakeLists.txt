cmake_minimum_required(VERSION 3.6)
project (calenhad)
include (CMakeFindDependencyMacro)

include_directories(
        ${LIBNOISE_LIBRARIES_INCLUDE_DIRS}
        ${GeographicLib_INCLUDE_DIRS}
        ${QtWebApp_INCLUDE_DIRS}
)

add_subdirectory (test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_AUTOMOC ON)
set(SOURCE_FILES
        preferences.h
        preferences.cpp
        geoutils.h
        geoutils.cpp
        nodeedit/qneblock.h
        nodeedit/qneblock.cpp
        nodeedit/qneconnection.h
        nodeedit/qneconnection.cpp
        nodeedit/Calenhad.h
        nodeedit/Calenhad.cpp
        nodeedit/qneport.h
        nodeedit/qneport.cpp
        nodeedit/CalenhadController.h
        nodeedit/CalenhadController.cpp
        nodeedit/ui_qnemainwindow.h
        messagefactory.cpp
        messagefactory.h
        LegendService.h
        LegendRoster.h
        LegendRoster.cpp
        nodeedit/qnemessagebox.cpp
        nodeedit/qnemessagebox.h
        nodeedit/CalenhadView.cpp
        nodeedit/CalenhadView.h
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
        actions/AddModuleCommand.cpp
        actions/AddModuleCommand.h
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
        icosphere/legend.h
        icosphere/legend.cpp
        libnoiseutils/intervallegend.h
        libnoiseutils/intervallegend.cpp
        libnoiseutils/GradientLegend.cpp
        libnoiseutils/GradientLegend.h
        libnoiseutils/IcosphereBuilder.h
        libnoiseutils/IcosphereBuilder.cpp
        icosphere/vertex.h
        icosphere/vertex.cpp
        icosphere/triangle.h
        icosphere/triangle.cpp
        icosphere/icosphere.h
        icosphere/icosphere.cpp
        icosphere/icosphereutils.h
        qmodule/QIcosphereMap.cpp
        qmodule/QIcosphereMap.h
        qmodule/QAltitudeMap.cpp
        qmodule/QAltitudeMap.h
        controls/AltitudeMapPlot.cpp
        controls/AltitudeMapPlot.h
        libnoiseutils/icospheremap.cpp
        libnoiseutils/icospheremap.h
        icosphere/Bounds.cpp
        icosphere/Bounds.h
        controls/AltitudeMapEditor.cpp
        controls/AltitudeMapEditor.h
        controls/AltitudeMapPlot.cpp
        controls/AltitudeMapPlot.h
        controls/AltitudeMapFitter.cpp
        controls/AltitudeMapFitter.h
        controls/TerraceMapFitter.cpp
        controls/TerraceMapFitter.h
        CalenhadServices.cpp
        CalenhadServices.h
        PreferencesService.h
        MessageService.h
        LegendService.h
        actions/ChangeModuleCommand.cpp
        actions/ChangeModuleCommand.h
        nodeedit/EditableLabel.cpp
        nodeedit/EditableLabel.h
        actions/XmlCommand.cpp
        actions/XmlCommand.h
        controls/QxtGlobal.h
        controls/AngleDial.cpp
        controls/AngleDial.h
        actions/CommandGroup.cpp
        actions/CommandGroup.h
        actions/DuplicateModuleCommand.cpp
        actions/DuplicateModuleCommand.h
        actions/DeleteModuleCommand.cpp
        actions/DeleteModuleCommand.h
        controls/CalenhadOverviewMap.cpp
        controls/CalenhadOverviewMap.h
        PreferencesService.h
        MessageService.h
        actions/DuplicateModuleCommand.cpp
        actions/DuplicateModuleCommand.h
        controls/CalenhadPreview.h
        controls/CalenhadPreview.cpp
        )


set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qwt REQUIRED)
find_package(Qt5OpenGL)
find_package(LibProj4 REQUIRED)
find_package(LibNoise REQUIRED)
find_package(GeographicLib 1.34 REQUIRED)
find_package(QtWebApp REQUIRED COMPONENTS HttpServer)
find_package(Marble REQUIRED)

add_library (core ${SOURCE_FILES})
add_executable (calenhad main.cpp ${SOURCE_FILES} controls/CalenhadOverviewInterface.h)
add_definitions (${GeographicLib_DEFINITIONS})

target_link_libraries (core Marble ${LIBNOISE_LIBRARIES} ${GeographicLib_LIBRARIES} ${QtWebApp_LIBRARIES} ${PROJ4_LIBRARY} ${QWT_LIBRARY})
target_link_libraries (calenhad core)