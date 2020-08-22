//
// Created by ferret on 21/08/2020.
//

#ifndef MESSAGES_MAP_H
#define MESSAGES_MAP_H
#include "Cache.h"



namespace calenhad {
    namespace module {

        class Map : public Cache {
        public:
            explicit Map (const QString& type = CalenhadServices::preferences() -> calenhad_module_map);
            ~Map() override;
            bool isComplete() override;

            int mapHeight() const;
            int mapWidth() const;
            geoutils::Geolocation centre();
            double scale() const;
            QImage* image() const;

        protected slots:

            void serialize (QDomElement& element) override;
            void inflate (const QDomElement& element) override;
            void generate();


        protected:
            QImage* _image;


            int _height, _width;
        };

    }
}

#endif //MESSAGES_MAP_H
