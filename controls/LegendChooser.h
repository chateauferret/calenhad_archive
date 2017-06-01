//
// Created by martin on 27/05/17.
//

#ifndef CALENHAD_LEGENDCHOOSER_H
#define CALENHAD_LEGENDCHOOSER_H


#include <QtWidgets/QComboBox>
#include "../mapping/Legend.h"

class QShowEvent;

class LegendChooser : public QComboBox {
    Q_OBJECT
public:
    LegendChooser (QWidget* parent);
    virtual ~LegendChooser();
    void showEvent (QShowEvent* e);
    Legend* selectedLegend();


    public slots:
    void setLabel (const QString& oldLabel, const QString& newLabel);
    void setCurrentIcon (const QIcon& icon);
    void refresh ();

signals:
    void legendSelected (const QString& name);

protected:
    QStringList _legends;
    QString _name;
    QString _oldName;

protected slots:
    void legendChosen ();


};


#endif //CALENHAD_LEGENDCHOOSER_H
