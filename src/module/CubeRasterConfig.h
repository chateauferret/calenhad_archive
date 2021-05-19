//
// Created by ferret on 20/01/2021.
//

#ifndef CALENHAD_CUBERASTERCONFIG_H
#define CALENHAD_CUBERASTERCONFIG_H

#include <QAbstractTableModel>
namespace calenhad {
    namespace module {
        class CubeRasterFile;
        class CubeRasterConfig : public QAbstractTableModel {
        Q_OBJECT
        public:
            CubeRasterConfig ();

            ~CubeRasterConfig () override;

            int columnCount (const QModelIndex& parent) const override;

            int rowCount (const QModelIndex& parent) const override;

            QVariant data (const QModelIndex& index, int role) const override;
            bool setData (const QModelIndex &index, const QVariant &value, int role) override;
            QVariant headerData (int section, Qt::Orientation orientation, int role) const override;
            Qt::ItemFlags flags (const QModelIndex &index) const override;

        signals:
            void configChanged();

        protected:
            CubeRasterFile* _files[6];


            void setRotation (const int& index, const int& rotation);

            void setReflection (const int& index, const bool& flipHorizontal, const bool& flipVertical);

            QString filename (const int& index);

            int rotation (const int& index);
        };
    }
}

#endif //CALENHAD_CUBERASTERCONFIG_H
