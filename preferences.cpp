//
// Created by martin on 16/10/16.
//

#include "preferences.h"
#include <iostream>

Preferences::Preferences() {
    _settings = new QSettings();
    std::cout << "Preferences file: " <<  _settings -> fileName ().toStdString () << "\n";
}

Preferences::~Preferences () {
    if (_settings) { delete _settings; }
}

void Preferences::loadSettings() {
    calenhad_stylesheet = _settings -> value ("calenhad/stylesheet", "/home/martin/.config/calenhad/darkorange.css").toString();
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
    calenhad_module_text_color_selected = _settings -> value ("calenhad/handle/text/color/selected", "#000000").value<QColor>();
    calenhad_module_text_color_normal = _settings -> value ("calenhad/handle/text/color/normal", "#000000").value<QColor>();
    calenhad_nodegroup_text_color_selected = _settings -> value ("calenhad/handle/text/color/selected", "#000000").value<QColor>();
    calenhad_nodegroup_text_color_normal = _settings -> value ("calenhad/handle/text/color/normal", "#000000").value<QColor>();
    calenhad_altitudemap_deletemargin = _settings -> value ("calenhad/altitude/deletemargin", 0.2).toUInt();
    calenhad_legends_filename = _settings -> value ("calenhad/legends/filename", "/home/martin/ClionProjects/calenhad/config/legends.xml").toString ();

    // Geometry

    calenhad_port_radius = _settings -> value ("calenhad/port/radius", 5).toUInt();
    calenhad_port_margin = _settings -> value ("calenhad/port/_margin", 2).toUInt();
    calenhad_module_duplicate_offset_x = _settings -> value ("calenhad/module/duplicate/offset/x", 64).toUInt();
    calenhad_module_duplicate_offset_y = _settings -> value ("calenhad_module_duplicate_offset_y", 64).toUInt();

    // Module names

    calenhad_module_perlin = _settings -> value ("calenhad/owner/perlin", "Perlin").toString();
    calenhad_module_cylinders = _settings -> value ("calenhad/owner/cylinders", "Cylinders").toString();
    calenhad_module_spheres = _settings -> value ("calenhad/owner/spheres", "Spheres").toString();
    calenhad_module_exponent = _settings -> value ("calenhad/owner/exponent", "Exponent").toString();
    calenhad_module_translate = _settings -> value ("calenhad/owner/translate", "Translate").toString();
    calenhad_module_rotate = _settings -> value ("calenhad/owner/rotate", "Rotate").toString();
    calenhad_module_clamp = _settings -> value ("calenhad/owner/clamp", "Clamp").toString();
    calenhad_module_constant = _settings -> value ("calenhad/owner/constant", "Constant").toString();
    calenhad_module_add = _settings -> value ("calenhad/owner/add", "Add").toString();
    calenhad_module_abs = _settings -> value ("calenhad/owner/abs", "Abs").toString();
    calenhad_module_blend = _settings -> value ("calenhad/owner/blend", "Blend").toString();
    calenhad_module_cache = _settings -> value ("calenhad/owner/cache", "Cache").toString();
    calenhad_module_checkerboard = _settings -> value ("calenhad/owner/checkerboard", "Checkerboard").toString();
    calenhad_module_invert = _settings -> value ("calenhad/owner/invert", "Invert").toString();
    calenhad_module_max = _settings -> value ("calenhad/owner/max", "Max").toString();
    calenhad_module_min = _settings -> value ("calenhad/owner/min", "Min").toString();
    calenhad_module_multiply = _settings -> value ("calenhad/owner/multiply", "Multiply").toString();
    calenhad_module_power = _settings -> value ("calenhad/owner/power", "Power").toString();
    calenhad_module_displace = _settings -> value ("calenhad/owner/displace", "Displace").toString();
    calenhad_module_diff = _settings -> value ("calenhad/owner/diff", "Diff").toString();
    calenhad_module_billow = _settings -> value ("calenhad/owner/billow", "Billow").toString();
    calenhad_module_ridgedmulti = _settings -> value ("calenhad/owner/ridgedmulti", "Ridged multiftactal").toString();
    calenhad_module_scalebias = _settings -> value ("calenhad/owner/scalebias", "Scale and bias").toString();
    calenhad_module_select = _settings -> value ("calenhad/owner/select", "Select").toString();
    calenhad_module_turbulence = _settings -> value ("calenhad/owner/turbulence", "Turbulence").toString();
    calenhad_module_voronoi = _settings -> value ("calenhad/owner/voronoi", "Voronoi").toString();
    calenhad_module_scalepoint = _settings -> value ("calenhad/owner/scalepoint", "Scalepoint").toString();
    calenhad_module_icospheremap = _settings -> value ("calenhad/owner/icospheremap", "Icosphere map").toString();
    calenhad_module_altitudemap = _settings -> value ("calenhad/owner/altitudemap", "Altitude map").toString();

}

void Preferences::saveSettings() {
    _settings -> setValue ("calenhad/stylesheet", calenhad_stylesheet);
    _settings -> setValue ("calenhad/module/brush/color/selected", calenhad_module_brush_color_selected);
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
    _settings -> setValue ("calenhad/nodegroup/text/color/selected", calenhad_nodegroup_text_color_selected);
    _settings -> setValue ("calenhad/nodegroup/text/color/normal", calenhad_nodegroup_text_color_normal);
    _settings -> setValue ("calenhad/altitudemap/deletemargin", calenhad_altitudemap_deletemargin);
    _settings -> setValue ("calenhad/legends/filename", calenhad_legends_filename);

    // Geometry

    _settings -> setValue ("calenhad/port/radius", calenhad_port_radius);
    _settings -> setValue ("calenhad/port/_margin", calenhad_port_margin);
    _settings -> setValue ("calenhad/module/duplicate/offset/x", calenhad_module_duplicate_offset_x);
    _settings -> setValue ("calenhad_module_duplicate_offset_y", calenhad_module_duplicate_offset_y);

    // Modules

    _settings -> setValue ("calenhad/owner/perlin", calenhad_module_perlin);
    _settings -> setValue ("calenhad/owner/cylinders", calenhad_module_cylinders);
    _settings -> setValue ("calenhad/owner/spheres", calenhad_module_spheres);
    _settings -> setValue ("calenhad/owner/exponent", calenhad_module_exponent);
    _settings -> setValue ("calenhad/owner/translate", calenhad_module_translate);
    _settings -> setValue ("calenhad/owner/rotate", calenhad_module_rotate);
    _settings -> setValue ("calenhad/owner/clamp", calenhad_module_clamp);
    _settings -> setValue ("calenhad/owner/constant", calenhad_module_constant);
    _settings -> setValue ("calenhad/owner/add", calenhad_module_add);
    _settings -> setValue ("calenhad/owner/abs", calenhad_module_abs);
    _settings -> setValue ("calenhad/owner/blend", calenhad_module_blend);
    _settings -> setValue ("calenhad/owner/cache", calenhad_module_cache);
    _settings -> setValue ("calenhad/owner/checkerboard", calenhad_module_checkerboard);
    _settings -> setValue ("calenhad/owner/invert", calenhad_module_invert);
    _settings -> setValue ("calenhad/owner/max", calenhad_module_max);
    _settings -> setValue ("calenhad/owner/min", calenhad_module_min);
    _settings -> setValue ("calenhad/owner/multiply", calenhad_module_multiply);
    _settings -> setValue ("calenhad/owner/power", calenhad_module_power);
    _settings -> setValue ("calenhad/owner/displace", calenhad_module_displace);
    _settings -> setValue ("calenhad/owner/diff", calenhad_module_diff);
    _settings -> setValue ("calenhad/owner/billow", calenhad_module_billow);
    _settings -> setValue ("calenhad/owner/ridgedmulti", calenhad_module_ridgedmulti);
    _settings -> setValue ("calenhad/owner/scalebias", calenhad_module_scalebias);
    _settings -> setValue ("calenhad/owner/select", calenhad_module_select);
    _settings -> setValue ("calenhad/owner/turbulence", calenhad_module_turbulence);
    _settings -> setValue ("calenhad/owner/voronoi", calenhad_module_voronoi);
    _settings -> setValue ("calenhad/owner/scalepoint", calenhad_module_scalepoint);
    _settings -> setValue ("calenhad/owner/icospheremap", calenhad_module_icospheremap);
    _settings -> setValue ("calenhad/owner/altitudemap", calenhad_module_altitudemap);

}


