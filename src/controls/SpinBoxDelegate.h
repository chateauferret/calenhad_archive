//
// Created by ferret on 01/02/2021.
//

#ifndef CALENHAD_SPINBOXDELEGATE_H
#define CALENHAD_SPINBOXDELEGATE_H

#include <QStyledItemDelegate>

namespace calenhad {
    namespace controls {

class SpinBoxDelegate : public QStyledItemDelegate {
    Q_OBJECT

    public:
    explicit SpinBoxDelegate (QObject *parent = nullptr);
    ~SpinBoxDelegate() override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};



    }
}

#endif //CALENHAD_SPINBOXDELEGATE_H
