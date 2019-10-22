//
// Created by martin on 25/04/17.
//

#ifndef CALENHAD_CHANGEALTITUDEMAPCOMMAND_H
#define CALENHAD_CHANGEALTITUDEMAPCOMMAND_H


#include <QtCore/QString>
#include <QtWidgets/QUndoCommand>
#include "../pipeline/CalenhadModel.h"

/* Preserves the state before an operation, and allows toggling between the pre-existing state and a new state.
 * Pattern for use in CalenhadModel:
 *
 *   preserve();        // store the pre-existing state in _oldXml
 *   doStuff();         // make the requested change to the model in response to UI action
 *   setChanged();      // mark the model dirty so that save is offered if discarding
 *   setRestorePoint(); // store the changed state and push it to the undo stack
 *
 */

namespace calenhad {
    namespace module {
        class Node;
    }
    namespace actions {

        class XmlCommand : public QUndoCommand {
        public:
            XmlCommand (calenhad::pipeline::CalenhadModel* model, QString  oldXml, QString  newXml = QString::null);

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
