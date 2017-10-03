//
// Created by martin on 01/10/17.
//

#ifndef CALENHAD_CONTEXTACTION_H
#define CALENHAD_CONTEXTACTION_H


#include <QtWidgets/QAction>
#include <QtWidgets/QGraphicsItem>

// A QAction subclass that remembers an object the action is to be applied to.

template <class T>
class ContextAction : public QAction {

public:
    ContextAction (T* context) : QAction(), _context (context) { };
    ContextAction (T* context, QObject *parent = nullptr) : QAction (parent), _context (context) {    };
    ContextAction (T* context, const QString &text, QObject *parent = nullptr) : QAction (text, parent), _context (context) { };;
    ContextAction (T* context, const QIcon &icon, const QString &text, QObject *parent = nullptr) : QAction (icon, text, parent), _context (context) { };
    T* context() { return _context; };

protected:
    T* _context;
};


#endif //CALENHAD_CONTEXTACTION_H
