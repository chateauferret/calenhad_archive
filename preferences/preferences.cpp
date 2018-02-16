//
// Created by martin on 16/10/16.
//

#include "preferences.h"
#include <iostream>

using namespace calenhad::preferences;

Preferences::Preferences() {
    _settings = new QSettings();
    std::cout << "Preferences file: " <<  _settings -> fileName ().toStdString () << "\n";
}

Preferences::~Preferences () {
    if (_settings) { delete _settings; }
}

void Preferences::loadSettings() {
    bool ok;
    // Colours and style

    calenhad_stylesheet = _settings -> value ("calenhad/stylesheet", "/home/martin/.config/calenhad/darkorange.css").toString();
    calenhad_default_planet_radius = _settings -> value ("calenhad/default_planet/radius", 6371000).toDouble (&ok);
    calenhad_module_brush_color_selected = _settings -> value ("calenhad/module/brush/color/selected", "#00F0F0").value<QColor>();
    calenhad_module_brush_color_normal = _settings -> value ("calenhad/module/brush/color/normal", "#00F000").value<QColor>();
    calenhad_nodegroup_brush_color_selected = _settings -> value ("calenhad/nodegroup/brush/color/selected", "#802020").value<QColor>();
    calenhad_nodegroup_brush_color_normal = _settings -> value ("calenhad/nodegroup/brush/color/normal", "#FF4040").value<QColor>();
    calenhad_handle_brush_color_bg = _settings -> value ("calenhad/handle/brush/color/bg", "#323232").value<QColor>();
    calenhad_port_in_border_color = _settings -> value ("calenhad/port/in/border/color", "#008000").value<QColor>();
    calenhad_port_in_fill_color = _settings -> value ("calenhad/port/in/fill/color", "transparent").value<QColor>();
    calenhad_port_in_fill_color_drop = _settings -> value ("calenhad/port/in/fill/color/drop", "#8080FF").value<QColor>();
    calenhad_port_in_border_color_drop = _settings -> value ("calenhad/port/in/border/color/drop", "#0000FF").value<QColor>();
    calenhad_port_in_border_color_connected = _settings -> value ("calenhad/port/in/border/color/connected", "#008000").value<QColor>();
    calenhad_port_in_fill_color_connected = _settings -> value ("calenhad/port/in/fill/color/connected", "#00F000").value<QColor>();
    calenhad_port_out_border_color = _settings -> value ("calenhad/port/out/border/color", "#008080").value<QColor>();
    calenhad_port_out_fill_color = _settings -> value ("calenhad/port/out/fill/color", "#00F0F0").value<QColor>();
    calenhad_port_border_weight = _settings -> value ("calenhad/port/border/weight", 2).toUInt();
    calenhad_port_text_color = _settings -> value ("calenhad/port/text/color", "#000000").value<QColor>();
    calenhad_connector_normal_color = _settings -> value ("calenhad/connector/normal/color", "#000080").value<QColor>();
    calenhad_connector_normal_weight = _settings -> value ("calenhad/connector/normal/weight", 3).toUInt();
    calenhad_connector_selected_color = _settings -> value ("calenhad/connector/selected/color", "#FF0000").value<QColor>();
    calenhad_connector_selected_weight = _settings -> value ("calenhad/connector/selected/weight", 3).toUInt();
    calenhad_connector_drawing_color = _settings -> value ("calenhad/connector/drawing/color", "#000080").value<QColor>();
    calenhad_connector_drawing_weight = _settings -> value ("calenhad/connector/drawing/weight", 1).toUInt();
    calenhad_connector_inactive_color = _settings -> value ("calenhad/connector/inactive/color", "808080").value<QColor>();
    calenhad_connector_inactive_weight = _settings -> value ("calenhad/connector/inactive_weight", 1).toUInt();
    calenhad_module_text_color_selected = _settings -> value ("calenhad/module/text/color/selected", "#000000").value<QColor>();
    calenhad_module_text_color_normal = _settings -> value ("calenhad/module/text/color/normal", "#000000").value<QColor>();
    calenhad_module_text_color_error = _settings -> value ("calenhad/module/text/color/error", "#FF0000").value<QColor>();
    calenhad_nodegroup_text_color_selected = _settings -> value ("calenhad/handle/text/color/selected", "#000000").value<QColor>();
    calenhad_nodegroup_text_color_normal = _settings -> value ("calenhad/handle/text/color/normal", "#000000").value<QColor>();
    calenhad_altitudemap_deletemargin = _settings -> value ("calenhad/altitudemap/deletemargin", 0.2).toUInt();
    calenhad_altitudemap_buffersize = _settings -> value ("calenhad/altitudemap/buffersize", 2048).toUInt();
    calenhad_colormap_buffersize = _settings -> value ("calenhad/colormap/buffersize", 2048).toUInt();
    calenhad_toolpalette_icon_color_shadow = _settings -> value ("calenhad/toolpalette/icon/color/shadow", "#505050").value<QColor>();
    calenhad_toolpalette_icon_color_normal = _settings -> value ("calenhad/toolpalette/icon/color/normal", "#000000").value<QColor>();
    calenhad_toolpalette_icon_color_mouseover = _settings -> value ("calenhad/toolpalette/icon/color/mouseover", "#0000FF").value<QColor>();
    calenhad_variables_color_normal = _settings -> value ("calenhad/variables/color/normal", "#000000").value<QColor>();
    calenhad_variables_color_error = _settings -> value ("calenhad/variables/color/error", "#FF0000").value<QColor>();
    calenhad_globe_scale_background_color = _settings -> value ("calenhad/globe/scale/background/color", "#C0C0C0").value<QColor>();
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
    calenhad_graticule_density = _settings -> value ("calenhad/graticule/density", 2).toUInt();
    calenhad_graticule_visible = _settings -> value ("calenhad/graticule/visible", true).toBool();
    calenhad_altitudemap_marker_normal_color = _settings -> value ("calenhad/altitudemap/marker/normal/color", "#7F7F7F").value<QColor>();
    calenhad_altitudemap_marker_selected_color = _settings -> value ("calenhad/altitudemap/marker/selected/color", "#FF0000").value<QColor>();
    calenhad_altitudemap_marker_normal_size = _settings -> value ("calenhad/altitudemap/marker/normal/size", 8).toUInt();
    calenhad_altitudemap_marker_selected_size = _settings -> value ("calenhad/altitudemap/marker/selected/size", 8).toUInt();
    calenhad_altitudemap_curve_color = _settings -> value ("calenhad/altitudemap/curve/color", "#7F0000").value<QColor>();
    calenhad_altitudemap_curve_width = _settings -> value ("calenhad/altitudemap/curve/width", 2).toUInt();

    // Configuration

    calenhad_legends_filename = _settings -> value ("calenhad/legends/filename", "/home/martin/.config/calenhad/legends.xml").toString ();
    calenhad_recentfiles_filename = _settings -> value ("calenhad/recentfiles/filename", "/home/martin/.config/calenhad/recentfiles.txt").toString ();
    calenhad_variables_name_maxlength = _settings -> value ("calenhad/variables/name/maxlength", 50).toUInt();
    calenhad_port_name_minLength = _settings -> value ("calenhad/port/name_minlength", 1).toUInt();
    calenhad_port_name_maxLength = _settings -> value ("calenhad/port/name/maxlength", 25).toUInt();
    calenhad_node_name_minLength = _settings -> value ("calenhad/node/name_minlength", 3).toUInt();
    calenhad_node_name_maxLength = _settings -> value ("calenhad/node/name/maxlength", 50).toUInt();
    calenhad_node_name_validChars = _settings -> value ("calenhad/node/name/validchars", "[A-Za-z_][A-Za-z_0-9]*").toString();
    
    // Geometry

    calenhad_port_radius = _settings -> value ("calenhad/port/radius", 5).toUInt();
    calenhad_port_margin = _settings -> value ("calenhad/port//margin", 2).toUInt();
    calenhad_globe_scale_width = _settings -> value ("calenhad/globe/scale/width", 200).toUInt();
    calenhad_globe_scale_height = _settings -> value ("calenhad/globe/scale/height", 30).toUInt();
    calenhad_module_duplicate_offset_x = _settings -> value ("calenhad/module/duplicate/offset/x", 64).toUInt();
    calenhad_module_duplicate_offset_y = _settings -> value ("calenhad/module/duplicate/offset/y", 64).toUInt();
    calenhad_handle_module_width = _settings -> value ("calenhad/handle/module/width", 32).toUInt();
    calenhad_handle_module_height = _settings -> value ("calenhad/handle/module/height", calenhad_handle_module_width).toUInt();
    calenhad_handle_module_margin = _settings -> value ("calenhad/handle/module/margin", 2).toUInt();
    calenhad_notifications_width = _settings -> value ("calenhad/preferences/width", 150).toUInt();
    calenhad_notifications_margin = _settings -> value ("calenhad/preferences/margin", 5).toUInt();
    calenhad_toolpalette_icon_size = _settings -> value ("calenhad/toolpalette/icon/size", 24).toInt();
    calenhad_toolpalette_icon_margin = _settings -> value ("calenhad/toolpalette/icon/margin", 4).toInt();
    calenhad_toolpalette_icon_shadow = _settings -> value ("calenhad/toolpalette/icon/shadow", 2).toInt();
    calenhad_toolpalette_arrow_size = _settings -> value ("calenhad/toolpalette/arrow/size", 16).toInt();
    calenhad_globe_zoom_min = _settings -> value ("calenhad/globe/zoom/min", 0.0).toDouble (&ok);
    calenhad_globe_zoom_max = _settings -> value ("calenhad/globe/zoom/max", 4.0).toDouble (&ok);
    calenhad_globe_inset_height = _settings -> value ("calenhad/globe/inset/height", 192).toUInt (&ok);
    calenhad_globe_inset_x = _settings -> value ("calenhad/globe/inset/x", 8).toUInt (&ok);
    calenhad_globe_inset_y = _settings -> value ("calenhad/globe/inset/y", 8).toUInt (&ok);
    calenhad_globe_texture_height = _settings -> value ("calenhad/globe/texture/height", 1024).toUInt (&ok);

    // Module names

    calenhad_module_perlin = _settings -> value ("calenhad/module/perlin", "Perlin").toString();
    calenhad_module_simplex = _settings -> value ("calenhad/module/simplex", "Simplex").toString();
    calenhad_module_cylinders = _settings -> value ("calenhad/module/cylinders", "Cylinders").toString();
    calenhad_module_spheres = _settings -> value ("calenhad/module/spheres", "Spheres").toString();
    calenhad_module_exponent = _settings -> value ("calenhad/module/exponent", "Exponent").toString();
    calenhad_module_translate = _settings -> value ("calenhad/module/translate", "Translate").toString();
    calenhad_module_rotate = _settings -> value ("calenhad/module/rotate", "Rotate").toString();
    calenhad_module_clamp = _settings -> value ("calenhad/module/clamp", "Clamp").toString();
    calenhad_module_constant = _settings -> value ("calenhad/module/constant", "Constant").toString();
    calenhad_module_add = _settings -> value ("calenhad/module/add", "Add").toString();
    calenhad_module_abs = _settings -> value ("calenhad/module/abs", "Abs").toString();
    calenhad_module_blend = _settings -> value ("calenhad/module/blend", "Blend").toString();
    calenhad_module_invert = _settings -> value ("calenhad/module/invert", "Invert").toString();
    calenhad_module_max = _settings -> value ("calenhad/module/max", "Max").toString();
    calenhad_module_min = _settings -> value ("calenhad/module/min", "Min").toString();
    calenhad_module_multiply = _settings -> value ("calenhad/module/multiply", "Multiply").toString();
    calenhad_module_power = _settings -> value ("calenhad/module/power", "Power").toString();
    calenhad_module_displace = _settings -> value ("calenhad/module/displace", "Displace").toString();
    calenhad_module_diff = _settings -> value ("calenhad/module/diff", "Diff").toString();
    calenhad_module_billow = _settings -> value ("calenhad/module/billow", "Billow").toString();
    calenhad_module_ridgedmulti = _settings -> value ("calenhad/module/ridgedmulti", "Ridged multifractal").toString();
    calenhad_module_scalebias = _settings -> value ("calenhad/module/scalebias", "Scale and bias").toString();
    calenhad_module_select = _settings -> value ("calenhad/module/select", "Select").toString();
    calenhad_module_turbulence = _settings -> value ("calenhad/module/turbulence", "Turbulence").toString();
    calenhad_module_voronoi = _settings -> value ("calenhad/module/voronoi", "Voronoi").toString();
    calenhad_module_scalepoint = _settings -> value ("calenhad/module/scalepoint", "Scalepoint").toString();
    calenhad_module_icospheremap = _settings -> value ("calenhad/module/icospheremap", "Icosphere map").toString();
    calenhad_module_altitudemap = _settings -> value ("calenhad/module/altitudemap", "Altitude map").toString();
    calenhad_module_raster = _settings -> value ("calenhad/module/raster", "Raster").toString();
    calenhad_nodegroup = _settings -> value ("calenhad/nodegroup", "NodeGroup").toString();

}

void Preferences::saveSettings() {

    // Colours and style

    _settings -> setValue ("calenhad/stylesheet", calenhad_stylesheet);
    _settings -> setValue ("calenhad/module/brush/color/selected", calenhad_module_brush_color_selected);
    _settings -> setValue ("calenhad/default_planet/radius", calenhad_default_planet_radius);
    _settings -> setValue ("calenhad/module/brush/color/normal", calenhad_module_brush_color_normal);
    _settings -> setValue ("calenhad/nodegroup/brush/color/selected", calenhad_nodegroup_brush_color_selected);
    _settings -> setValue ("calenhad/nodegroup/brush/color/normal", calenhad_nodegroup_brush_color_normal);
    _settings -> setValue ("calenhad/handle/brush/color/bg", calenhad_handle_brush_color_bg);
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
    _settings -> setValue ("calenhad/nodegroup/text/color/selected", calenhad_nodegroup_text_color_selected);
    _settings -> setValue ("calenhad/nodegroup/text/color/normal", calenhad_nodegroup_text_color_normal);
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
    _settings -> setValue ("calenhad/altitudemap/marker/normal/size", calenhad_altitudemap_marker_normal_size);
    _settings -> setValue ("calenhad/altitudemap/marker/selected/size", calenhad_altitudemap_marker_selected_size);
    _settings -> setValue ("calenhad/altitudemap/curve/color", calenhad_altitudemap_curve_color);
    _settings -> setValue ("calenhad/altitudemap/curve/width", calenhad_altitudemap_curve_width);

    // Configuration

    _settings -> setValue ("calenhad/legends/filename", calenhad_legends_filename);
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
    _settings -> setValue ("calenhad/globe/inset/x", calenhad_globe_inset_x);
    _settings -> setValue ("calenhad/globe/inset/y", calenhad_globe_inset_y);
    _settings -> setValue ("calenhad/globe/texture/height", calenhad_globe_texture_height);

    // Modules

    _settings -> setValue ("calenhad/module/perlin", calenhad_module_perlin);
    _settings -> setValue ("calenhad/module/simplex", calenhad_module_simplex);
    _settings -> setValue ("calenhad/module/cylinders", calenhad_module_cylinders);
    _settings -> setValue ("calenhad/module/spheres", calenhad_module_spheres);
    _settings -> setValue ("calenhad/module/exponent", calenhad_module_exponent);
    _settings -> setValue ("calenhad/module/translate", calenhad_module_translate);
    _settings -> setValue ("calenhad/module/rotate", calenhad_module_rotate);
    _settings -> setValue ("calenhad/module/clamp", calenhad_module_clamp);
    _settings -> setValue ("calenhad/module/constant", calenhad_module_constant);
    _settings -> setValue ("calenhad/module/add", calenhad_module_add);
    _settings -> setValue ("calenhad/module/abs", calenhad_module_abs);
    _settings -> setValue ("calenhad/module/blend", calenhad_module_blend);
    _settings -> setValue ("calenhad/module/invert", calenhad_module_invert);
    _settings -> setValue ("calenhad/module/max", calenhad_module_max);
    _settings -> setValue ("calenhad/module/min", calenhad_module_min);
    _settings -> setValue ("calenhad/module/multiply", calenhad_module_multiply);
    _settings -> setValue ("calenhad/module/power", calenhad_module_power);
    _settings -> setValue ("calenhad/module/displace", calenhad_module_displace);
    _settings -> setValue ("calenhad/module/diff", calenhad_module_diff);
    _settings -> setValue ("calenhad/module/billow", calenhad_module_billow);
    _settings -> setValue ("calenhad/module/ridgedmulti", calenhad_module_ridgedmulti);
    _settings -> setValue ("calenhad/module/scalebias", calenhad_module_scalebias);
    _settings -> setValue ("calenhad/module/select", calenhad_module_select);
    _settings -> setValue ("calenhad/module/turbulence", calenhad_module_turbulence);
    _settings -> setValue ("calenhad/module/voronoi", calenhad_module_voronoi);
    _settings -> setValue ("calenhad/module/scalepoint", calenhad_module_scalepoint);
    _settings -> setValue ("calenhad/module/icospheremap", calenhad_module_icospheremap);
    _settings -> setValue ("calenhad/module/altitudemap", calenhad_module_altitudemap);
    _settings -> setValue ("calenhad/module/raster", calenhad_module_raster);
    _settings -> setValue ("calenhad/nodegroup", calenhad_nodegroup);

}


