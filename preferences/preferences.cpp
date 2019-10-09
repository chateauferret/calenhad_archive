//
// Created by martin on 16/10/16.
//

#include "preferences.h"
#include <iostream>
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
#include <utility>

using namespace calenhad::preferences;

QString Preferences::_styleSheet;

Preferences::Preferences() {
    _settings = new QSettings();
    std::cout << "Preferences file: " <<  _settings -> fileName ().toStdString () << "\n";
}

Preferences::~Preferences () {
    delete _settings;
}

void Preferences::setStylesheet (QString styleSheet) {
    _styleSheet = std::move (styleSheet);
}

QString Preferences::styleSheet() {
    return _styleSheet;
}

void Preferences::loadSettings() {
    bool ok;

    // Computation parameters
    calenhad_default_planet_radius = _settings -> value ("calenhad/default_planet/radius", 6371000).toDouble (&ok);
    calenhad_colormap_buffersize = _settings -> value ("calenhad/colormap/buffersize", 2048).toUInt();
    calenhad_altitudemap_buffersize = _settings -> value ("calenhad/altitudemap/buffersize", 2048).toUInt();
    calenhad_icosphere_depth = _settings -> value ("calenhad/icosphere/depth", 8).toUInt();

    // Styling for non-QGraphicsItem elements
    calenhad_stylesheet = _settings -> value ("calenhad/stylesheet", "/home/martin/.config/calenhad/darkorange.css").toString();

    // CalenhadView zooming
    calenhad_desktop_zoom_limit_zoomin = _settings -> value ("calenhad/desktop/zoom/limit/zoomin", 4).toDouble (&ok);
    calenhad_desktop_zoom_limit_zoomout = _settings -> value ("calenhad/desktop/zoom/limit/zoomin", 0.05).toDouble (&ok);
    calenhad_desktop_zoom_default = _settings -> value ("calenhad/desktop/zoom/default", 0.5).toDouble (&ok);
    calenhad_model_extent = _settings -> value ("calenhad/model/extent", 2000.0).toDouble (&ok);

    // CalenhadView grid
    calenhad_desktop_grid_major_color = _settings -> value ("calenhad/desktop/grid/major/color", "#404040").value<QColor>();
    calenhad_desktop_grid_major_style = _settings -> value ("calenhad/graticule/major/style", (int) Qt::PenStyle::SolidLine).toInt();
    calenhad_desktop_grid_major_weight = _settings -> value ("calenhad/desktop/grid/major/weights", 2).toUInt ();
    calenhad_desktop_grid_major_steps = _settings -> value ("calenhad/desktop/grid/minor/steps", 5).toUInt ();
    calenhad_desktop_grid_minor_color = _settings -> value ("calenhad/desktop/grid/minor/color", "#808080").value<QColor>();
    calenhad_desktop_grid_minor_style = _settings -> value ("calenhad/graticule/minor/style", (int) Qt::PenStyle::DashLine).toInt();
    calenhad_desktop_grid_minor_weight = _settings -> value ("calenhad/desktop/grid/minor/weights", 1).toUInt();
    calenhad_desktop_grid_density = _settings -> value ("calenhad/desktop/grid/density", 50).toUInt();
    calenhad_desktop_grid_visible = _settings -> value ("calenhad/desktop/grid/visible", true).toBool();
    calenhad_desktop_grid_snap = _settings -> value ("calenhad/desktop/grid/snao", true).toBool();

    // Modules in the CalenhadView
    calenhad_module_brush_color_selected = _settings -> value ("calenhad/module/brush/color/selected", "#00F0F0").value<QColor>();
    calenhad_module_brush_color_normal = _settings -> value ("calenhad/module/brush/color/normal", "#00C000").value<QColor>();
    calenhad_module_pen_color_selected = _settings -> value ("calenhad/module/pen/color/selected", "#007070").value<QColor>();
    calenhad_module_pen_color_normal = _settings -> value ("calenhad/module/pen/color/normal", "#00F000").value<QColor>();
    calenhad_module_text_color_selected = _settings -> value ("calenhad/module/text/color/selected", "#000000").value<QColor>();
    calenhad_module_text_color_normal = _settings -> value ("calenhad/module/text/color/normal", "#000000").value<QColor>();
    calenhad_module_text_color_error = _settings -> value ("calenhad/module/text/color/error", "#FF0000").value<QColor>();
    calenhad_module_duplicate_offset_x = _settings -> value ("calenhad/module/duplicate/offset/x", 64).toUInt();
    calenhad_module_duplicate_offset_y = _settings -> value ("calenhad/module/duplicate/offset/y", 64).toUInt();
    calenhad_handle_module_height = _settings -> value ("calenhad/handle/module/height", 48).toUInt();
    calenhad_handle_module_width = _settings -> value ("calenhad/handle/module/width", 96).toUInt();
    calenhad_handle_module_margin = _settings -> value ("calenhad/handle/module/margin", 2).toUInt();

    // Ports attached to modules
    calenhad_port_in_border_color = _settings -> value ("calenhad/port/in/border/color", "#000000").value<QColor>();
    calenhad_port_in_fill_color = _settings -> value ("calenhad/port/in/fill/color", "#FF0000").value<QColor>();
    calenhad_port_in_fill_color_drop = _settings -> value ("calenhad/port/in/fill/color/drop", "#8080FF").value<QColor>();
    calenhad_port_in_border_color_drop = _settings -> value ("calenhad/port/in/border/color/drop", "#0000FF").value<QColor>();
    calenhad_port_in_border_color_connected = _settings -> value ("calenhad/port/in/border/color/connected", "#0000F0").value<QColor>();
    calenhad_port_in_fill_color_connected = _settings -> value ("calenhad/port/in/fill/color/connected", "#00F000").value<QColor>();
    calenhad_port_out_border_color = _settings -> value ("calenhad/port/out/border/color", "#000000").value<QColor>();
    calenhad_port_out_fill_color = _settings -> value ("calenhad/port/out/fill/color", "#00F000").value<QColor>();
    calenhad_port_border_weight = _settings -> value ("calenhad/port/border/weight", 1).toUInt();
    calenhad_port_text_color = _settings -> value ("calenhad/port/text/color", "#000000").value<QColor>();
    calenhad_port_radius = _settings -> value ("calenhad/port/radius", 5).toUInt();
    calenhad_port_margin = _settings -> value ("calenhad/port/margin", 2).toUInt();
    calenhad_port_spacing = _settings -> value ("calenhad/port/spacing", 2).toUInt();

    // Connections between ports
    calenhad_connector_normal_color = _settings -> value ("calenhad/connector/normal/color", "#000080").value<QColor>();
    calenhad_connector_normal_weight = _settings -> value ("calenhad/connector/normal/weight", 3).toUInt();
    calenhad_connector_selected_color = _settings -> value ("calenhad/connector/selected/color", "#FF0000").value<QColor>();
    calenhad_connector_selected_weight = _settings -> value ("calenhad/connector/selected/weight", 3).toUInt();
    calenhad_connector_drawing_color = _settings -> value ("calenhad/connector/drawing/color", "#000080").value<QColor>();
    calenhad_connector_drawing_weight = _settings -> value ("calenhad/connector/drawing/weight", 1).toUInt();
    calenhad_connector_inactive_color = _settings -> value ("calenhad/connector/inactive/color", "808080").value<QColor>();
    calenhad_connector_inactive_weight = _settings -> value ("calenhad/connector/inactive_weight", 1).toUInt();

    // Text in the variables editor
    calenhad_variables_color_normal = _settings -> value ("calenhad/variables/color/normal", "#000000").value<QColor>();
    calenhad_variables_color_error = _settings -> value ("calenhad/variables/color/error", "#FF0000").value<QColor>();

    // Globe view elements

    // Globe view zooming
    calenhad_globe_zoom_min = _settings -> value ("calenhad/globe/zoom/min", 0.0).toDouble (&ok);
    calenhad_globe_zoom_max = _settings -> value ("calenhad/globe/zoom/max", 4.0).toDouble (&ok);

    // Dimensions
    calenhad_globe_inset_height = _settings -> value ("calenhad/globe/inset/height", 128).toUInt (&ok);
    calenhad_globe_texture_height = _settings -> value ("calenhad/globe/texture/height", 1024).toUInt (&ok);

    // Scale bar
    calenhad_globe_scale_background_color = _settings -> value ("calenhad/globe/scale/background/color", "#C0C0C0").value<QColor>();
    calenhad_globe_scale_width = _settings -> value ("calenhad/globe/scale/width", 200).toUInt();
    calenhad_globe_scale_height = _settings -> value ("calenhad/globe/scale/height", 30).toUInt();

    // Graticule
    calenhad_graticule_major_color  = _settings -> value ("calenhad/graticule/major/color", "#FFFFFF").value<QColor>();
    calenhad_graticule_major_weight = _settings -> value ("calenhad/graticule/major/weight", 1).toUInt();;
    calenhad_graticule_major_style = _settings -> value ("calenhad/graticule/major/style", (int) Qt::PenStyle::SolidLine).toInt();
    calenhad_graticule_minor_color = _settings -> value ("calenhad/graticule/minor/color", "#00FFFF").value<QColor>();
    calenhad_graticule_minor_weight = _settings -> value ("calenhad/graticule/minor/weight", 1).toUInt();
    calenhad_graticule_minor_style = _settings -> value ("calenhad/graticule/minor/style", (int) Qt::PenStyle::DashLine).toInt();
    calenhad_graticule_major_text_size = _settings -> value ("calenhad/graticule/major/text/size", 10).toUInt();
    calenhad_graticule_major_text_color = _settings -> value ("calenhad/graticule/major/text/color", "#FFFFFF").value<QColor>();
    calenhad_graticule_minor_text_size = _settings -> value ("calenhad/graticule/major/text/size", 8).toUInt();
    calenhad_graticule_minor_text_color = _settings -> value ("calenhad/graticule/major/text/color", "#00FFFF").value<QColor>();
    calenhad_graticule_visible = _settings -> value ("calenhad/graticule/visible", true).toBool();

    // Other UI elements

    // Icons in the toolpalette
    calenhad_toolpalette_icon_color_shadow = _settings -> value ("calenhad/toolpalette/icon/color/shadow", "#505050").value<QColor>();
    calenhad_toolpalette_icon_color_normal = _settings -> value ("calenhad/toolpalette/icon/color/normal", "#000000").value<QColor>();
    calenhad_toolpalette_icon_color_mouseover = _settings -> value ("calenhad/toolpalette/icon/color/mouseover", "#0000FF").value<QColor>();
    calenhad_toolpalette_icon_size = _settings -> value ("calenhad/toolpalette/icon/size", 24).toInt();
    calenhad_toolpalette_icon_margin = _settings -> value ("calenhad/toolpalette/icon/margin", 4).toInt();
    calenhad_toolpalette_icon_shadow = _settings -> value ("calenhad/toolpalette/icon/shadow", 2).toInt();
    calenhad_toolpalette_arrow_size = _settings -> value ("calenhad/toolpalette/arrow/size", 16).toInt();

    // Altitude map control
    calenhad_altitudemap_deletemargin = _settings -> value ("calenhad/altitudemap/deletemargin", 0.2).toUInt();
    calenhad_altitudemap_marker_normal_color = _settings -> value ("calenhad/altitudemap/marker/normal/color", "#0000FF").value<QColor>();
    calenhad_altitudemap_marker_selected_color = _settings -> value ("calenhad/altitudemap/marker/selected/color", "#FF0000").value<QColor>();
    calenhad_altitudemap_marker_fixed_color = _settings -> value ("calenhad/altitudemap/marker/fixed/color", "#7F7F7F").value<QColor>();
    calenhad_altitudemap_marker_normal_size = _settings -> value ("calenhad/altitudemap/marker/normal/size", 8).toUInt();
    calenhad_altitudemap_marker_selected_size = _settings -> value ("calenhad/altitudemap/marker/selected/size", 8).toUInt();
    calenhad_altitudemap_curve_color = _settings -> value ("calenhad/altitudemap/curve/color", "#7F0000").value<QColor>();
    calenhad_altitudemap_curve_width = _settings -> value ("calenhad/altitudemap/curve/width", 2).toUInt();

    // Configuration

    // File paths
    calenhad_legends_filename = _settings -> value ("calenhad/legends/filename", "/home/martin/.config/calenhad/legends.xml").toString ();
    calenhad_legends_filename_temp = _settings -> value ("calenhad/legends/filename/temp", "/home/martin/.config/calenhad/legends.temp.xml").toString ();
    calenhad_moduletypes_icons_path = _settings -> value ("calenhad/moduletypes/icons/path", "//home/martin/.config/calenhad/modules/icons/").toString();
    calenhad_moduletypes_filename = _settings -> value ("calenhad/moduletypes/filename", "/home/martin/.config/calenhad/modules/modules.xml").toString();
    calenhad_recentfiles_filename = _settings -> value ("calenhad/recentfiles/filename", "/home/martin/.config/calenhad/recentfiles.txt").toString ();
    calenhad_variables_name_maxlength = _settings -> value ("calenhad/variables/name/maxlength", 50).toUInt();

    // Validations
    calenhad_port_name_minLength = _settings -> value ("calenhad/port/name_minlength", 1).toUInt();
    calenhad_port_name_maxLength = _settings -> value ("calenhad/port/name/maxlength", 25).toUInt();
    calenhad_node_name_minLength = _settings -> value ("calenhad/node/name_minlength", 3).toUInt();
    calenhad_node_name_maxLength = _settings -> value ("calenhad/node/name/maxlength", 50).toUInt();
    calenhad_node_name_validChars = _settings -> value ("calenhad/node/name/validchars", "[A-Za-z_][A-Za-z_0-9]*").toString();
    
    // Notifications
    calenhad_notifications_width = _settings -> value ("calenhad/preferences/width", 300).toUInt();
    calenhad_notifications_margin = _settings -> value ("calenhad/preferences/margin", 5).toUInt();


    // Module names

    calenhad_module_icospheremap = _settings -> value ("calenhad/module/icospheremap", "icospheremap").toString();
    calenhad_module_altitudemap = _settings -> value ("calenhad/module/altitudemap", "altitudemap").toString();
    calenhad_module_raster = _settings -> value ("calenhad/module/raster", "raster").toString();
    calenhad_module_raster_file = _settings -> value ("calenhad/module/rasterfile", "rasterfile").toString();
    calenhad_nodegroup = _settings -> value ("calenhad/nodegroup", "nodegroup").toString();


    // Make stylesheet available to the application
    QString fileName =  calenhad_stylesheet;
    QFile file (fileName);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String (file.readAll());
    std::cout << "Stylesheet: " << file.fileName().toStdString() << "\n";
    setStylesheet (styleSheet);
    _styleSheet = styleSheet;

}

void Preferences::saveSettings() {

    // Colours and style

    _settings -> setValue ("calenhad/stylesheet", calenhad_stylesheet);

    _settings -> setValue ("calenhad/default_planet/radius", calenhad_default_planet_radius);
    _settings -> setValue ("calenhad/module/brush/color/normal", calenhad_module_brush_color_normal);
    _settings -> setValue ("calenhad/module/brush/color/selected", calenhad_module_brush_color_selected);
    _settings -> setValue ("calenhad/module/pen/color/normal", calenhad_module_pen_color_normal);
    _settings -> setValue ("calenhad/module/pen/color/selected", calenhad_module_pen_color_selected);
    _settings -> setValue ("calenhad/port/in/border/color", calenhad_port_in_border_color);
    _settings -> setValue ("calenhad/port/in/fill/color", calenhad_port_in_fill_color);
    _settings -> setValue ("calenhad/port/out/border/color", calenhad_port_out_border_color);
    _settings -> setValue ("calenhad/port/out/fill/color", calenhad_port_out_fill_color);
    _settings -> setValue ("calenhad/port/border/weight", calenhad_port_border_weight);
    _settings -> setValue ("calenhad/port/text/color", calenhad_port_text_color);
    _settings -> setValue ("calenhad/connector/normal/color", calenhad_connector_normal_color);
    _settings -> setValue ("calenhad/connector/normal/weight", calenhad_connector_normal_weight);
    _settings -> setValue ("calenhad/connector/selected/color", calenhad_connector_selected_color);
    _settings -> setValue ("calenhad/connector/selected/weight", calenhad_connector_selected_weight);
    _settings -> setValue ("calenhad/connector/drawing/color", calenhad_connector_drawing_color);
    _settings -> setValue ("calenhad/connector/drawing/weight", calenhad_connector_drawing_weight);
    _settings -> setValue ("calenhad/connector/inactive/color", calenhad_connector_inactive_color);
    _settings -> setValue ("calenhad/connector/inactive_weight", calenhad_connector_inactive_weight);
    _settings -> setValue ("calenhad/port/in/fill/color/drop", calenhad_port_in_fill_color_drop);
    _settings -> setValue ("calenhad/port/in/border/color/connected", calenhad_port_in_border_color_connected);
    _settings -> setValue ("calenhad/port/in/fill/color/connected", calenhad_port_in_fill_color_connected);
    _settings -> setValue ("calenhad/port/in/border/color/drop", calenhad_port_in_border_color_drop);
    _settings -> setValue ("calenhad/module/text/color/selected", calenhad_module_text_color_selected);
    _settings -> setValue ("calenhad/module/text/color/normal", calenhad_module_text_color_normal);
    _settings -> setValue ("calenhad/module/text/color/error", calenhad_module_text_color_error);
    _settings -> setValue ("calenhad/altitudemap/deletemargin", calenhad_altitudemap_deletemargin);
    _settings -> setValue ("calenhad/altitudemap/buffersize", calenhad_altitudemap_buffersize);
    _settings -> setValue ("calenhad/colormap/buffersize", calenhad_colormap_buffersize);
    _settings -> setValue ("calenhad/nodegroup", calenhad_nodegroup);
    _settings -> setValue ("calenhad/toolpalette/icon/color/shadow", calenhad_toolpalette_icon_color_shadow);
    _settings -> setValue ("calenhad/toolpalette/icon/color/normal", calenhad_toolpalette_icon_color_normal);
    _settings -> setValue ("calenhad/toolpalette/icon/color/mouseover", calenhad_toolpalette_icon_color_mouseover);
    _settings -> setValue ("calenhad/variables/color/normal", calenhad_variables_color_normal);
    _settings -> setValue ("calenhad/variables/color/error", calenhad_variables_color_error);
    _settings -> setValue ("calenhad/globe/scale/background/color", calenhad_globe_scale_background_color);
    _settings -> setValue ("calenhad/graticule/major/color", calenhad_graticule_major_color );
    _settings -> setValue ("calenhad/graticule/major/weight", calenhad_graticule_major_weight);
    _settings -> setValue ("calenhad/graticule/major/style", calenhad_graticule_major_style);
    _settings -> setValue ("calenhad/graticule/minor/color", calenhad_graticule_minor_color);
    _settings -> setValue ("calenhad/graticule/minor/weight", calenhad_graticule_minor_weight);
    _settings -> setValue ("calenhad/graticule/major/style", calenhad_graticule_minor_style);
    _settings -> setValue ("calenhad/graticule/major/text/size", calenhad_graticule_major_text_size);
    _settings -> setValue ("calenhad/graticule/major/text/color", calenhad_graticule_major_text_color);
    _settings -> setValue ("calenhad/graticule/major/text/size", calenhad_graticule_minor_text_size);
    _settings -> setValue ("calenhad/graticule/major/text/color", calenhad_graticule_minor_text_color);
    _settings -> setValue ("calenhad/graticule/density", calenhad_graticule_density);
    _settings -> setValue ("calenhad/graticule/visible", calenhad_graticule_visible);
    _settings -> setValue ("calenhad/altitudemap/marker/normal/color", calenhad_altitudemap_marker_normal_color);
    _settings -> setValue ("calenhad/altitudemap/marker/selected/color", calenhad_altitudemap_marker_selected_color);
    _settings -> setValue ("calenhad/altitudemap/marker/selected/color", calenhad_altitudemap_marker_selected_color);
    _settings -> setValue ("calenhad/altitudemap/marker/fixed/color", calenhad_altitudemap_marker_fixed_color);
    _settings -> setValue ("calenhad/altitudemap/marker/selected/size", calenhad_altitudemap_marker_selected_size);
    _settings -> setValue ("calenhad/altitudemap/curve/color", calenhad_altitudemap_curve_color);
    _settings -> setValue ("calenhad/altitudemap/curve/width", calenhad_altitudemap_curve_width);
    _settings -> setValue ("calenhad/desktop/grid/major/color", calenhad_desktop_grid_major_color);
    _settings -> setValue ("calenhad/graticule/major/style", calenhad_desktop_grid_major_style);
    _settings -> setValue ("calenhad/desktop/grid/major/weights", calenhad_desktop_grid_major_weight);
    _settings -> setValue ("calenhad/desktop/grid/minor/steps", calenhad_desktop_grid_major_steps);
    _settings -> setValue ("calenhad/desktop/grid/minor/color", calenhad_desktop_grid_minor_color);
    _settings -> setValue ("calenhad/graticule/minor/style", calenhad_desktop_grid_minor_style);
    _settings -> setValue ("calenhad/desktop/grid/minor/weights", calenhad_desktop_grid_minor_weight);
    _settings -> setValue ("calenhad/desktop/grid/density", calenhad_desktop_grid_density);
    _settings -> setValue ("calenhad/desktop/grid/visible", calenhad_desktop_grid_visible);
    _settings -> setValue ("calenhad/desktop/grid/snap", calenhad_desktop_grid_snap);

    // Configuration

    _settings -> setValue ("calenhad/legends/filename", calenhad_legends_filename);
    _settings -> setValue ("calenhad/legends/filename/temp", calenhad_legends_filename_temp);
    _settings -> setValue ("calenhad/moduletypes/icons/path", calenhad_moduletypes_icons_path);
    _settings -> setValue ("calenhad/moduletypes/filename", calenhad_moduletypes_filename);
    _settings -> setValue ("calenhad/recentfiles/filename", calenhad_recentfiles_filename);
    _settings -> setValue ("calenhad/variables/name/maxlength", calenhad_variables_name_maxlength);
    _settings -> setValue ("calenhad/port/name_minlength", calenhad_port_name_minLength);
    _settings -> setValue ("calenhad/port/name/maxlength", calenhad_port_name_maxLength);
    _settings -> setValue ("calenhad/node/name_minlength", calenhad_node_name_minLength);
    _settings -> setValue ("calenhad/node/name/maxlength", calenhad_node_name_maxLength);
    _settings -> setValue ("calenhad/node/name/validchars", calenhad_node_name_validChars);

    // Geometry

    _settings -> setValue ("calenhad/port/radius", calenhad_port_radius);
    _settings -> setValue ("calenhad/port/margin", calenhad_port_margin);
    _settings -> setValue ("calenhad/port/spacing", calenhad_port_spacing);
    _settings -> setValue ("calenhad/globe/scale/width", calenhad_globe_scale_width);
    _settings -> setValue ("calenhad/globe/scale/height", calenhad_globe_scale_height);
    _settings -> setValue ("calenhad/module/duplicate/offset/x", calenhad_module_duplicate_offset_x);
    _settings -> setValue ("calenhad/module/duplicate/offset/y", calenhad_module_duplicate_offset_y);
    _settings -> setValue ("calenhad/toolpalette/arrow/size", calenhad_toolpalette_arrow_size);
    _settings -> setValue ("calenhad/toolpalette/icon/size", calenhad_toolpalette_icon_size);
    _settings -> setValue ("calenhad/toolpalette/icon/margin", calenhad_toolpalette_icon_margin);
    _settings -> setValue ("calenhad/toolpalette/icon/shadow", calenhad_toolpalette_icon_shadow);
    _settings -> setValue ("calenhad/toolpalette/arrow/size", calenhad_toolpalette_arrow_size);
    _settings -> setValue ("calenhad/handle/module/width",  calenhad_handle_module_width);
    _settings -> setValue ("calenhad/handle/module/height", calenhad_handle_module_height);
    _settings -> setValue ("calenhad/handle/module/margin",  calenhad_handle_module_margin);
    _settings -> setValue ("calenhad/notifications/width", calenhad_notifications_width);
    _settings -> setValue ("calenhad/globe/zoom/min", calenhad_globe_zoom_min);
    _settings -> setValue ("calenhad/globe/zoom/max", calenhad_globe_zoom_max);
    _settings -> setValue ("calenhad/globe/inset/height", calenhad_globe_inset_height);
    _settings -> setValue ("calenhad/globe/texture/height", calenhad_globe_texture_height);
    _settings -> setValue ("calenhad/desktop/zoomlimit/zoomin", calenhad_desktop_zoom_limit_zoomin);
    _settings -> setValue ("calenhad/desktop/zoomlimit/zoomout", calenhad_desktop_zoom_limit_zoomout);
    _settings -> setValue ("calenhad/desktop/zoom/default", calenhad_desktop_zoom_default);
    _settings -> setValue ("calenhad/desktop/nodegroup/height/default", calenhad_desktop_nodegroup_height_default);
    _settings -> setValue ("calenhad/desktop/nodegroup/width/default", calenhad_desktop_nodegroup_width_default);
    _settings -> setValue ("calenhad/model/extent", calenhad_model_extent);
    _settings -> setValue ("calenhad/icosphere/depth", calenhad_icosphere_depth);

    // Modules

    _settings -> setValue ("calenhad/module/icospheremap", calenhad_module_icospheremap);
    _settings -> setValue ("calenhad/module/altitudemap", calenhad_module_altitudemap);
    _settings -> setValue ("calenhad/module/raster", calenhad_module_raster);
    _settings -> setValue ("calenhad/module/rasterfile", calenhad_module_raster_file);
    _settings -> setValue ("calenhad/nodegroup", calenhad_nodegroup);

}


