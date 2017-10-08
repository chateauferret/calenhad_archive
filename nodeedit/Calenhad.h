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
#include "../messages/QNotificationHost.h"
#include "qnetoolbox.h"

namespace calenhad {
    namespace controls {
        class CalenhadLegendDialog;
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
    namespace qmodule {
        class QNode;
    }
    namespace nodeedit {
        class CalenhadController;
        class CalenhadView;
        class QNEToolBox;

        enum CalenhadFileType { CalenhadModelFile, CalenhadLegendFile };

        class Calenhad : public calenhad::notification::QNotificationHost {
        Q_OBJECT

        public:
            explicit Calenhad (QWidget* parent = 0);

            ~Calenhad ();

            QNEToolBox* _toolbox;

            void setModel (calenhad::pipeline::CalenhadModel* model);

            calenhad::pipeline::CalenhadModel* model ();

            void initialiseLegends ();

            void addToolbar (QToolBar* toolbar, calenhad::qmodule::QNode* node);

            CalenhadController* controller ();

            void clearTools ();
            void setSelectionActionsEnabled (const bool& enabled);

        private slots:

            void saveFile();
            void saveFileAs (const CalenhadFileType& fileType);
            void loadFile (const CalenhadFileType& fileType = calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);


            void closeEvent (QCloseEvent* event);

        private:
            CalenhadController* _controller;
            CalenhadView* _view;
            calenhad::pipeline::CalenhadModel* _model;
            calenhad::expressions::VariablesDialog* _variablesDialog;
            QString _lastFile;
            QMap<QString, calenhad::legend::Legend*> _legends;

            //void readMetadata (const QDomDocument& doc, QNotificationFactory* messages);

            bool readXml (const QString& fname, QDomDocument& doc);

            calenhad::controls::CalenhadLegendDialog* _legendDialog;

            void resizeEvent (QResizeEvent* event) override;

            void moveEvent (QMoveEvent* event);


            ToolDrawer* _viewDrawer;
            ToolDrawer* _editDrawer;

            ToolDrawer* _addModuleDrawer;
            ToolGroup* _addModuleGroup;



            QAction* addModuleTool (const QString& name, const QString& tooltip);

            QMenu* _moduleContextMenu;
            QMenu* _connectionContextMenu;
            QMenu* _outputPortContextMenu;
            QMenu* _inputPortContextMenu;
            QMenu* _defaultContextMenu;
            QMenu* _zoomMenu;
            QMenu* _addModuleMenu;
            //void makeContextMenus ();

            QAction* undoAction, * redoAction;
            QAction* zoomInAction;
            QAction* zoomOutAction;
            QAction* zoomToFitAction;
            QAction* zoomSelectionAction;
            QAction* deleteConnectionAction;
            QAction* deleteModuleAction;
            QAction* deleteSelectionAction;
            QAction* duplicateModuleAction;

            QAction* createTool (const QIcon& icon, const QString& name, const QString& statusTip, const QVariant& id, ToolDrawer* drawer, const bool& toggle = false, const QKeySequence& shortcut = QKeySequence());

            QWidget* _nodeRoster;
            void addMenus (QMenuBar* menuBar);


            void updateZoomActions ();

            CalenhadToolBar* makeToolbar (const QString& name);

            QAction* createAction (const QIcon& icon, const QString& name, const QString statusTip, const QKeySequence& shortcut = QKeySequence());

        };
    }
}

#endif // QNEMAINWINDOW_H
