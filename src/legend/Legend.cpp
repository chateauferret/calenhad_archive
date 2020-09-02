
// Created by martin on 27/01/17.
//
#include "Legend.h"
#include <QtWidgets/QVBoxLayout>
#include "controls/legend/LegendEditor.h"
#include "LegendService.h"
#include "LegendWidget.h"
#include "../CalenhadServices.h"
#include <QIcon>
#include <utility>
#include "exprtk/Calculator.h"

using namespace calenhad::legend;
using namespace calenhad::expressions;

Legend::Legend (QString  name) : _interpolate (true), _name (std::move(name)), _notes (QString()), _widget (nullptr), _colorMapBuffer (nullptr) {

}

Legend::Legend (const Legend& other) : _interpolate (other._interpolate), _notes (other._notes), _widget (nullptr), _colorMapBuffer (other._colorMapBuffer) {
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
        QVector<LegendEntry>::iterator i = std::find_if_not (_entries.begin(), _entries.end(), [&index] (LegendEntry entry) -> bool {
            return entry.keyValue() <= index;
        });
        QVector<LegendEntry>::iterator j = i;
        if (i == _entries.begin()) {
            return i -> color();
        } else {
            if (i == _entries.end()) {
                return i->color();
            } else {
                QVector<LegendEntry>::iterator j = i;
                return interpolateColors(--j, i, index);
            }
        }
    } else {
        QVector<LegendEntry>::iterator i = std::find_if_not (_entries.begin(), _entries.end(), [&index] (LegendEntry entry) -> bool {
            return entry.keyValue() <= index;
        });
        return (--i) -> color();
    }
}

QColor Legend::interpolateColors (LegendEntry* lower, LegendEntry* higher, const double& index) {
    QColor color;
    color.setRedF (interpolateValues (lower -> keyValue(), lower -> color().redF(), higher -> keyValue(), higher -> color().redF(), index));
    color.setGreenF (interpolateValues (lower -> keyValue(), lower -> color().greenF(), higher -> keyValue(), higher -> color().greenF(), index));
    color.setBlueF (interpolateValues (lower -> keyValue(), lower -> color().blueF(), higher -> keyValue(), higher -> color().blueF(), index));
    color.setAlphaF (interpolateValues (lower -> keyValue(), lower -> color().alphaF(), higher -> keyValue(), higher -> color().alphaF(), index));
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
void Legend::addEntry (const LegendEntry& entry) {
    for (LegendEntry e : _entries) {
        if (e.key() == entry.key()) {
            _entries.remove(_entries.indexOf(e));
        }
    }
    _entries.append(entry);
    CalenhadServices::legends()->setDirty();
}

unsigned Legend::removeEntries (const double& from, const double& unto) {
    unsigned count = 0;
    for (LegendEntry e : _entries) {
        if (e.keyValue() > from && e.keyValue() < unto) {
            _entries.remove (_entries.indexOf (e));
            count++;
        }
    }
    CalenhadServices::legends() -> setDirty();
    return count;
}

const QVector<LegendEntry>& Legend::entries() const {
    return _entries;
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

void Legend::setEntries (const QVector<LegendEntry>& entries) {
    _entries = entries;
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

    if (! (name.isEmpty())) {
        CalenhadServices::legends() -> rename (_name, name);
        _name = name;
        emit renamed (name);
    }
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
        setName (name);

        setInterpolated (legendType.toLower () == "gradient");
        QDomNode notesNode = e.firstChildElement ("notes");
        QString notes = notesNode.nodeValue ();
        setNotes (notes);

        QDomNodeList nodes = e.elementsByTagName ("entry");
        for (int i = 0; i < nodes.size (); i++) {
            QDomElement element = nodes.item (i).toElement ();
            if (element.hasAttribute ("index") && element.hasAttribute ("color")) {
                QString index = element.attribute("index");
                QColor c = QColor(element.attribute("color"));
                addEntry(LegendEntry(index, c));
            }
        }
    }
}

void Legend::serialise (QDomDocument doc) {
    QDomElement root = doc.documentElement().firstChildElement ("legends");
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
        entryElement.setAttribute ("index", entry.key());
        entryElement.setAttribute ("color", entry.color().name());
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

void Legend::setEntry (const int& index, const QString& key, const QColor& color) {
    LegendEntry entry (key, color);
    _entries.replace (index, entry);
}

bool Legend::isComputed() {
    for (LegendEntry entry : _entries) {
        if (entry.isComputed()) {
            return true;
        }
    }
    return false;
}

float* Legend::colorMapBuffer() {
        // To do - magic number
        uint size = CalenhadServices::preferences() -> calenhad_colormap_buffersize;
        if (! _colorMapBuffer) {
            _colorMapBuffer = new float [size * 4];
        }

        int k = 0;
        QColor c = _entries.first().color();
        float dx = (1 / ((float) size)) ;
        for (int i = 0; i < size; i++)  {
            double index = (i * dx * 2) - 1;
            if (k < _entries.size()) {
                LegendEntry e = _entries[k];
                if (index > e.keyValue()) {
                    c = _entries[k].color();
                    k++;
                }

                if (_interpolate && k > 0 && k < _entries.size()) {
                    c = interpolateColors(&_entries[k - 1], &_entries[k], index);
                }
            }
            _colorMapBuffer [i * 4 + 0] = (float) c.redF();
            _colorMapBuffer [i * 4 + 1] = (float) c.greenF();
            _colorMapBuffer [i * 4 + 2] = (float) c.blueF();
            _colorMapBuffer [i * 4 + 3] = (float) c.alphaF();

        }

        return _colorMapBuffer;
    }
