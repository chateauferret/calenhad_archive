//
// Created by martin on 10/05/17.
//

#include <QtWidgets/QTabWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QFormLayout>
#include <QtWidgets/QDialogButtonBox>
#include "CalenhadGlobeConfigDialog.h"
#include "../CalenhadServices.h"


CalenhadGlobeConfigDialog::CalenhadGlobeConfigDialog (CalenhadGlobe* parent) : QDialog (), _parent (parent) {
    setLayout (new QVBoxLayout());

    QTabWidget* tabs = new QTabWidget (this);
    QWidget* widgetsTab = new QWidget (tabs);
    widgetsTab -> setLayout (new QFormLayout (widgetsTab));
    tabs -> addTab (widgetsTab, "&Widgets");
    _overviewCheck = new QCheckBox (widgetsTab);
    ((QFormLayout*) widgetsTab ->  layout()) -> addRow ("Overview map", _overviewCheck);
    _scaleCheck = new QCheckBox (widgetsTab);
    ((QFormLayout*) widgetsTab -> layout()) -> addRow ("Scale bar", _scaleCheck);
    _zoomBarCheck = new QCheckBox (widgetsTab);
    ((QFormLayout*) widgetsTab -> layout()) -> addRow ("Zoom bar",_zoomBarCheck);
    _compassCheck = new QCheckBox (widgetsTab);
    ((QFormLayout*) widgetsTab -> layout()) -> addRow ("Navigator", _compassCheck);
    _graticuleCheck = new QCheckBox (widgetsTab);
    ((QFormLayout*) widgetsTab -> layout()) -> addRow ("Graticule", _graticuleCheck);

    QWidget* mouseTab = new QWidget (tabs);
    mouseTab -> setLayout (new QFormLayout (mouseTab));
    tabs -> addTab (mouseTab, "&Mouse");

    _doubleClickModeCombo = new QComboBox (mouseTab);
    _doubleClickModeCombo -> addItem ("Do nothing");
    _doubleClickModeCombo -> addItem ("Go to");
    _doubleClickModeCombo -> addItem ("Describe place");
    ((QFormLayout*) mouseTab -> layout()) -> addRow ("Mouse double-click action", _doubleClickModeCombo);


    _dragModeCombo = new QComboBox (mouseTab);
    _dragModeCombo -> addItem ("Do nothing");
    _dragModeCombo -> addItem ("Pan");
    _dragModeCombo -> addItem ("Zoom");
    ((QFormLayout*) mouseTab -> layout()) -> addRow ("Mouse drag action", _dragModeCombo);

    _mouseSensitivitySlider = new QwtSlider (mouseTab);
    _mouseSensitivitySlider -> setGroove (true);
    _mouseSensitivitySlider -> setTrough (false);
    _mouseSensitivitySlider -> setLowerBound (0.0);
    _mouseSensitivitySlider -> setUpperBound (1.0);
    _mouseSensitivitySlider -> setSingleSteps (100);
    _mouseSensitivitySlider -> setPageSteps (10);
    _mouseSensitivitySlider -> setOrientation (Qt::Horizontal);
    ((QFormLayout*) mouseTab -> layout()) -> addRow ("Sensitivity", _mouseSensitivitySlider);

    // disable the sensitivity parameter if there is no mouse dragging
    connect (_dragModeCombo, &QComboBox::currentTextChanged, this, [=] () { _mouseSensitivitySlider -> setEnabled ( _dragModeCombo -> currentText() != "NoDrag"); });

    QWidget* legendTab = new QWidget (tabs);
    tabs -> addTab (legendTab, "&Legend");

    QWidget* projectionTab = new QWidget (tabs);
    projectionTab -> setLayout (new QFormLayout (projectionTab));
    tabs -> addTab (projectionTab, "&Projection");

    _projectionCombo = new QComboBox (projectionTab);
    QMap<QString, Projection> m = CalenhadServices::projections() -> all();
    for (QString name : m.keys()) {
        _projectionCombo -> addItem (name);
        if (parent -> projection() == m.value (name)) {
            _projectionCombo -> setCurrentText (name);
        }
    }
    ((QFormLayout*) projectionTab -> layout()) -> addRow ("Projection", _projectionCombo);

    layout() -> addWidget (tabs);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout() -> addWidget (buttonBox);

}

void CalenhadGlobeConfigDialog::initialise() {
    _mouseSensitivitySlider -> setValue (_parent -> sensitivity());
    _dragModeCombo -> setCurrentText ("Do nothing");
    _doubleClickModeCombo -> setCurrentText ("Do nothing");
    _mouseSensitivitySlider -> setEnabled (false);
    if (_parent->dragMode() == CalenhadGlobeDragMode::Pan) { _dragModeCombo -> setCurrentText ("Pan"); _mouseSensitivitySlider -> setEnabled (true); }
    if (_parent->dragMode() == CalenhadGlobeDragMode::Zoom) { _dragModeCombo -> setCurrentText ("Zoom"); _mouseSensitivitySlider -> setEnabled (true); }
    if (_parent->doubleClickMode() == CalenhadGlobeDoubleClickMode::Goto) { _doubleClickModeCombo -> setCurrentText ("Go to"); _mouseSensitivitySlider -> setEnabled (true); }
    if (_parent->doubleClickMode() == CalenhadGlobeDoubleClickMode::Place) { _doubleClickModeCombo -> setCurrentText ("Describe place"); _mouseSensitivitySlider -> setEnabled (true); }
    _overviewCheck -> setChecked (_parent -> isOverviewVisible());
    _scaleCheck -> setChecked (_parent -> isFloatItemVisible ("scalebar"));
    _zoomBarCheck -> setChecked (_parent -> isZoomBarVisible());
    _compassCheck -> setChecked (_parent -> isCompassVisible());
    _graticuleCheck -> setChecked (_parent -> isGraticuleVisible());

}


CalenhadGlobeConfigDialog::~CalenhadGlobeConfigDialog () {

}

bool CalenhadGlobeConfigDialog::overviewCheckState() { return _overviewCheck -> isChecked(); }
bool CalenhadGlobeConfigDialog::scaleCheckState() { return _scaleCheck -> isChecked(); }
bool CalenhadGlobeConfigDialog::zoomBarCheckState() { return _zoomBarCheck -> isChecked(); }
bool CalenhadGlobeConfigDialog::compassCheckState () { return _compassCheck -> isChecked(); }
bool CalenhadGlobeConfigDialog::graticuleCheckState () { return _graticuleCheck -> isChecked(); }
double CalenhadGlobeConfigDialog::mouseSensitivity() { return _mouseSensitivitySlider -> value(); }
Projection CalenhadGlobeConfigDialog::selectedProjection() { return CalenhadServices::projections() -> fetch (_projectionCombo -> currentText()); }

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