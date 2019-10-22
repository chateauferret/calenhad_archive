//
// Created by martin on 06/06/17.
//

#include <CalenhadServices.h>
#include "NodeGroup.h"
#include "../pipeline/CalenhadModel.h"
#include "CalenhadServices.h"
#include "../preferences/PreferencesService.h"

using namespace calenhad::module;
using namespace calenhad::nodeedit;
using namespace calenhad::pipeline;

NodeGroup::NodeGroup (CalenhadModel* model) :
    _color (CalenhadServices::preferences() -> calenhad_module_brush_color_normal),
    _borderColor (CalenhadServices::preferences() -> calenhad_module_pen_color_normal),
    _selectedColor (CalenhadServices::preferences() -> calenhad_module_brush_color_selected),
    _selectedBorderColor (CalenhadServices::preferences() -> calenhad_module_pen_color_selected),
    _name (QString::null),
    _model (model) {

}

NodeGroup::~NodeGroup() {

}

void NodeGroup::inflate (const QDomElement& element) {
    QDomElement colorsElement = element.firstChildElement ("colors");
    QDomElement colorElement = colorsElement.firstChildElement ("color");
    QDomElement borderColorElement = colorsElement.firstChildElement ("borderColor");
    QDomElement selectedColorElement = colorsElement.firstChildElement ("selectedColor");
    QDomElement selectedBorderColorElement = colorsElement.firstChildElement ("selectedBorderColor");

    _color = colorElement.isNull() ? CalenhadServices::preferences() -> calenhad_module_brush_color_normal : QColor (colorElement.attribute ("color", CalenhadServices::preferences() -> calenhad_module_brush_color_normal.name ()));
    _borderColor = borderColorElement.isNull() ? CalenhadServices::preferences() -> calenhad_module_pen_color_normal : QColor (borderColorElement.attribute ("color", CalenhadServices::preferences() -> calenhad_module_pen_color_normal.name ()));
    _selectedColor = selectedColorElement.isNull() ? CalenhadServices::preferences() -> calenhad_module_brush_color_selected : QColor (selectedColorElement.attribute ("color", CalenhadServices::preferences() -> calenhad_module_brush_color_selected.name ()));
    _selectedBorderColor = selectedBorderColorElement.isNull() ? CalenhadServices::preferences() -> calenhad_module_pen_color_selected : QColor (selectedBorderColorElement.attribute ("color", CalenhadServices::preferences() -> calenhad_module_pen_color_selected.name ()));

    QDomElement nodesElement = element.firstChildElement ("nodes");


}

void NodeGroup::serialize (QDomElement& element) {
    QDomElement nameElement = element.ownerDocument().createElement ("name");
    QDomText nameText = element.ownerDocument().createTextNode (_name);
    nameElement.appendChild (nameText);
    element.appendChild (nameElement);
    QDomElement colorsElement = element.ownerDocument().createElement ("colors");
    QDomElement colorElement = element.ownerDocument().createElement ("color");
    colorElement.setAttribute ("color", _color.name());
    colorsElement.appendChild (colorElement);
    QDomElement borderColorElement = element.ownerDocument().createElement ("borderColor");
    borderColorElement.setAttribute ("color", _borderColor.name());
    colorsElement.appendChild (borderColorElement);
    QDomElement selectedColorElement = element.ownerDocument().createElement ("selectedBorderColor");
    selectedColorElement.setAttribute ("color", _selectedColor.name());
    colorsElement.appendChild (selectedColorElement);
    QDomElement selectedBorderColorElement = element.ownerDocument().createElement ("selectedBorderColor");
    selectedBorderColorElement.setAttribute ("color", _selectedBorderColor.name());
    colorsElement.appendChild (selectedBorderColorElement);
    element.appendChild (colorsElement);
    QDomElement nodesElement = element.ownerDocument().createElement ("nodes");
    element.appendChild (nodesElement);

    // serialise all the nodes in this group
    for (Node* node : _model -> nodes()) {
        if (node -> group() == this) {
            node -> serialize (nodesElement);
        }
    }
}

void NodeGroup::setColors (const QColor& color, const QColor& selectedColor, const QColor& borderColor, const QColor& selectedBorderColor) {
    _color = color;
    _selectedColor = selectedColor;
    _borderColor = borderColor;
    _selectedBorderColor = selectedBorderColor;
}

void NodeGroup::setName (const QString& name) {
    _name = name;
}

void NodeGroup::setDocumentation (const QString& documentation) {
    _documentation = documentation;
}

void NodeGroup::setModel (calenhad::pipeline::CalenhadModel* model) {
    _model = model;
}

QString NodeGroup::name () {
    return _name;
}

QString NodeGroup::documentation () {
    return _documentation;
}

QColor NodeGroup::color () {
    return _color;
}

QColor NodeGroup::borderColor () {
    return _borderColor;
}

QColor NodeGroup::selectedColor () {
    return _selectedColor;
}

QColor NodeGroup::selectedBorderColor () {
    return _selectedBorderColor;
}

QList<Module*> NodeGroup::modules () {
    return _model -> modules (this);
}
