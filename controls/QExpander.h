//
// Created by martin on 18/11/16.
//

#ifndef CALENHAD_QEXPANDER_H
#define CALENHAD_QEXPANDER_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QToolBar>

class QExpander: public QToolBar {

Q_OBJECT
public:
    QExpander (const QString& title, QWidget* parent);
    ~QExpander();
    void setTitle (const QString& title);
    bool isExpanded() const;
    void setExpanded (const bool& expanded);
    void showEvent (QShowEvent* event) override;

    signals:
    void expansionChanged (const bool& expanded);

    public slots:
    void toggle ();

protected:
    QPushButton* _button;
};


#endif //CALENHAD_QEXPANDER_H
