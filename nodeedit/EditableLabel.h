//
// Created by martin on 21/04/17.
//

#ifndef CALENHAD_EDITABLELABEL_H
#define CALENHAD_EDITABLELABEL_H


#include <QtWidgets/QGraphicsTextItem>
#include <QtWidgets/QGraphicsProxyWidget>
#include <QtWidgets/QLineEdit>
namespace calenhad {
    namespace nodeedit {
        class NodeNameValidator;
        class EditableLabel : public QGraphicsTextItem {
        Q_OBJECT



        public:
            EditableLabel (QGraphicsItem* parent = 0);

            virtual ~EditableLabel ();
            void setAlignment (Qt::AlignmentFlag flag);
            void setValidator (NodeNameValidator* v);
            void setText (const QString& text);
            QString proposedText();
            void mousePressEvent (QGraphicsSceneMouseEvent* event) override;
            void setTextColor (const QColor& color);
        public slots:
            void showMessage (const QString& message);
            void clearMessage();
            void finishedEditing ();

        signals:

            void editingStateChanged (const bool& state);

            void textChanged (const QString& text);

            void textEdited (const QString& text);

        protected:
            QGraphicsProxyWidget* _proxy;
            QLineEdit* _textEdit;


            QPalette* _palette;

            NodeNameValidator* _validator;
            Qt::AlignmentFlag _alignment;

            Qt::AlignmentFlag alignment ();
        };
    }
}

#endif //CALENHAD_EDITABLELABEL_H
