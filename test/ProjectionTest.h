//
// Created by martin on 26/06/17.
//

#ifndef CALENHAD_PROJECTIONTEST_H
#define CALENHAD_PROJECTIONTEST_H

//
// Created by martin on 25/06/17.
//

#include <QTest>
#include "../CalenhadServices.h"
#include "QTestSuite.h"

namespace calenhad {
    namespace mapping {
        namespace projection {
            class EquirectangularProjection;

        }
    }
}
class ProjectionTest : public QTestSuite {
Q_OBJECT

public:
    ProjectionTest ();

    ~ProjectionTest ();

private slots:

    void initTestCase ();

    void init ();

    void cleanUp ();

    void cleanUpTestCase ();
    void equirectangularProjectionInverseTest_data ();
    void equirectangularProjectionInverseTest ();
    void equirectangularProjectionForwardTest_data ();
    void equirectangularProjectionForwardTest ();

protected:

    calenhad::mapping::projection::ProjectionService* roster;
};



#endif //CALENHAD_PROJECTIONTEST_H

