//
// Created by martin on 20/12/17.
//

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QDialog>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialogButtonBox>
#include "ProjectPropertiesDialog.h"

using namespace calenhad::nodeedit;
using namespace calenhad::pipeline;

ProjectPropertiesDialog::ProjectPropertiesDialog (CalenhadModel* model) : QDialog(), _model (model) {

    setLayout (new QVBoxLayout());
    QTabWidget* tabs = new QTabWidget();
    layout() -> addWidget (tabs);

    QWidget* panel = new QWidget (this);
    QFormLayout* form = new QFormLayout (this);
    panel -> setLayout (form);
    tabs -> addTab (panel, "File");

    _titleText = new QLineEdit (this);
    _authorText = new QLineEdit (this);
    _dateLabel = new QLabel (this);
    _filenameLabel = new QLabel (this);
    _descriptionBox = new QTextEdit();

    _titleText -> setText (_model -> title());
    _authorText -> setText (_model -> author());
    _dateLabel -> setText (_model -> lastSave().toString());
    _filenameLabel -> setText (_model -> filename());
    _descriptionBox-> setText (_model -> description());

    form -> addRow ("Project title", _titleText);
    form -> addRow ("Author", _authorText);
    form -> addRow ("Filename", _filenameLabel);
    form -> addRow ("Last updated", _dateLabel);

    QWidget* descriptionPanel = new QWidget (this);
    descriptionPanel -> setLayout (new QVBoxLayout());
    descriptionPanel -> layout() -> addWidget (_descriptionBox);
    tabs -> addTab (descriptionPanel, "Description");

    QDialogButtonBox* buttonBox = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect (buttonBox, &QDialogButtonBox::accepted, this, &ProjectPropertiesDialog::accept);
    connect (buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout() -> addWidget (buttonBox);

}

ProjectPropertiesDialog::~ProjectPropertiesDialog() {

}

void ProjectPropertiesDialog::accept() {
    QDialog::accept();
    _model -> setTitle (_titleText -> text());
    _model -> setAuthor (_authorText -> text());
    _model -> setDescription (_descriptionBox -> toPlainText());
}