//
// Created by martin on 16/10/16.
//

#ifndef CALENHAD_PREFERENCES_H
#define CALENHAD_PREFERENCES_H
#include <QSettings>
#include <QFont>
#include <QString>
#include <QColor>

class Preferences {
public:
    Preferences();
    ~Preferences();
    void saveSettings();
    void loadSettings();
    QSettings* settings() { return _settings; }
    QString calenhad_stylesheet;
    QColor calenhad_handle_brush_color_selected;
    QColor calenhad_handle_brush_color_normal;
    QColor calenhad_handle_brush_color_bg;
    unsigned calenhad_port_radius;
    unsigned calenhad_port_margin;
    QColor calenhad_port_in_border_color;
    QColor calenhad_port_in_fill_color;
    QColor calenhad_port_out_border_color;
    QColor calenhad_port_out_fill_color;
    QColor calenhad_port_control_border_color;
    QColor calenhad_port_control_fill_color;
    unsigned calenhad_port_border_weight;
    QColor calenhad_port_text_color;
    QColor calenhad_connector_normal_color;
    unsigned calenhad_connector_normal_weight;
    QColor calenhad_connector_selected_color;
    unsigned calenhad_connector_selected_weight;
    QColor calenhad_connector_drawing_color;
    unsigned calenhad_connector_drawing_weight;
    QColor calenhad_connector_inactive_color;
    unsigned calenhad_connector_inactive_weight;
    QColor calenhad_port_in_fill_color_drop;
    QColor calenhad_port_in_border_color_drop;
    QColor calenhad_handle_text_color_selected;
    QColor calenhad_handle_text_color_normal;
private:
    QSettings* _settings;


};


#endif //CALENHAD_PREFERENCES_H