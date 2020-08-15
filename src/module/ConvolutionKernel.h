//
// Created by ferret on 13/08/2020.
//

#ifndef MESSAGES_CONVOLUTIONKERNEL_H
#define MESSAGES_CONVOLUTIONKERNEL_H

#include <QString>
#include <QtCore/QStringList>
#include "../exprtk/Calculator.h"

namespace calenhad {
    namespace module {

        class ConvolutionKernel {
        public:
            ConvolutionKernel();
            ~ConvolutionKernel();
            void setKernel (const QStringList& kernel, const int& size);
            QStringList kernel();
            void setExpression (const QString& expr, const int& x, const int& y);
            void setValue (const double& value, const int& x, const int& y);
            QString expression (const int& x, const int& y);
            double value (const int&x, const int& y);

        protected:
            QStringList _kernel;
            QString _name;
            QString _description;
            QString multiplier;

            void clear ();
        };


    }
}


#endif //MESSAGES_CONVOLUTIONKERNEL_H
