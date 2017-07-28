//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2012      Bernhard Beschow <bbeschow@cs.tu-berlin.de>
//
#ifndef MARBLE_CALENHADOVERVIEWINTERFACE_H
#define MARBLE_CALENHADOVERVIEWINTERFACE_H

#include <QObject> // for Q_DECLARE_INTERFACE macro
#include "marble/marble_export.h"
namespace calenhad {
    namespace controls {
        namespace globe {


/**
 * @brief This interface allows a plugin to accept a Calenhad QModule to source data for rendering an image.
 *
 * @note Make sure to deploy the @code Q_INTERFACES @endcode macro, which tells
 * Marble that you actually implemented the interface.
 */
            class MARBLE_EXPORT CalenhadOverviewInterface {
            public slots:

                virtual void setImage (const QImage&) = 0;
            };
        }
    }
}


Q_DECLARE_INTERFACE(calenhad::controls::globe::CalenhadOverviewInterface, "com.calenhad.CalenhadOverviewInterface/1.0" )

#endif