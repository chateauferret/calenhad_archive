//
// Created by martin on 15/12/17.
//

#ifndef CALENHAD_SPLASHDIALOG_H
#define CALENHAD_SPLASHDIALOG_H


#include <QtWidgets/QDialog>
#include <QtWidgets/QComboBox>

namespace calenhad {
    namespace nodeedit{
        class Calenhad;
    }

    namespace controls {
        class SplashDialog : public QDialog {
        Q_OBJECT
        public:
            SplashDialog (calenhad::nodeedit::Calenhad* parent);
            virtual ~SplashDialog();
            QString fileSelected();

        public slots:
            void optionSelected();

        signals:
            void openProject (const QString& filenmae);
            void closeCalenhad();
            void newProject();

        protected:
            QPushButton* _openButton, * _closeButton, * _newProjectButton, * _openRecentButton, * _cancelButton;
            QComboBox* _recentFileBox;
            QStringList _recentFiles;
            QString _fileSelected;
            void showEvent (QShowEvent* e) override;
        };
    }
}


#endif //CALENHAD_SPLASHDIALOG_H
