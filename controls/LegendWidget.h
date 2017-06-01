//
// Created by martin on 31/05/17.
//

#ifndef CALENHAD_GRADIENTLEGENDWIDGET_H
#define CALENHAD_GRADIENTLEGENDWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QCheckBox>


class Legend;
class LegendEditor;

typedef QPair<qreal, QColor> LegendEntry;

class LegendWidget : public QWidget {
    Q_OBJECT
public:
    LegendWidget (QWidget* parent = 0);
    virtual ~LegendWidget ();

    QLineEdit* _legendNameBox;
    LegendEditor* _legendEditor;
    QCheckBox* _legendInterpolateCheck;
    QTextEdit* _legendNotesBox;
    void showLegend (Legend* legend);
    void updateLegend (Legend* legend);
    QIcon icon();
    void showEvent (QShowEvent* event) override;
    QString name ();

    signals:
    void nameChanged (QString);
    void iconChanged (QIcon);

protected:

    QString _name;
    QList<LegendEntry> _entries;
    bool _interpolate;
    QString _notes;



};


#endif //CALENHAD_GRADIENTLEGENDWIDGET_H
