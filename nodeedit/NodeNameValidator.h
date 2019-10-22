//
// Created by martin on 20/09/17.
//

#ifndef CALENHAD_NODENAMEVALIDATOR_H
#define CALENHAD_NODENAMEVALIDATOR_H


#include <QtGui/QValidator>
namespace calenhad {
    namespace module {
        class Node;

    }

    namespace nodeedit {

        class NodeNameValidator : public QRegularExpressionValidator {
        Q_OBJECT

        public:
            explicit NodeNameValidator (calenhad::module::Node* node);
            ~NodeNameValidator() override;
            QValidator::State validate(QString &input, int &pos) const override;

        signals:
            void message (const QString& text) const;
            void success() const;

        protected:
            calenhad::module::Node* _node;

        };
    }
}

#endif //CALENHAD_NODENAMEVALIDATOR_H
