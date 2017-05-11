//
// Created by martin on 10/05/17.
//

#include <QtWidgets/QTabWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QFormLayout>
#include <QtWidgets/QDialogButtonBox>
#include "CalenhadGlobeConfigDialog.h"



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

    QWidget* mouseTab = new QWidget (tabs);
    tabs -> addTab (mouseTab, "&Mouse");

    mouseTab -> setLayout (new QFormLayout (mouseTab));
    _mouseModeCombo = new QComboBox (mouseTab);
    _mouseModeCombo -> addItem ("Disabled");
    _mouseModeCombo -> addItem ("Pan");
    _mouseModeCombo -> addItem ("Zoom");
    ((QFormLayout*) mouseTab -> layout()) -> addRow ("Mouse mode", _mouseModeCombo);

    _mouseSensitivitySlider = new QwtSlider (mouseTab);
    _mouseSensitivitySlider -> setGroove (true);
    _mouseSensitivitySlider -> setTrough (false);
    _mouseSensitivitySlider -> setLowerBound (0.0);
    _mouseSensitivitySlider -> setUpperBound (1.0);
    _mouseSensitivitySlider -> setSingleSteps (100);
    _mouseSensitivitySlider -> setPageSteps (10);
    _mouseSensitivitySlider -> setOrientation (Qt::Horizontal);
    ((QFormLayout*) mouseTab -> layout()) -> addRow ("Sensitivity", _mouseSensitivitySlider);

    connect (_mouseModeCombo, &QComboBox::currentTextChanged, this, [=] () { _mouseSensitivitySlider -> setEnabled ( _mouseModeCombo -> currentText() != "Disabled"); });

    QWidget* legendTab = new QWidget (tabs);
    tabs -> addTab (legendTab, "&Legend");

    layout() -> addWidget (tabs);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout() -> addWidget (buttonBox);

}

void CalenhadGlobeConfigDialog::initialise() {
    _mouseSensitivitySlider -> setValue (_parent -> sensitivity());
    _mouseModeCombo -> setCurrentText ("Disabled");
    _mouseSensitivitySlider -> setEnabled (false);
    if (_parent -> mouseMode() == CalenhadGlobeMouseMode::Pan) { _mouseModeCombo -> setCurrentText ("Pan"); _mouseSensitivitySlider -> setEnabled (true); }
    if (_parent -> mouseMode() == CalenhadGlobeMouseMode::Zoom) { _mouseModeCombo -> setCurrentText ("Zoom"); _mouseSensitivitySlider -> setEnabled (true); }
    _overviewCheck -> setChecked (_parent -> isOverviewVisible());
    _scaleCheck -> setChecked (_parent -> isFloatItemVisible ("scalebar"));
    _zoomBarCheck -> setChecked (_parent -> isZoomBarVisible());
    _compassCheck -> setChecked (_parent -> isCompassVisible());

}


CalenhadGlobeConfigDialog::~CalenhadGlobeConfigDialog () {

}

bool CalenhadGlobeConfigDialog::overviewCheckState() { return _overviewCheck -> isChecked(); }
bool CalenhadGlobeConfigDialog::scaleCheckState() { return _scaleCheck -> isChecked(); }
bool CalenhadGlobeConfigDialog::zoomBarCheckState() { return _zoomBarCheck -> isChecked(); }
bool CalenhadGlobeConfigDialog::compassCheckState () { return _compassCheck -> isChecked(); }
double CalenhadGlobeConfigDialog::mouseSensitivity() { return _mouseSensitivitySlider -> value(); }

CalenhadGlobeMouseMode CalenhadGlobeConfigDialog::mouseMode() {
    if (_mouseModeCombo->currentText () == "Pan") { return CalenhadGlobeMouseMode::Pan; }
    if (_mouseModeCombo->currentText () == "Zoom") { return CalenhadGlobeMouseMode::Zoom; }
    return CalenhadGlobeMouseMode::Disabled;
}


