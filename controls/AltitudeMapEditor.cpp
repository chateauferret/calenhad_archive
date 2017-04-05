//
// Created by martin on 31/03/17.
//


#include "AltitudeMapEditor.h"
#include "AltitudeMapPlot.h"
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_symbol.h>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialogButtonBox>


AltitudeMapEditor::AltitudeMapEditor (QWidget* parent) : QDialog (parent),
   _plot (new AltitudeMapPlot()) {
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout (layout);
    layout -> addWidget (_plot);
    QDialogButtonBox* buttonBox = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect (buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect (buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

AltitudeMapEditor::~AltitudeMapEditor () {

}

QVector<QPointF> AltitudeMapEditor::getEntries () {
    return _plot -> getEntries();
}

void AltitudeMapEditor::accept () {
   // close();
    emit accepted();
}

void AltitudeMapEditor::reject () {
   // close();
    emit rejected();
}

void AltitudeMapEditor::setEntries (const QVector<QPointF>& entries) {
   _plot -> setEntries (entries);
}
