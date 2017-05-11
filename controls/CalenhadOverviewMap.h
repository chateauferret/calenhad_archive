//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2008 Torsten Rahn <tackat@kde.org>
//

#ifndef MARBLEOVERVIEWMAP_H
#define MARBLEOVERVIEWMAP_H

#include <QHash>
#include <QColor>
#include <QPixmap>
#include <memory>

#include "marble/GeoDataLatLonAltBox.h"
#include "marble/AbstractFloatItem.h"
#include "marble/DialogConfigurationInterface.h"
#include "marble/MarbleModel.h"
#include "CalenhadOverviewInterface.h"
#include "CalenhadPreview.h"


class OverviewMapConfigWidget;
class CalenhadLayer;

/**
 * @short The class that creates an overview map.
 *
 */
using namespace Marble;

namespace Marble {
    class MarbleWidget;
}

class RenderJob;

    class CalenhadOverviewMap : public CalenhadPreview {

    public:
        CalenhadOverviewMap();
        explicit CalenhadOverviewMap (QModule* module, QWidget* widget);
        virtual ~CalenhadOverviewMap();
        void paintEvent (QPaintEvent* e);
        public slots:
        void setBounds (const Marble::GeoDataLatLonAltBox& bounds) override;
        QSize renderSize() override;
    protected:
        bool eventFilter( QObject *object, QEvent *e ) override;
        RenderJob* prepareRender() override;

        protected slots:
        virtual void jobComplete (std::shared_ptr<QImage> image) override;


    private:

        OverviewMapConfigWidget *ui_configWidget;
        QDialog *m_configDialog;
        MarbleWidget* _widget;
        qreal m_centerLat;
        qreal m_centerLon;


    //  QDialog* configDialog ();

        void drawGrid ();
        void drawBoundingBox ();

        int _renderWidth;
        int _renderHeight;
    };

#endif