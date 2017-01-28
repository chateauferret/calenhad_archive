//
// Created by martin on 23/01/17.
//

#ifndef CALENHAD_ZOOMACTION_H
#define CALENHAD_ZOOMACTION_H


#include <QtWidgets/QUndoCommand>
#include "../nodeedit/CalenhadView.h"

class ZoomCommand : public QUndoCommand {

public:
    ZoomCommand (double delta, CalenhadView* view);
    virtual ~ZoomCommand();
    virtual void undo() override;
    virtual void redo() override;

protected:
    double _delta, _previousZoom;
    CalenhadView* _view;
};

class ZoomToFitCommand : public ZoomCommand {
public:
    ZoomToFitCommand (CalenhadView* view);
    void redo() override;
protected:
    void zoomToFit (QList<QGraphicsItem*> list);
};


class ZoomToSelectionCommand : public ZoomToFitCommand {
public:
    ZoomToSelectionCommand (CalenhadView* view);
    void redo() override;
};

#endif //CALENHAD_ZOOMACTION_H
