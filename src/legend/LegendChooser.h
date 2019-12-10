//
// Created by martin on 27/05/17.
//

#ifndef CALENHAD_LEGENDCHOOSER_H
#define CALENHAD_LEGENDCHOOSER_H


#include <QtWidgets/QComboBox>
#include <QShowEvent>

namespace calenhad {
    namespace legend {
        class LegendService;

        class LegendChooser : public QComboBox {
        Q_OBJECT
        public:
            LegendChooser (LegendService* service, QWidget* parent = 0);

            virtual ~LegendChooser ();

            void showEvent (QShowEvent* e);

            Legend* selectedLegend ();


        public slots:

            void setLabel (const QString& oldLabel, const QString& newLabel);

            void setCurrentIcon (const QIcon& icon);

            void refresh ();

        signals:

            void legendSelected (const QString& name);

        protected:
            LegendService* _service;
            QStringList _legends;
            QString _name;
            QString _oldName;

        protected slots:

            void legendChosen ();


        };
    }
}


#endif //CALENHAD_LEGENDCHOOSER_H
