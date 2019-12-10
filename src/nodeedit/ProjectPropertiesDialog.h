//
// Created by martin on 20/12/17.
//

#ifndef CALENHAD_PROJECTPROPERTIESDIALOG_H
#define CALENHAD_PROJECTPROPERTIESDIALOG_H

#include <pipeline/CalenhadModel.h>

namespace calenhad {
    namespace pipeline {
        class CalenhadModel;
    }
    namespace nodeedit {
        class ProjectPropertiesDialog : public QDialog {
            Q_OBJECT

        public:
            ProjectPropertiesDialog (calenhad::pipeline::CalenhadModel* model);
            virtual ~ProjectPropertiesDialog();


        protected:
            calenhad::pipeline::CalenhadModel* _model;
            QLineEdit* _titleText, * _authorText;
            QLabel* _filenameLabel, * _dateLabel;
            QTextEdit* _descriptionBox;

        protected slots:
            void accept() override;
        };
    }
}


#endif //CALENHAD_PROJECTPROPERTIESDIALOG_H
