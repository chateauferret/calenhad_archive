//
// Created by martin on 10/05/17.
//

#include <QtWidgets/QTabWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QCloseEvent>
#include <QFormLayout>
#include <QTextEdit>
#include <QtWidgets/QDialogButtonBox>
#include "CalenhadGlobeDialog.h"
#include "CalenhadGlobeConfigDialog.h"
#include "CalenhadServices.h"
#include "CalenhadStatsTab.h"
#include "../../legend/Legend.h"
#include "../../legend/LegendManager.h"
#include "../../legend/LegendService.h"
#include <marble/AbstractProjection.h>
#include "mapping/projection/ProjectionService.h"
#include <QtWidgets/QMessageBox>
#include "qmodule/QModule.h"

using namespace calenhad::controls::globe;
using namespace calenhad::legend;
using namespace calenhad::mapping::projection;

CalenhadGlobeConfigDialog::CalenhadGlobeConfigDialog (CalenhadGlobeDialog* parent) : QDialog (), _parent (parent) {
    setLayout (new QVBoxLayout ());

    QTabWidget* tabs = new QTabWidget (this);
    QWidget* widgetsTab = new QWidget (tabs);
    widgetsTab->setLayout (new QFormLayout (widgetsTab));
    tabs->addTab (widgetsTab, "&Widgets");
    _overviewCheck = new QCheckBox (widgetsTab);
    ((QFormLayout*) widgetsTab->layout ())->addRow ("Overview map", _overviewCheck);
    _scaleCheck = new QCheckBox (widgetsTab);
    ((QFormLayout*) widgetsTab->layout ())->addRow ("Scale bar", _scaleCheck);
    _zoomBarCheck = new QCheckBox (widgetsTab);
    ((QFormLayout*) widgetsTab->layout ())->addRow ("Zoom bar", _zoomBarCheck);
    _compassCheck = new QCheckBox (widgetsTab);
    ((QFormLayout*) widgetsTab->layout ())->addRow ("Navigator", _compassCheck);
    _graticuleCheck = new QCheckBox (widgetsTab);
    ((QFormLayout*) widgetsTab->layout ())->addRow ("Graticule", _graticuleCheck);
    _tooltipOptionCombo = new QComboBox (widgetsTab);
    _tooltipOptionCombo -> addItem ("None", CoordinatesFormat::NoCoordinates);
    _tooltipOptionCombo -> addItem ("Decimal", CoordinatesFormat::Decimal);
    _tooltipOptionCombo -> addItem ("Traditional", CoordinatesFormat::Traditional);
    ((QFormLayout*) widgetsTab -> layout()) -> addRow ("Coordinates format", _tooltipOptionCombo);
    _tooltipDatumCombo = new QComboBox (widgetsTab);
    _tooltipDatumCombo -> addItem ("None", DatumFormat::NoDatum);
    _tooltipDatumCombo -> addItem ("Native", DatumFormat::Native);
    _tooltipDatumCombo -> addItem ("Scaled", DatumFormat::Scaled);
    ((QFormLayout*) widgetsTab -> layout()) -> addRow ("Datum format", _tooltipDatumCombo);
    connect (_tooltipOptionCombo, &QComboBox::currentTextChanged, this, [=] () {
        _tooltipDatumCombo-> setEnabled (! (_tooltipOptionCombo -> currentText() == "None"));
    });

    QWidget* mouseTab = new QWidget (tabs);
    mouseTab->setLayout (new QFormLayout (mouseTab));
    tabs->addTab (mouseTab, "&Mouse");

    _doubleClickModeCombo = new QComboBox (mouseTab);
    _doubleClickModeCombo->addItem ("Do nothing");
    _doubleClickModeCombo->addItem ("Go to");
    _doubleClickModeCombo->addItem ("Describe place");
    ((QFormLayout*) mouseTab->layout ())->addRow ("Mouse double-click action", _doubleClickModeCombo);


    _dragModeCombo = new QComboBox (mouseTab);
    _dragModeCombo->addItem ("Do nothing");
    _dragModeCombo->addItem ("Pan");
    _dragModeCombo->addItem ("Zoom");
    ((QFormLayout*) mouseTab->layout ())->addRow ("Mouse drag action", _dragModeCombo);

    _mouseSensitivitySlider = new QwtSlider (mouseTab);
    _mouseSensitivitySlider->setGroove (true);
    _mouseSensitivitySlider->setTrough (false);
    _mouseSensitivitySlider->setLowerBound (0.0);
    _mouseSensitivitySlider->setUpperBound (1.0);
    _mouseSensitivitySlider->setSingleSteps (100);
    _mouseSensitivitySlider->setPageSteps (10);
    _mouseSensitivitySlider->setOrientation (Qt::Horizontal);
    ((QFormLayout*) mouseTab->layout ())->addRow ("Sensitivity", _mouseSensitivitySlider);

    // disable the sensitivity parameter if there is no mouse dragging
    connect (_dragModeCombo, &QComboBox::currentTextChanged, this, [=] () {
        _mouseSensitivitySlider->setEnabled (_dragModeCombo->currentText () != "NoDrag");
    });

    _legendManager = new LegendManager (this);
    _legendTab = new QWidget (tabs);
    tabs -> addTab (_legendTab, "Legend");
    _legendTab-> setLayout (new  QVBoxLayout());
    _legendTab -> layout() -> addWidget (_legendManager);


    QWidget* projectionTab = new QWidget (tabs);
    projectionTab->setLayout (new QFormLayout (projectionTab));
    tabs->addTab (projectionTab, "&Projection");

    _projectionCombo = new QComboBox (projectionTab);
    QMap<QString, Projection*> m = CalenhadServices::projections() -> all();
    for (QString name : m.keys ()) {
        _projectionCombo->addItem (name);
        if (parent -> globe() -> projection () == m.value (name)) {
            _projectionCombo->setCurrentText (name);
        }
    }
    ((QFormLayout*) projectionTab->layout ())->addRow ("Projection", _projectionCombo);

    CalenhadStatsTab* statsTab = new CalenhadStatsTab (_parent);
    tabs -> addTab (statsTab, "&Statistics");

    layout ()->addWidget (tabs);
    QDialogButtonBox* buttonBox = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect (buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect (buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout ()->addWidget (buttonBox);

}

void CalenhadGlobeConfigDialog::initialise() {
    CalenhadServices::legends() -> setDirty (false);
    _mouseSensitivitySlider -> setValue (_parent -> globe() -> sensitivity());
    _dragModeCombo -> setCurrentText ("Do nothing");
    _doubleClickModeCombo -> setCurrentText ("Do nothing");
    _mouseSensitivitySlider -> setEnabled (false);
    if (_parent -> globe() -> mouseDragMode() == CalenhadGlobeDragMode::Pan) { _dragModeCombo -> setCurrentText ("Pan"); _mouseSensitivitySlider -> setEnabled (true); }
    if (_parent -> globe() -> mouseDragMode() == CalenhadGlobeDragMode::Zoom) { _dragModeCombo -> setCurrentText ("Zoom"); _mouseSensitivitySlider -> setEnabled (true); }
    if (_parent -> globe() -> mouseDoubleClickMode() == CalenhadGlobeDoubleClickMode::Goto) { _doubleClickModeCombo -> setCurrentText ("Go to"); _mouseSensitivitySlider -> setEnabled (true); }
    if (_parent -> globe() -> mouseDoubleClickMode() == CalenhadGlobeDoubleClickMode::Place) { _doubleClickModeCombo -> setCurrentText ("Describe place"); _mouseSensitivitySlider -> setEnabled (true); }
    _overviewCheck -> setChecked (_parent -> isOverviewVisible ());
    _scaleCheck -> setChecked (_parent -> isScaleVisible());
    _zoomBarCheck -> setChecked (_parent -> isZoomBarVisible());
    _compassCheck -> setChecked (_parent->isNavigatorVisible ());
    _graticuleCheck -> setChecked (_parent -> isGraticuleVisible());
    _legendManager -> setCurrentLegend (_parent -> globe() -> source () -> legend());
    switch (_parent -> globe() -> coordinatesFormat()) {
        case (CoordinatesFormat::NoCoordinates) : { _tooltipOptionCombo -> setCurrentText ("None"); break; }
        case (CoordinatesFormat::Decimal) : { _tooltipOptionCombo -> setCurrentText ("Decimal"); break; }
        case (CoordinatesFormat::Traditional) : { _tooltipOptionCombo -> setCurrentText ("Traditional"); break; }
    }
    switch (_parent -> globe() -> datumFormat()) {
        case (DatumFormat::NoDatum) : { _tooltipOptionCombo -> setCurrentText ("None"); break; }
        case (DatumFormat::Native) : { _tooltipOptionCombo -> setCurrentText ("Native"); break; }
        case (DatumFormat::Scaled) : { _tooltipOptionCombo -> setCurrentText ("Scale"); break; }
    }
}

CalenhadGlobeConfigDialog::~CalenhadGlobeConfigDialog () {

}

bool CalenhadGlobeConfigDialog::overviewCheckState() { return _overviewCheck -> isChecked(); }
bool CalenhadGlobeConfigDialog::scaleCheckState() { return _scaleCheck -> isChecked(); }
bool CalenhadGlobeConfigDialog::zoomBarCheckState() { return _zoomBarCheck -> isChecked(); }
bool CalenhadGlobeConfigDialog::compassCheckState () { return _compassCheck -> isChecked(); }
bool CalenhadGlobeConfigDialog::graticuleCheckState () { return _graticuleCheck -> isChecked(); }
double CalenhadGlobeConfigDialog::mouseSensitivity() { return _mouseSensitivitySlider -> value(); }
Projection* CalenhadGlobeConfigDialog::selectedProjection() { return CalenhadServices::projections() -> fetch (_projectionCombo -> currentText()); }

CalenhadGlobeDragMode CalenhadGlobeConfigDialog::dragMode () {
    if (_dragModeCombo->currentText () == "Pan") { return CalenhadGlobeDragMode::Pan; }
    if (_dragModeCombo->currentText () == "Zoom") { return CalenhadGlobeDragMode::Zoom; }
    return CalenhadGlobeDragMode::NoDrag;
}

CalenhadGlobeDoubleClickMode CalenhadGlobeConfigDialog::doubleClickMode () {
    if (_doubleClickModeCombo->currentText () == "Go to") { return CalenhadGlobeDoubleClickMode::Goto; }
    if (_doubleClickModeCombo->currentText () == "Place") { return CalenhadGlobeDoubleClickMode::Place; }
    return CalenhadGlobeDoubleClickMode::NoDoubleClick;
}

Legend* CalenhadGlobeConfigDialog::selectedLegend () {
    return _legendManager -> currentLegend ();
}

void CalenhadGlobeConfigDialog::commitChanges () {
    CalenhadServices::legends() -> commit();
    emit legendChanged (_legendManager -> currentLegend());

}

void CalenhadGlobeConfigDialog::reject() {
    if (CalenhadServices::legends() -> isDirty()) {
        QMessageBox confirm;
        confirm.setText ("This will discard all changes to legends. Are you sure?");
        confirm.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
        confirm.setDefaultButton (QMessageBox::Yes);
        confirm.setEscapeButton (QMessageBox::No);
        int result = confirm.exec ();
        if (result == QMessageBox::Yes) {
            CalenhadServices::legends() -> rollback();
            QDialog::reject();
        }
    } else {
        QDialog::reject();
    }
}

CoordinatesFormat CalenhadGlobeConfigDialog::coordinatesFormat () {
    return _tooltipOptionCombo -> currentData().value<CoordinatesFormat>();
}

DatumFormat CalenhadGlobeConfigDialog::datumFormat() {
    return _tooltipDatumCombo -> currentData().value<DatumFormat>();
}