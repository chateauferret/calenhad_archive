//
// Created by ferret on 12/10/2020.
//

#ifndef CALENHAD_CUBERASTERFILE_H
#define CALENHAD_CUBERASTERFILE_H
#include <QString>
#include <QImage>
#include <QIcon>
#include <QLabel>
#include <QPushButton>

namespace calenhad {
    namespace module {

        class CubeRasterModule;

        class CubeRasterFile : public QObject {
            Q_OBJECT



        public:
            explicit CubeRasterFile();
            ~CubeRasterFile() override;

            const QString& getFilename () const;
            QString filename ();
            void setFilename (const QString& filename);
            void setRotation (int i);
            int getIndex () const;

            void setIndex (int index);
            void setEnabled (const bool& enabled);

            int getRotation () const;
            bool isEnabled () const;
            bool isFlippedHorizontal () const;
            bool isFlippedVertical () const;


            QImage* getImage () const;

            void flipHorizontal (const bool& flip);
            void flipVertical (const bool& flip);

        private:
            QString _filename;
            int _index;
            int _rotation;
            QImage* _image;
            QIcon _arrows [4];
            QTransform _rotations [4];
            QWidget* _ui;
            QLabel* _filenameLabel;
            QPushButton* _rotationButton;
            QPushButton* _flipVerticalButton;
            QPushButton* _flipHorizontalButton;

            bool _flipHorizontal, _flipVertical;

            bool _enabled;

            void updateImage ();
        };
    }
}



#endif //CALENHAD_CUBERASTERFILE_H
