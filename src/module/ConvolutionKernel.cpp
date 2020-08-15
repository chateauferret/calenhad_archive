//
// Created by ferret on 13/08/2020.
//

#include <src/CalenhadServices.h>
#include "ConvolutionKernel.h"


using namespace calenhad::module;

ConvolutionKernel::ConvolutionKernel () {
    clear();
}

ConvolutionKernel::~ConvolutionKernel () {

}

void ConvolutionKernel::clear() {
    _kernel.clear();
}

void calenhad::module::ConvolutionKernel::setKernel (const QStringList& kernel, const int& size) {
    _kernel = kernel;
}

QStringList calenhad::module::ConvolutionKernel::kernel() {
    return _kernel;
}

void ConvolutionKernel::setExpression (const QString& expr, const int& x, const int& y) {
    _kernel.insert (y * 9 + x, expr);
}

void ConvolutionKernel::setValue (const double& value, const int& x, const int& y) {
    QString expr = QString::number (value);
    setExpression (expr, x, y);
}

QString ConvolutionKernel::expression (const int& x, const int& y) {
    return _kernel.at (y * 9 + x);
}

double ConvolutionKernel::value (const int& x, const int& y) {
    QString expr = expression (x, y);
    return CalenhadServices::calculator() -> compute (expr);
}
