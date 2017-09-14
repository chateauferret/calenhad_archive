
#include <QtGui/QGuiApplication>
#include <mapping/CalenhadMap.h>
#include <QtWidgets/QApplication>
#include <iostream>
#include <legend/LegendRoster.h>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include "../qmodule/QModule.h"
#include "../CalenhadServices.h"
#include "../pipeline/ModuleFactory.h"
#include "../exprtk/CalculatorService.h"
#include "../pipeline/CalenhadModel.h"
#include "../preferences/preferences.h"
#include "../nodeedit/Calenhad.h"
#include "../graph/graph.h"
#include "../nodeedit/CalenhadController.h"

using namespace calenhad;
using namespace calenhad::graph;
using namespace calenhad::preferences;
using namespace calenhad::qmodule;
using namespace calenhad::pipeline;
using namespace calenhad::expressions;
using namespace calenhad::legend;
using namespace calenhad::nodeedit;
using namespace calenhad::mapping;

int main(int argc, char* argv[]) {
    QApplication app (argc, argv);

    // set up services
    // Preferences service
    Preferences* preferences = new Preferences();
    preferences -> loadSettings();
    CalenhadServices::providePreferences (preferences);

    // Legends service
    QString fname = CalenhadServices::preferences() -> calenhad_legends_filename;
    LegendRoster* roster = new LegendRoster();
    CalenhadServices::provideLegends (roster);
    roster->inflate (fname);

    // Calculator service
    CalculatorService* calculator = new CalculatorService();
    CalenhadServices::provideCalculator (calculator);

    // test glslInverse generation with some modules

    // test xml file
    QFile file ("/home/martin/ClionProjects/calenhad/cmake-build-debug/test-2.xml");
    file.open(QIODevice::ReadOnly);
    QTextStream textStream (&file);
    QString xml;
    xml.append (textStream.readAll());
    std::cout<< xml.toStdString () << "\n\n";

    Graph* g = new Graph (xml, "m");

    CalenhadMap cm;
    cm.setGraph (g);
    cm.show();

    return app.exec();

 }
/*
#include "cudatest.h"
/*
 ============================================================================
 Name        : cudatest.cu
 Author      : Martin Bucknall
 Version     :
 Copyright   : Your copyright notice
 Description : CUDA compute reciprocals
 ============================================================================
 */
/*
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <driver_types.h>
#include <cuda_runtime_api.h>


#define CUDA_CHECK_RETURN(value) CheckCudaErrorAux(__FILE__,__LINE__, #value, value)



int main(void)
{

    gpuReciprocal (65530);

    return 0;
}

*/