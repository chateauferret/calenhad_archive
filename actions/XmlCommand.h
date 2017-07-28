//
// Created by martin on 25/04/17.
//

#ifndef CALENHAD_CHANGEALTITUDEMAPCOMMAND_H
#define CALENHAD_CHANGEALTITUDEMAPCOMMAND_H


#include <QtCore/QString>
#include <QtWidgets/QUndoCommand>

namespace calenhad {
    namespace qmodule {
        class QNode;
    }
    namespace actions {

        class XmlCommand : public QUndoCommand {
        public:
            XmlCommand (calenhad::qmodule::QNode* node, const QString& oldXml, const QString& newXml);

            virtual ~XmlCommand ();

            virtual void redo () override;

            virtual void undo () override;

        protected:
            QString _oldXml, _newXml;
            calenhad::qmodule::QNode* _node;
        };
    }
}

#endif //CALENHAD_CHANGEALTITUDEMAPCOMMAND_H
