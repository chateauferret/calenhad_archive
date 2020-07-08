/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef QNEMAINWINDOW_H
#define QNEMAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDockWidget>
#include <QtXml/QDomDocument>
#include <QtCore/QMap>
#include <QtWidgets/QUndoStack>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QScrollArea>
#include "../messages/QNotificationHost.h"
#include <QScrollArea>
#include <src/controls/globe/CalenhadGlobeDialog.h>

namespace calenhad {
    namespace controls {
        class CalenhadLegendDialog;
        class SplashDialog;
        namespace globe {
            class CalenhadGlobeWidget;
        }
    }
    namespace preferences {
        class Preferences;
    }
    namespace pipeline {
        class CalenhadModel;
    }
    namespace legend {
        class Legend;
    }
    namespace expressions {
        class VariablesDialog;
    }
    namespace notifications {
        class QNotificationStack;
    }
    namespace module {
        class Module;
    }
    namespace nodeedit {
        class CalenhadController;
        class CalenhadView;
        class ToolBox;

        enum CalenhadFileType { CalenhadModelFile, CalenhadLegendFile, CalenhadModelFragment };

        class Calenhad : public calenhad::notification::QNotificationHost {
        Q_OBJECT

        public:
            explicit Calenhad (QWidget* parent = 0);

            ~Calenhad () override;

            void setModel (calenhad::pipeline::CalenhadModel* model);

            calenhad::pipeline::CalenhadModel* model ();

            void initialiseLegends ();

            void addToolbar (QToolBar* toolbar, calenhad::module::Module* m);

            CalenhadController* controller ();

            void showSplash();
        public slots:
            void titleChanged (const QString& title);
            void toggleMouseMode();
            void newProject();
            void closeProject();
            void saveFile();
            void saveFileAs (const CalenhadFileType& fileType);
            void loadFile (const CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
            void loadFile (const QString& fname, const CalenhadFileType& fileType);
            void openProject (const QString& fname);
            void open();
            void quit();
            void projectProperties();
            void manageLegends();


        private:
            CalenhadController* _controller;
            CalenhadView* _view;
            calenhad::pipeline::CalenhadModel* _model;
            calenhad::expressions::VariablesDialog* _variablesDialog{};
            QString _lastFile;
            QMap<QString, calenhad::legend::Legend*> _legends;
            calenhad::controls::globe::CalenhadGlobeDialog *_globe{};
            //void readMetadata (const QDomDocument& doc, QNotificationFactory* messages);

            calenhad::controls::CalenhadLegendDialog* _legendDialog;

            void resizeEvent (QResizeEvent* event) override;

            void moveEvent (QMoveEvent* event) override;

            QMenu* _defaultContextMenu{};


            //void makeContextMenus ();

            QAction* selectModeAction{}, * panModeAction{};

            calenhad::controls::SplashDialog* _splash;

            void closeEvent (QCloseEvent* event) override;
            void showEvent (QShowEvent* event) override;

            void clearUndo();



            void provideSplashDialog();

            void showGlobe (calenhad::module::Module* module);

            void provideGlobe (calenhad::module::Module* module);
        };
    }
}

#endif // QNEMAINWINDOW_H
