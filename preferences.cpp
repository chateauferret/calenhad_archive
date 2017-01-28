//
// Created by martin on 16/10/16.
//

#include "preferences.h"
#include <iostream>

Preferences::Preferences() {
    _settings = new QSettings();
    std::cout << "Settings file: " <<  _settings -> fileName ().toStdString () << "\n";
}

Preferences::~Preferences () {
    if (_settings) { delete _settings; }
}

void Preferences::loadSettings() {
    calenhad_stylesheet = _settings -> value ("calenhad/stylesheet", "/home/martin/.config/calenhad/darkorange.css").toString();
    calenhad_handle_brush_color_selected = _settings -> value ("calenhad/handle/brush/color/selected", "#00F0F0").value<QColor>();
    calenhad_handle_brush_color_normal = _settings -> value ("calenhad/handle/brush/color/normal", "#00F000").value<QColor>();
    calenhad_handle_brush_color_bg = _settings -> value ("calenhad/handle/brush/color/bg", "#323232").value<QColor>();
    calenhad_port_radius = _settings -> value ("calenhad/port/radius", 5).toUInt();
    calenhad_port_margin = _settings -> value ("calenhad/port/_margin", 2).toUInt();
    calenhad_port_in_border_color = _settings -> value ("calenhad/port/in/border/color", "#008000").value<QColor>();
    calenhad_port_in_fill_color = _settings -> value ("calenhad/port/in/fill/color", "#00F000").value<QColor>();
    calenhad_port_out_border_color = _settings -> value ("calenhad/port/out/border/color", "#008080").value<QColor>();
    calenhad_port_out_fill_color = _settings -> value ("calenhad/port/out/fill/color", "#00F0F0").value<QColor>();
    calenhad_port_control_border_color = _settings -> value ("calenhad/port/in/border/color", "#008080").value<QColor>();
    calenhad_port_control_fill_color = _settings -> value ("calenhad/port/in/fill/color", "#00F0F0").value<QColor>();
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
    calenhad_port_in_fill_color_drop = _settings -> value ("calenhad/port/in/fill/color/drop", "#8080FF").value<QColor>();
    calenhad_port_in_border_color_drop = _settings -> value ("calenhad/port/in/border/color/drop", "#0000FF").value<QColor>();
    calenhad_handle_text_color_selected = _settings -> value ("calenhad/handle/text/color/selected", "#000000").value<QColor>();
    calenhad_handle_text_color_normal = _settings -> value ("calenhad/handle/text/color/normal", "#000000").value<QColor>();
}

void Preferences::saveSettings() {
    _settings -> setValue ("calenhad/stylesheet", calenhad_stylesheet);
    _settings -> setValue ("calenhad/handle/brush/color/selected", calenhad_handle_brush_color_selected);
    _settings -> setValue ("calenhad/handle/brush/color/normal", calenhad_handle_brush_color_normal);
    _settings -> setValue ("calenhad/handle/brush/color/bg", calenhad_handle_brush_color_bg);
    _settings -> setValue ("calenhad/port/radius", calenhad_port_radius);
    _settings -> setValue ("calenhad/port/_margin", calenhad_port_margin);
    _settings -> setValue ("calenhad/port/in/border/color", calenhad_port_in_border_color);
    _settings -> setValue ("calenhad/port/in/fill/color", calenhad_port_in_fill_color);
    _settings -> setValue ("calenhad/port/out/border/color", calenhad_port_out_border_color);
    _settings -> setValue ("calenhad/port/out/fill/color", calenhad_port_out_fill_color);
    _settings -> setValue ("calenhad/port/in/border/color", calenhad_port_control_border_color);
    _settings -> setValue ("calenhad/port/in/fill/color", calenhad_port_control_fill_color);
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
    _settings -> setValue ("calenhad/port/in/border/color/drop", calenhad_port_in_border_color_drop);
    _settings -> setValue ("calenhad/handle/text/color/selected", calenhad_handle_text_color_selected);
    _settings -> setValue ("calenhad/handle/text/color/normal", calenhad_handle_text_color_normal);
}


