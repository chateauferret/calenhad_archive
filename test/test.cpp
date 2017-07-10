//
// Created by martin on 27/06/17.
//

#include "QTestSuite.h"

#include <QtTest/QtTest>
#include <iostream>

int main (int args, char** argv) {
    int failedSuitesCount = 0;
    std::vector<QObject*>::iterator iSuite;
    QGuiApplication app (args, argv);
    for (iSuite = QTestSuite::m_Suites().begin (); iSuite != QTestSuite::m_Suites().end (); iSuite++) {
        int result = QTest::qExec (*iSuite);
        if (result != 0) {
            failedSuitesCount++;
        }
    }
    return failedSuitesCount;
}