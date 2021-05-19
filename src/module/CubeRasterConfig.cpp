//
// Created by ferret on 20/01/2021.
//

#include "CubeRasterConfig.h"
#include "CubeRasterFile.h"

using namespace calenhad::module;

CubeRasterConfig::CubeRasterConfig() : _files() {
    for (int i = 0; i < 6; i++) {
        CubeRasterFile* file = new CubeRasterFile();
        if (i == 0) { file -> setRotation (2); }
        if (i == 1) { file -> setRotation (0); file -> flipVertical (true); }
        if (i == 4) { file -> setRotation (3); }
        if (i == 5) { file -> setRotation (3); file -> flipVertical (true); }
        _files [i] = file;
    }
}

CubeRasterConfig::~CubeRasterConfig() {
    for (int i = 0; i < 6; i++) {
        delete _files [i];
    }
}

QString CubeRasterConfig::filename (const int& index) {
    return _files [index] -> filename();
}

void CubeRasterConfig::setRotation (const int& index, const int& rotation) {
    _files [index] -> setRotation (rotation);
    emit configChanged();
}

int CubeRasterConfig::rotation (const int& index) {
    return _files [index] -> getRotation();
}

void CubeRasterConfig::setReflection (const int& index, const bool& flipHorizontal, const bool& flipVertical) {
    _files [index] -> flipHorizontal (flipHorizontal);
    _files [index] -> flipVertical (flipVertical);
    emit configChanged();
}

int CubeRasterConfig::columnCount (const QModelIndex& parent) const {
    Q_UNUSED (parent)
    return 7;
}

int CubeRasterConfig::rowCount (const QModelIndex& parent) const {
    Q_UNUSED (parent)
    return 6;
}

QVariant CubeRasterConfig::data (const QModelIndex& index, int role) const {
    int row = index.row();
    int column = index.column();
    if (column == 0) {
        if (row == 0) { return " 90ºW  0ºN"; }
        if (row == 1) { return " 90ºE  0ºN"; }
        if (row == 2) { return "  0ºE 90ºN"; }
        if (row == 3) { return "  0ºE 90ºS"; }
        if (row == 4) { return "  0ºE  0ºN"; }
        if (row == 5) { return "180ºE  0ºN"; }
    }
    if (column == 1) {
        return _files [row] -> filename();
    }
    if (column == 2) {
        QImage* image = _files [row] -> getImage();
        return image ? QString::number (image -> width()) + " x " + QString::number (image -> height()) : QString();
    }
    if (column == 3) {
        return _files [row] -> isFlippedVertical() ? "Bottom up" : "Top down";
    }
    if (column == 4) {
        return _files [row] -> isFlippedHorizontal() ? "Right to left" : "Left to right";
    }
    if (column == 5) {
        return QString::number (_files [row] -> getRotation() * 90) + "º";
    }
    if (column == 6) {
        return _files [row] -> isEnabled() ? "Yes" : "No";
    }
    return QVariant();
}

bool CubeRasterConfig::setData (const QModelIndex &index, const QVariant &value, int role) {
    int row = index.row();
    int column = index.column();
    if (role == Qt::EditRole) {
        if (column == 1) {
            _files [row] -> setFilename (value.toString());
            emit dataChanged (index, index);
            return true;
        }
        if (column == 3) {
            _files [row] -> flipVertical (value.toBool());
            emit dataChanged (index, index);
            return true;
        }
        if (column == 4) {
            _files [row] -> flipHorizontal (value.toBool());
            emit dataChanged (index, index);
            return true;
        }
        if (column == 5) {
            _files [row] -> setRotation (value.toInt());
            emit dataChanged (index, index);
            return true;
        }
        if (column == 6) {
            _files [row] -> setEnabled (value.toBool());
            emit dataChanged (index, index);
            return true;
        }
        return false;
    }
}

QVariant CubeRasterConfig::headerData (int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            if (section == 0) { return "Centre"; }
            if (section == 1) { return "Filename"; }
            if (section == 2) { return "Size"; }
            if (section == 3) { return "Row order"; }
            if (section == 4) { return "Column order"; }
            if (section == 5) { return "Rotation"; }
            if (section == 6) { return "Enabled"; }
        } else {
            if (section == 0) { return "West"; }
            if (section == 1) { return "East"; }
            if (section == 2) { return "North"; }
            if (section == 3) { return "South"; }
            if (section == 4) { return "Front"; }
            if (section == 5) { return "Back"; }
        }
    }
    return QVariant();
}

Qt::ItemFlags CubeRasterConfig::flags (const QModelIndex& index) const {
    int column = index.column();
    if (column > 0 && column < 7 && column != 2) {
        return {Qt::ItemIsEditable | Qt::ItemIsEnabled};
    } else {
        return {Qt::ItemIsEnabled};
    }
}
