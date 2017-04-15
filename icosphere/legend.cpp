#include <QtXml/QDomNode>
#include "legend.h"
#include "../libnoiseutils/GradientLegend.h"

using namespace icosphere;
using namespace noise::utils;

Legend::Legend (const QString& name) : _name (name), _defaultColor (QColor (0, 0, 0, 0)) {

}

Legend::~Legend() {

}

Legend::Legend (const Legend& other) :
    _defaultColor (other._defaultColor), _name (other._name) {}

QString Legend::name() {
    return _name;
}

void Legend::setDefaultColor (const QColor& colour) {
    _defaultColor = colour;
}

QColor Legend::lookup (const std::experimental::optional<double>& value) {
    if ((bool) value) {
        return lookup (value.value ());
    } else {
        return _defaultColor;
    }
}

icosphere::Legend* Legend::fromNode (const QDomNode& n) {
    if (n.isElement()) {

        QDomElement e = n.toElement ();
        QString legendType = e.attribute ("type");

        QDomNode nameNode = e.firstChildElement ("name");
        QString name = nameNode.nodeValue ();
        Legend* legend;
        if (legendType == "Gradient") {
            GradientLegend* l = new GradientLegend (name);
            legend = l;
        } else {
            IntervalLegend* l = new IntervalLegend (name);
            legend = l;
        }

        QDomNode notesNode = e.firstChildElement ("notes");
        QString notes = notesNode.nodeValue ();
        legend -> setNotes (notes);

        QDomNodeList nodes = e.elementsByTagName ("entry");
        for (int i = 0; i < nodes.size (); i++) {
            QDomElement element = nodes.item (i).toElement ();
            bool ok;
            double index = element.attribute ("index").toDouble (&ok);
            if (ok) {
                QColor c = QColor (element.attribute ("color"));
                legend->addEntry (index, c);
            }
        }
        return legend;
    } else return nullptr;
}

void Legend::setName (QString name) {
    _name = name;
}

void Legend::setName (const QString& name) {
    _name = name;
}

void Legend::setNotes (const QString& notes) {
    _notes = notes;
}

QString Legend::notes () {
    return _notes;
}



