
// Created by martin on 27/01/17.
//
#include "Legend.h"
#include <QtWidgets/QVBoxLayout>
#include "LegendEditor.h"
#include "LegendService.h"
#include "LegendWidget.h"
#include "CalenhadServices.h"
#include <QIcon>

using namespace calenhad::legend;

Legend::Legend (const QString& name) : _interpolate (true), _name (name), _notes (QString()), _widget (nullptr) {


}

Legend::Legend (const Legend& other) : _interpolate (other._interpolate), _notes (other._notes), _widget (nullptr) {
    setEntries (other.entries());
    QString name;
    int n = 0;
    do {
         name = other._name + "_" + QString::number (n);
    } while (CalenhadServices::legends() -> exists (name));
    _name = other._name + "_" + QString::number (n + 1);
    _widget = new LegendWidget (this);
    _widget -> focusNameField();
}


Legend::~Legend()  {

}


int Legend::size() {
    return entries().size ();
}

QColor Legend::lookup (const double& index) {
    if (_interpolate) {
        std::map<double, QColor>::iterator i = std::find_if_not (_entries.begin(), _entries.end(), [&index] (std::pair<double, QColor> entry) -> bool {
            return entry.first <= index;
        });
        std::map<double, QColor>::iterator j = i;
        QColor color = interpolateColors (i, --j, index);
        return color;
    } else {
        std::map<double, QColor>::iterator i = std::find_if_not (_entries.begin(), _entries.end(), [&index] (std::pair<double, QColor> entry) -> bool {
            return entry.first <= index;
        });
        return (--i) -> second;
    }
}

QColor Legend::interpolateColors (std::map<double, QColor>::iterator lower, std::map<double, QColor>::iterator higher, const double& index) {
    QColor color;
    color.setRedF (interpolateValues (lower -> first, lower -> second.redF(), higher -> first, higher -> second.redF(), index));
    color.setGreenF (interpolateValues (lower -> first, lower -> second.greenF(), higher -> first, higher -> second.greenF(), index));
    color.setBlueF (interpolateValues (lower -> first, lower -> second.blueF(), higher -> first, higher -> second.blueF(), index));
    color.setAlphaF (interpolateValues (lower -> first, lower -> second.alphaF(), higher -> first, higher -> second.alphaF(), index));
    return color;
}

double Legend::interpolateValues (const double& p1, const double& v1, const double& p2, const double& v2, const double& index) {
    if (p1 > p2) { return interpolateValues (p2, v2, p1, v1, index); }
    if (index < p1) return v1;
    if (index > p2) return v2;
    double delta = p2 - p1;
    double weighting = (index - p1) / delta;
    double result = v1 + ((v2 - v1) * weighting);
    return result;
}


void Legend::setInterpolated (const bool& interpolate) {
    _interpolate = interpolate;
    CalenhadServices::legends() -> setDirty();
    emit legendChanged();
}

const bool& Legend::isInterpolated () const {
    return _interpolate;
}


// Replace the entry if it exists, add it in otherwise.
void Legend::addEntry (const double& value, const QColor& colour) {
    _entries [value] = colour;
    CalenhadServices::legends() -> setDirty();
}

unsigned Legend::removeEntries (const double& from, const double& unto) {
    unsigned count = 0;

    for (std::map<double, QColor>::iterator i = _entries.begin(); i != _entries.end(); i++) {
        std::pair<const double, QColor> entry = *i;
        if (entry.first > from && entry.first < unto) {
            _entries.erase (i);
            count++;
        }
    }
    CalenhadServices::legends() -> setDirty();
    return count;
}

const QList<LegendEntry> Legend::entries() const {
    QList<LegendEntry> entries;
    std::map<double, QColor>::const_iterator i;
    for (i = _entries.begin (); i != _entries.end (); i++) {
        std::pair<const double, QColor> entry = *i;
        entries.append (LegendEntry (entry.first, entry.second));
    }
    return entries;
}

LegendEntry Legend::at (int i) {
    return entries().at (i);
}

void Legend::clear() {
    _entries.clear ();
}

LegendWidget* Legend::widget() {
    if (! _widget) {
        _widget = new LegendWidget (this);
    }
    return _widget;
}

void Legend::setEntries (const QList<LegendEntry>& entries) {
    _entries.clear ();
    for (LegendEntry entry : entries) {
        std::pair<qreal, QColor> item = std::make_pair (entry.first, entry.second);
        _entries.insert (item);
    }
    CalenhadServices::legends() -> setDirty();
    emit legendChanged();
}

const bool Legend::isValid() const {
    return _entries.size() >= 2;
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


void Legend::setName (const QString& name) {

    // when we change the name of a legend, we need to tell the LegendService so that it can find it under the new name
    CalenhadServices::legends() -> rename (_name, name);
    _name = name;
    emit renamed (name);
}

void Legend::setNotes (const QString& notes) {
    _notes = notes;
}

const QString& Legend::notes() const {
    return _notes;
}

QString Legend::name () {
    return _name;
}


void Legend::inflate (const QDomNode& n) {
    if (n.isElement()) {
        clear ();
        QDomElement e = n.toElement ();
        QString legendType = e.attribute ("type");

        QDomNode nameNode = e.firstChildElement ("name");
        QString name = nameNode.firstChild ().nodeValue ();
        _name = name;

        setInterpolated (legendType.toLower () == "gradient");
        QDomNode notesNode = e.firstChildElement ("notes");
        QString notes = notesNode.nodeValue ();
        setNotes (notes);

        QDomNodeList nodes = e.elementsByTagName ("entry");
        for (int i = 0; i < nodes.size (); i++) {
            QDomElement element = nodes.item (i).toElement ();
            bool ok;
            double index = element.attribute ("index").toDouble (&ok);
            if (ok) {
                QColor c = QColor (element.attribute ("color"));
                addEntry (index, c);
            }
        }



    }
}



void Legend::serialise (QDomDocument doc) {
    QDomElement root = doc.firstChildElement ("legends");
    QDomElement e = doc.createElement ("legend");
    e.setAttribute ("type", isInterpolated() ? "gradient" : "stepwise");
    root.appendChild (e);
    QDomElement nameNode = doc.createElement ("name");
    QDomText nameText = doc.createTextNode (name());
    nameNode.appendChild (nameText);
    e.appendChild (nameNode);
    QDomElement notesNode = doc.createElement ("notes");
    QDomText notesText = doc.createTextNode (notes());
    notesNode.appendChild (notesText);
    e.appendChild (notesNode);
    for (LegendEntry entry : entries()) {
        QDomElement entryElement = doc.createElement ("entry");
        entryElement.setAttribute ("index", entry.first);
        entryElement.setAttribute ("color", entry.second.name());
        e.appendChild (entryElement);
    }
}

QIcon Legend::icon() {
    QPixmap pixmap (150, 30);
    QColor color;
    QPainter painter (&pixmap);
    double step = 2.0 / pixmap.width();
    int i = 0;
    for (double index = -1.0; index < 1.0; index += step) {
        color = lookup (index);
        QPen pen = QPen (color);
        painter.setPen (pen);
        painter.drawLine (i, 0, i, pixmap.height());
        i++;
    }
    QIcon icon (pixmap);
    return icon;
}