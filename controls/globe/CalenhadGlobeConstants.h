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
            enum DatumFormat {
                NoDatum, Native, Scaled
            };

            enum RenderMode {
                RenderModePreview = 0,
                RenderModeOverview = 1,
                RenderModeGlobe = 2,
                RenderModeVertices = 3
            };

        }
    }
}

#endif // CALENHAD_CALENHADGLOBECONSTANTS_H