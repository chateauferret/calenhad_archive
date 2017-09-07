//
// Created by martin on 22/07/17.
//

#ifndef CALENHAD_CALENHADGLOBECONSTANTS_H
#define CALENHAD_CALENHADGLOBECONSTANTS_H

namespace calenhad {
    namespace controls {
        namespace globe {

            enum CalenhadGlobeDragMode {
                NoDrag, Pan, Zoom
            };
            enum CalenhadGlobeDoubleClickMode {
                NoDoubleClick, Goto, Place
            };

            enum CoordinatesFormat {
                NoCoordinates, Decimal, Traditional
            };
            enum DatumFormat {
                NoDatum, Native, Scaled
            };


        }
    }
}

#endif // CALENHAD_CALENHADGLOBECONSTANTS_H