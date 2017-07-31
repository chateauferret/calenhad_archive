//
// Created by martin on 27/11/16.
//
#include <iostream>
#include "../preferences/PreferencesService.h"
#include "../CalenhadServices.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "QConstModule.h"
#include "../exprtk/ExpressionWidget.h"
#include "../nodeedit/QNodeBlock.h"

using namespace noise::module;
using namespace calenhad::qmodule;
using namespace calenhad::expressions;
using namespace calenhad::nodeedit;

QConstModule::QConstModule (QWidget* parent) : QModule (new Const(), parent) {

}
