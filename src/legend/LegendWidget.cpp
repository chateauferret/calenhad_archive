//
// Created by martin on 31/05/17.
//

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QTextEdit>
#include "Legend.h"
#include "LegendService.h"
#include "LegendWidget.h"
#include "../controls/legend/LegendEditor.h"
#include "src/CalenhadServices.h"
#include "../messages/QNotificationHost.h"

using namespace calenhad::notification;
using namespace calenhad::legend;
using namespace calenhad::controls::legend;

LegendWidget::LegendWidget (Legend* legend, QWidget* parent) : QWidget (parent), _legend (legend) {
    setLayout (new QVBoxLayout ());

   _legendNameBox = new QLineEdit (this);
    layout ()->addWidget (_legendNameBox);
    // if the text box contains a changed name that's the same as that for another Legend, warn the user
    connect (_legendNameBox, &QLineEdit::textChanged, this, &LegendWidget::checkName);

    // when we finish updating the name, change the name in the LegendService, unless it exists already
    connect (_legendNameBox, &QLineEdit::editingFinished, this, &LegendWidget::updateName);

    _legendEditor = new LegendEditor (_legend, this);
    layout ()->addWidget (_legendEditor);
    connect (_legendEditor, &LegendEditor::legendChanged, this, [=] () {
        emit iconChanged (_legend -> icon());
        emit legendChanged (_legend -> entries());
    });

    _legendInterpolateCheck = new QCheckBox (this);
    _legendInterpolateCheck->setText ("Interpolate colours");

    layout ()->addWidget (_legendInterpolateCheck);
    connect (_legendInterpolateCheck, &QAbstractButton::toggled, this, [=] () {
        _legend -> setInterpolated (_legendInterpolateCheck -> isChecked());
        _legendEditor -> update();
        emit iconChanged (_legend -> icon());
        emit legendChanged (_legend -> entries());
    });

    _legendNotesBox = new QTextEdit (this);
    layout ()->addWidget (_legendNotesBox);
    connect (_legendNotesBox, &QTextEdit::textChanged, this, [=] () {
        _legend -> setNotes (_legendNotesBox -> toPlainText());
        emit notesChanged (_legendNotesBox -> toPlainText());
    });

    populate();
}

LegendWidget::~LegendWidget () {

}

void LegendWidget::populate() {
    _legendNameBox->setText (_legend->name ());
    _legendInterpolateCheck->setChecked (_legend->isInterpolated ());
    _legendNotesBox->setText (_legend->notes ());
}

void LegendWidget::showEvent (QShowEvent* event) {
   populate();
}

void LegendWidget::updateLegend() {
    if (_legendNameBox -> text() != _legend -> name()) {
        _legend->setName (_legendNameBox -> text());
    }
    _legend -> setNotes (_legendNotesBox -> toPlainText());
    _legend -> setEntries (_legendEditor -> legend() -> entries());
    _legend -> setInterpolated (_legendInterpolateCheck -> isChecked());
}

Legend* LegendWidget::legend() {
    return _legend;
}


void LegendWidget::checkName() {

    if (CalenhadServices::legends() -> exists (_legendNameBox -> text()) && _legendNameBox -> text() != _legend -> name()) {
        QPalette palette;
        palette.setColor (QPalette::Base, Qt::red);
        _legendNameBox->setPalette (palette);
        _legendNameBox -> setToolTip ("Legend with this name already exists");
    } else {
        QPalette palette;
        palette.setColor (QPalette::Base, Qt::white);
        _legendNameBox -> setPalette (palette);
        _legendNameBox -> setToolTip (QString::null);
    }
}

void LegendWidget::updateName() {
    if (_legendNameBox->text () != _legend -> name()) {
        if (! CalenhadServices::legends() ->  exists (_legendNameBox->text ())) {
            _legend -> setName (_legendNameBox->text ());
            emit nameChanged (_legend -> name());
        } else {
            CalenhadServices::messages ()->message ("Duplicate legend", "Cannot change legend name. Legend with name " + _legendNameBox->text () + " already exists", NotificationStyle::WarningNotification);
            _legendNameBox->setText (_legend -> name());
        }
    }
}

QVector<LegendEntry> LegendWidget::entries () {
    return _legend -> entries();
}

void LegendWidget::focusNameField () {
    _legendNameBox -> setFocus();
    _legendNameBox -> selectAll();
}

const bool& LegendWidget::isInterpolated () {
    return _legend -> isInterpolated();
}
