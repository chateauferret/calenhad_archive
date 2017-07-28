//
// Created by martin on 23/05/17.
//

#ifndef CALENHAD_SCANLINERENDERER_H
#define CALENHAD_SCANLINERENDERER_H


#include <libnoise/noise.h>
#include <QtCore/QMutex>
#include <QtCore/QRunnable>
#include <mapping/RenderPoint.h>

namespace noise {
    namespace model {
        class Sphere;
    }
}
namespace calenhad {
    namespace legend {
        class Legend;
    }

    namespace pipeline {
        typedef std::vector<calenhad::mapping::RenderPoint> RenderLine;
        typedef std::vector<RenderLine> RenderBuffer;

        class ScanLineRenderer : public QObject, public QRunnable {
        Q_OBJECT
        public:
            ScanLineRenderer (const RenderBuffer::iterator& index, noise::model::Sphere* sphere, calenhad::legend::Legend* legend);

            //ScanLineRenderer (const ScanLineRendererParams& params, noise::model::Sphere* sphere, Legend* legend);
            virtual ~ScanLineRenderer ();

            void setFinalScanLine (const bool&);

        public slots:

            void run () override;

        signals:

            void scanline ();

            void complete ();


        protected:
            bool _final;
            noise::model::Sphere* _sphere;
            calenhad::legend::Legend* _legend;
            QMutex _mutex;

            RenderLine::iterator _item;
            RenderLine::iterator _end;
        };
    }
}

#endif //CALENHAD_SCANLINERENDERER_H
