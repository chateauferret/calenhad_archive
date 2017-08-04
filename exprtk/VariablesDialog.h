//
// Created by martin on 20/07/17.
//

#ifndef CALENHAD_VARIABLESDIALOG_H
#define CALENHAD_VARIABLESDIALOG_H

#include <QtWidgets/QDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDialogButtonBox>
#include "CalculatorService.h"

namespace calenhad {
    namespace expressions {

        class VariablesDialog : public QDialog {
        Q_OBJECT

        public:
            VariablesDialog ();

            virtual ~VariablesDialog ();

        public slots:

            void deleteSelected ();

            void insertItem ();

            void selectionChanged ();

            void rollback ();

            void commit ();

            void validateContent ();

        protected:
            QTableWidget* _table;
            QPushButton* _deleteButton, * _insertButton, * _rollbackButton;;

            void resizeEvent (QResizeEvent* e) override;

            double value (QTableWidgetItem* item);

            bool validate (int row);

            QDialogButtonBox* _buttonBox;
            QMap<QString, CalenhadVariable> _oldVariables;

            void showEvent (QShowEvent* e) override;
        };
    }
}


#endif //CALENHAD_VARIABLESDIALOG_H
