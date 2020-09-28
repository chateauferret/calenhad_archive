//
// Created by ferret on 04/06/2020.
//

#ifndef MESSAGES_CALENHADUI_H
#define MESSAGES_CALENHADUI_H


#include <QtWidgets/QStyleOption>

#include <QMenu>
#include <QtWidgets/QGraphicsItem>
#include <src/pipeline/CalenhadModel.h>

namespace calenhad {
    namespace nodeedit {
        class Toolbox;
        class CalenhadController;
    }

    class CalenhadUi : public QObject {
    Q_OBJECT



    public:
        explicit CalenhadUi (nodeedit::CalenhadController* controller);
        ~CalenhadUi() override;
        void connectView(nodeedit::CalenhadView *view);
        QMenuBar* getMenuBar();
        void makeRecentFilesMenu();

        QMenu *makeContextMenu(QGraphicsItem *item);
        void clearTools();
        void updateZoomActions();
        void initialise();
    public slots:
        void setEditActionStatus (pipeline::CalenhadModel *model);

    private:
        nodeedit::CalenhadController* _controller;
        QToolBar * _viewToolbar, * _fileToolbar, * _modulesToolbar, * _editToolbar;
        void setActive (QWidget* widget, const bool& enabled);
        QMenuBar* _menuBar;
        QMenu* _applicationMenu, * _mouseModeMenu, * _viewMenu, * _moduleMenu, * _editMenu, * _fileMenu, * _contextMenu, * _windowMenu, _helpMenu;
        nodeedit::Calenhad* _app;
        QAction* createAction (const QIcon& icon, const QString& name, const QString& statusTip, const QKeySequence& shortcut = QKeySequence());

        QActionGroup* _mouseModeGroup, * _moduleGroup;

        QMenu *getApplicatonMenu();

        QToolBar * makeToolbar (const QString& name);

        void makeActions();

        QMenu* _defaultContextMenu, * _openRecentMenu;


        void makeWidgets();

        QAction* _newAction = nullptr;
        QAction* _openAction = nullptr;
        QAction* _saveAction = nullptr;
        QAction* _saveAsAction = nullptr;
        QAction* _closeAction = nullptr;
        QAction* _loadLegendsAction = nullptr;
        QAction* _saveLegendsAction = nullptr;
        QAction* _projectPropertiesAction = nullptr;
        QAction* _xmlAction = nullptr;
        QAction* _quitAction = nullptr;
        QAction* _cutAction = nullptr;
        QAction* _copyAction = nullptr;
        QAction* _pasteAction = nullptr;
        QAction* _moduleTreeAction = nullptr;
        QAction* _selectModeAction = nullptr;
        QAction* _panModeAction = nullptr;
        QAction* _undoAction = nullptr;
        QAction* _redoAction = nullptr;
        QAction* _manageLegendsAction = nullptr;
        QAction* _zoomInAction = nullptr;
        QAction* _zoomOutAction = nullptr;
        QAction* _zoomToFitAction = nullptr;
        QAction* _zoomSelectionAction = nullptr;
        QAction* _deleteAction = nullptr;
        QAction* _variablesAction = nullptr;
        QAction* _gridAction = nullptr;
        QAction* _snapAction = nullptr;
        QAction *_importAction = nullptr;
        QAction* _newWindowAction = nullptr;
        QAction* _closeWindowAction = nullptr;
        QAction* _tileWindowsAction = nullptr;
        QAction* _cascadeWindowsAction = nullptr;
        QMenu* _windowSwitchMenu = nullptr;
        QMenu *_toolbarsMenu;
    };

}
#endif //MESSAGES_CALENHADUI_H
