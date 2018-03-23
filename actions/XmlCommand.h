//
// Created by martin on 25/04/17.
//

#ifndef CALENHAD_CHANGEALTITUDEMAPCOMMAND_H
#define CALENHAD_CHANGEALTITUDEMAPCOMMAND_H


#include <QtCore/QString>
#include <QtWidgets/QUndoCommand>
#include "../pipeline/CalenhadModel.h"

namespace calenhad {
    namespace qmodule {
        class Node;
    }
    namespace actions {

        class XmlCommand : public QUndoCommand {
        public:
            XmlCommand (calenhad::pipeline::CalenhadModel* model, const QString& oldXml, const QString& newXml = QString::null);

            virtual ~XmlCommand ();

            virtual void redo () override;

            virtual void undo () override;

            void setNewXml (const QString& xml);

        protected:
            QString _oldXml, _newXml;
            calenhad::pipeline::CalenhadModel* _model;

        };
    }
}

#endif //CALENHAD_CHANGEALTITUDEMAPCOMMAND_H
