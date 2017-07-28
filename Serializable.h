//
// Created by martin on 21/07/17.
//

#ifndef CALENHAD_SERIALIZABLE_H
#define CALENHAD_SERIALIZABLE_H

#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>

namespace calenhad {
    class Serializable {

    public:
        virtual void inflate (const QDomElement& element) = 0;
        virtual void serialize (QDomDocument& doc) = 0;
    };

} // namespace

#endif //CALENHAD_SERIALIZABLE_H
