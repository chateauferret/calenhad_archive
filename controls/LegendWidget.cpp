//
// Created by martin on 31/05/17.
//

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QTextEdit>
#include "LegendWidget.h"
#include "LegendEditor.h"
#include "../CalenhadServices.h"


LegendWidget::LegendWidget (QWidget* parent) : QWidget (parent)  {
    setLayout (new QVBoxLayout ());

   _legendNameBox = new QLineEdit (this);
    layout ()->addWidget (_legendNameBox);

    // if the text box contains a changed name that's the same as that for another Legend, warn the user
    connect (_legendNameBox, &QLineEdit::textChanged, this, [=] () {
        if (CalenhadServices::legends() -> exists (_legendNameBox -> text()) && _legendNameBox -> text() != _name) {
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
    });

    // when we finish updating the name, change the name in the LegendService, unless it exists already
    connect (_legendNameBox, &QLineEdit::editingFinished, this, [=] () {
            if (_legendNameBox -> text() != _name) {
                if (!CalenhadServices::legends ()->exists (_legendNameBox->text ())) {
                    _name = _legendNameBox->text ();
                    emit nameChanged (_name);
                } else {
                    CalenhadServices::messages ()->message ("Cannot change legend name", "Legend with name " + _legendNameBox->text () + " already exists");
                    _legendNameBox->setText (_name);
                }
            }
    });

    _legendEditor = new LegendEditor (this);
    layout ()->addWidget (_legendEditor);
    connect (_legendEditor, &LegendEditor::legendChanged, this, [=] () {
        _entries = _legendEditor -> legend();
        emit iconChanged (icon());});

    _legendInterpolateCheck = new QCheckBox (this);
    _legendInterpolateCheck->setText ("Interpolate colours");

    layout ()->addWidget (_legendInterpolateCheck);
    connect (_legendInterpolateCheck, &QAbstractButton::toggled, this, [=] () {
        _interpolate = _legendInterpolateCheck ->isChecked();
        emit iconChanged (icon());
    });

    _legendNotesBox = new QTextEdit (this);
    layout ()->addWidget (_legendNotesBox);
    connect (_legendNotesBox, &QTextEdit::textChanged, this, [=] () {
        _notes = ((QTextEdit*) sender()) -> toPlainText();
    });
}

LegendWidget::~LegendWidget () {

}

void LegendWidget::showEvent (QShowEvent* event) {
    _legendNameBox -> setText (_name);
    _legendEditor -> setLegend (_entries);
    _legendInterpolateCheck -> setChecked (_interpolate);
    _legendNotesBox -> setText (_notes);
}

void LegendWidget::showLegend (Legend* legend) {
    _name = legend -> name();
    _notes = legend -> notes();
    _entries = legend -> entries();
    _interpolate = legend -> isInterpolated();
}

void LegendWidget::updateLegend (Legend* legend) {
    if (_name != legend -> name()) {
        legend->setName (_name);
    }
    legend -> setNotes (_notes);
    legend -> setEntries (_entries);
    legend -> setInterpolated (_interpolate);
}


QIcon LegendWidget::icon () {
    QPixmap pixmap (150, 30);

    QLinearGradient grad;
    QRect crec = pixmap.rect ();
    grad = QLinearGradient (0, 0, crec.width (), 0);

    if (! _interpolate) {
        std::cout << "Not interpolated\n";
        double index = 0.0;
        QColor lastColor;
        for (LegendEntry entry : _entries) {
            grad.setColorAt (index - 1e-10, lastColor);
            index = entry.first / 2 + 0.5;
            grad.setColorAt (index, entry.second);
            lastColor = entry.second;
        }

    } else {

        std::cout << "Interpolated\n";
        for (LegendEntry entry : _entries) {
            double index = entry.first / 2 + 0.5;
            grad.setColorAt (index, entry.second);
        }
    }
    QBrush brush = QBrush (grad);
    QPainter painter (&pixmap);
    painter.setBrush (brush);
    painter.drawRect (crec);
    QIcon icon (pixmap);
    return icon;

}

QString LegendWidget::name() {
    return _name;
}