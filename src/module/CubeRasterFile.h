//
// Created by ferret on 12/10/2020.
//

#ifndef CALENHAD_CUBERASTERFILE_H
#define CALENHAD_CUBERASTERFILE_H
#include <QString>
#include <QImage>
#include <QIcon>
#include <QLabel>

namespace calenhad {
    namespace module {

        class CubeRasterModule;

        class CubeRasterFile : public QObject {
            Q_OBJECT



        public:
            explicit CubeRasterFile (CubeRasterModule* module);
            ~CubeRasterFile() override;

            const QString& getFilename () const;

            void setFilename (const QString& filename);
            void setRotation (int i);
            int getIndex () const;

            void setIndex (int index);

            int getRotation () const;


            QImage* getImage () const;

            void rotateRight ();

            void rotateLeft ();
            QWidget* ui();

        private:
            QString _filename;
            int _index;
            int _rotation;
            QImage* _image;
            QIcon _arrows [4];
            QTransform _rotations [4];
            QWidget* _ui;
            QLabel* _filenameLabel;
            CubeRasterModule* _module;
            QPushButton* _rotationButton;

            void updateUi();
        };
    }
}



#endif //CALENHAD_CUBERASTERFILE_H
