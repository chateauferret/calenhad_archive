//
// Created by martin on 06/03/17.
//

#ifndef CALENHAD_QICOSPHERENODE_H
#define CALENHAD_QICOSPHERENODE_H


#include "QModule.h"
#include "../icosphere/Bounds.h"
#include <memory>
#include <QPushButton>
#include <QLabel>
#include <icosphere/icosphere.h>
#include <icosphere/IcosphereModule.h>
#include <QtWidgets/QAction>


namespace icosphere {
    class Icosphere;
}

namespace calenhad {
    namespace qmodule {


        class QIcosphereMap : public QModule {
        Q_OBJECT
        public:
            static QIcosphereMap* newInstance ();

            virtual ~QIcosphereMap ();

            void initialise () override;

            noise::module::IcosphereModule* module () override;

            QIcosphereMap* clone () override;

            QString nodeType () override;

            void setIcosphereDepth (const unsigned& depth);

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;

            QMenu* boundsMenu ();
            //void icosphereBuilt (std::shared_ptr<icosphere::Icosphere> icosphere);

        public slots:

            void setIcosphereDepth ();

            void setBounds (const icosphere::Bounds&);

            virtual bool generateMap () override;

            void boundsMenuRequested ();

            void boundsChangeRequested ();

            bool isComplete() override;
        signals:
            void initialised ();
            void complete (std::shared_ptr<icosphere::Icosphere>);

            void icosphereChangeRequested ();

            void progress (int done);

        protected:
            QIcosphereMap (QWidget* parent = 0);

            unsigned _depth;
            icosphere::Bounds _bounds;
            QSpinBox* _depthSpin;
            QLabel* _vertexCountLabel;

            virtual void addInputPorts () override;

            const QString _key = "altitude";

            QLabel* _boundsLabel;
            QPushButton* _boundsButton;
            QMenu* _boundsMenu;
            QAction* _displayedBoundsAction, * _wholeWorldBoundsAction; // to do - list previous bounds used and select one or step back/forwards; enter geocoordinates for custom bounds

        };
    }
}

#endif //CALENHAD_QICOSPHERENODE_H
