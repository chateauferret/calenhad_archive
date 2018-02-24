//
// Created by martin on 15/12/17.
//

#include "SplashDialog.h"
#include <QPushButton>
#include <QtWidgets/QFormLayout>
#include <nodeedit/Calenhad.h>
#include <QtCore/QFile>
#include <QtWidgets/QFileDialog>
#include <QSet>
#include <iostream>

using namespace calenhad::controls;
using namespace calenhad::nodeedit;

SplashDialog::SplashDialog (Calenhad* parent) {
    setLayout (new QVBoxLayout());
    QWidget* panel = new QWidget (this);
    QFormLayout* form = new QFormLayout (this);
    panel -> setLayout (form);
    layout() -> addWidget (panel);

    _openButton = new QPushButton ();
    _openButton -> setText ("Open");

    _newProjectButton = new QPushButton ();
    _newProjectButton -> setText ("New project");

    _recentFileBox = new QComboBox();
    _recentFiles = parent -> recentFiles();

    _openRecentButton = new QPushButton ();
    _openRecentButton -> setText ("Open");

    _closeButton = new QPushButton ();
    _closeButton -> setText ("Close");

    _cancelButton = new QPushButton ();
    _cancelButton -> setText ("Cancel");

    form -> addRow ("Open an existing project", _openButton);
    form -> addRow ("Start a new project", _newProjectButton);
    form -> addRow (_recentFileBox, _openRecentButton);
    form -> addRow ("Exit from Calenhad", _closeButton);
    form -> addRow ("Cancel", _cancelButton);

    connect (_openButton, &QPushButton::pressed, this, &SplashDialog::optionSelected);
    connect (_openRecentButton, &QPushButton::pressed, this, &SplashDialog::optionSelected);
    connect (_newProjectButton, &QPushButton::pressed, this, &SplashDialog::optionSelected);
    connect (_closeButton, &QPushButton::pressed, this, &SplashDialog::optionSelected);
    connect (_cancelButton, &QPushButton::pressed, this, &SplashDialog::optionSelected);
}

SplashDialog::~SplashDialog () {

}

void SplashDialog::showEvent (QShowEvent* e) {
    _recentFileBox -> clear();
    _recentFiles = ((Calenhad*) parent()) -> recentFiles();
    for (QString entry : _recentFiles) {
        QFile f (entry);
        QDomDocument doc;
        doc.setContent (&f);
        QDomElement metadataElement = doc.documentElement ().firstChildElement ("metadata");
        QDomElement titleElement = metadataElement.firstChildElement ("title");
        QString title = titleElement.text ();
        title += " (" + entry + ")";
        _recentFileBox -> addItem (title, entry);
    }
}

void SplashDialog::optionSelected() {
    if (sender() == _openButton) {
        _fileSelected = QFileDialog::getOpenFileName (this, "Open project", "/home/martin", "Calenhad project files (*.chp *.xml)");
        emit openProject (_fileSelected);
    }
    if (sender() == _openRecentButton) {
        int item = _recentFileBox -> currentIndex ();
        _fileSelected = _recentFileBox -> itemData (item).toString();
        emit openProject (_fileSelected);
    }
    if (sender() == _newProjectButton) {
        emit newProject();
    }
    if (sender() == _closeButton) {
        emit closeCalenhad();
    }
    if (sender() == _cancelButton) {
        close();
    }
}

QString SplashDialog::fileSelected () {
    return _fileSelected;
}
