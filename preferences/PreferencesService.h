//
// Created by martin on 15/04/17.
//

#ifndef CALENHAD_PREFERENCESSERVICE_H
#define CALENHAD_PREFERENCESSERVICE_H


#include <QtCore/QSettings>
#include <QColor>

namespace calenhad {
    namespace preferences {
        class PreferencesService {
        public:
            virtual void saveSettings () = 0;
            virtual void loadSettings () = 0;
            virtual QSettings* settings () = 0;

            // Colours and styles

            QString calenhad_stylesheet;
            double calenhad_default_planet_radius;
            QColor calenhad_module_brush_color_selected;
            QColor calenhad_module_brush_color_normal;
            QColor calenhad_module_pen_color_selected;
            QColor calenhad_module_pen_color_normal;
            QColor calenhad_port_in_border_color;
            QColor calenhad_port_in_fill_color;
            QColor calenhad_port_in_fill_color_drop;
            QColor calenhad_port_in_border_color_drop;
            QColor calenhad_port_in_fill_color_connected;
            QColor calenhad_port_in_border_color_connected;
            QColor calenhad_port_out_border_color;
            QColor calenhad_port_out_fill_color;
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
            QColor calenhad_module_text_color_selected;
            QColor calenhad_module_text_color_normal;
            QColor calenhad_module_text_color_error;
            QColor calenhad_variables_color_normal;
            QColor calenhad_variables_color_error;
            QColor calenhad_globe_scale_background_color;
            QColor calenhad_graticule_major_color;
            unsigned calenhad_graticule_major_weight;
            int calenhad_graticule_major_style;
            QColor calenhad_graticule_minor_color;
            unsigned calenhad_graticule_minor_weight;
            int calenhad_graticule_minor_style;
            unsigned calenhad_graticule_major_text_size;
            QColor calenhad_graticule_major_text_color;
            unsigned calenhad_graticule_minor_text_size;
            QColor calenhad_graticule_minor_text_color;
            unsigned calenhad_graticule_density;
            bool calenhad_graticule_visible;
            QColor calenhad_altitudemap_marker_normal_color;
            QColor calenhad_altitudemap_marker_selected_color;
            QColor calenhad_altitudemap_marker_fixed_color;
            unsigned calenhad_altitudemap_marker_normal_size;
            unsigned calenhad_altitudemap_marker_selected_size;
            QColor calenhad_altitudemap_curve_color;
            unsigned calenhad_altitudemap_curve_width;
            QColor calenhad_desktop_grid_major_color;
            int calenhad_desktop_grid_major_style;
            unsigned calenhad_desktop_grid_major_weight;
            unsigned calenhad_desktop_grid_major_steps;
            QColor calenhad_desktop_grid_minor_color;
            int calenhad_desktop_grid_minor_style;
            unsigned calenhad_desktop_grid_minor_weight;
            unsigned calenhad_desktop_grid_density;
            bool calenhad_desktop_grid_visible;
            bool calenhad_desktop_grid_snap;

            // Configuration

            QString calenhad_legends_filename;
            QString calenhad_legends_filename_temp;
            QString calenhad_moduletypes_icons_path;
            QString calenhad_moduletypes_filename;
            QString calenhad_recentfiles_filename;
            int calenhad_variables_name_maxlength;
            int calenhad_port_name_minLength;
            int calenhad_port_name_maxLength;
            int calenhad_node_name_minLength;
            int calenhad_node_name_maxLength;
            QString calenhad_node_name_validChars;


            // Geometry

            unsigned calenhad_port_radius;
            unsigned calenhad_port_margin;
            unsigned calenhad_port_spacing;
            unsigned calenhad_module_duplicate_offset_x;
            unsigned calenhad_module_duplicate_offset_y;
            unsigned calenhad_altitudemap_deletemargin;
            unsigned calenhad_altitudemap_buffersize;
            unsigned calenhad_colormap_buffersize;
            int calenhad_toolpalette_icon_size;
            int calenhad_toolpalette_icon_margin;
            int calenhad_toolpalette_icon_shadow;
            int calenhad_toolpalette_arrow_size;
            int calenhad_handle_module_width;
            int calenhad_handle_module_height;
            int calenhad_handle_module_margin;
            int calenhad_notifications_width;
            int calenhad_notifications_margin;
            double calenhad_globe_zoom_max;
            double calenhad_globe_zoom_min;
            int calenhad_globe_scale_width;
            int calenhad_globe_scale_height;
            unsigned calenhad_globe_inset_height;
            unsigned calenhad_globe_texture_height;
            double calenhad_desktop_zoom_limit_zoomin;
            double calenhad_desktop_zoom_limit_zoomout;
            double calenhad_desktop_zoom_default;
            double calenhad_desktop_nodegroup_height_default;
            double calenhad_desktop_nodegroup_width_default;
            double calenhad_model_extent;
            int calenhad_icosphere_depth;

            // Modules

            QString calenhad_module_icospheremap;
            QString calenhad_module_altitudemap;
            QString calenhad_module_raster;
            QString calenhad_module_raster_file;
            QString calenhad_nodegroup;
            QColor calenhad_toolpalette_icon_color_normal;
            QColor calenhad_toolpalette_icon_color_mouseover;
            QColor calenhad_toolpalette_icon_color_shadow;

        };
    }
}


#endif //CALENHAD_PREFERENCESSERVICE_H
