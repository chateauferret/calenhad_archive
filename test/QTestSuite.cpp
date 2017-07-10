//
// Created by martin on 27/06/17.
//

#include "QTestSuite.h"
#include <iostream>

std::vector<QObject*> &QTestSuite::m_Suites() { static std::vector<QObject*> INST; return INST;}


QTestSuite::QTestSuite() : QObject() {
    m_Suites().push_back(this);
}
