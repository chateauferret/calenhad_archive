//
// Created by martin on 31/03/17.
//


#include "AltitudeMapEditor.h"
#include "AltitudeMapPlot.h"
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_symbol.h>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QDialogButtonBox>
#include "AltitudeMapping.h"

using namespace calenhad::controls::altitudemap;

AltitudeMapEditor::AltitudeMapEditor (QWidget* parent) : QDialog (parent),
   _plot (new AltitudeMapPlot (100, this)) {
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout (layout);
    layout -> addWidget (_plot);
    QDialogButtonBox* buttonBox = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect (buttonBox, &QDialogButtonBox::accepted, this, &AltitudeMapEditor::accept);
    connect (buttonBox, &QDialogButtonBox::rejected, this, &AltitudeMapEditor::reject);
    _curveTypeCombo = new QComboBox();
    _curveTypeOptions.insert ("Altitude curve", CurveType::Altitude);
    _curveTypeOptions.insert ("Terrace", CurveType::Terrace);
    _curveTypeOptions.insert ("Inverted Terrace", CurveType::InvertedTerrace);
    _curveTypeCombo -> addItems (_curveTypeOptions.keys());
    connect (_curveTypeCombo, SIGNAL (currentIndexChanged (int)), this, SLOT (curveTypeSelected (int)));
    layout -> addWidget (_curveTypeCombo);
    layout -> addWidget (buttonBox);
}

AltitudeMapEditor::~AltitudeMapEditor() {

}

void AltitudeMapEditor::curveTypeSelected (int index) {
    _plot -> setCurveType (_curveTypeOptions.find (_curveTypeCombo -> currentText()).value ());
}

CurveType AltitudeMapEditor::curveType() {
    return _plot -> curveType();
}

QVector<AltitudeMapping> AltitudeMapEditor::getEntries() {
    return _plot -> getEntries ();
}

void AltitudeMapEditor::accept() {
    QDialog::accept();
    emit accepted();
}

void AltitudeMapEditor::reject() {
   QDialog::reject();
    emit rejected();
}

void AltitudeMapEditor::setEntries (QVector<AltitudeMapping>& entries) {
   _plot -> setEntries (entries);
}

void AltitudeMapEditor::setCurveType (CurveType curveType) {
    _plot -> setCurveType (curveType);
    for (int i = 0; i < _curveTypeOptions.size (); i++) {
        if (curveType == _curveTypeOptions.values ().at (i)) {
            if (_curveTypeOptions.keys ().at (i) != _curveTypeCombo->currentText ()) {
                _curveTypeCombo->setCurrentText (_curveTypeOptions.keys ().at (i));
            }
        }
    }
}

void AltitudeMapEditor::showEvent (QShowEvent* event) {
    //CurveType ct = _plot -> curveType();
    //_curveTypeCombo -> setCurrentIndex (ct);
}
