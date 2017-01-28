//
// Created by martin on 30/12/16.
//

#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include "RenderJob.h"
#include "../libnoiseutils/NoiseContstants.h"
#include "noise/module/modulebase.h"
#include "../libnoiseutils/NoiseMapBuilderSphere.h"
#include "../libnoiseutils/Color.h"
#include "../libnoiseutils/Image.h"
#include "../libnoiseutils/NoiseMap.h"
#include "../libnoiseutils/RendererImage.h"

using namespace noise::utils;
using namespace geoutils;


bool RenderJob::declared = false;

RenderJob::RenderJob (const GeoQuad& bounds, noise::module::Module* source) :
        _bounds (bounds),
        _source (source),
        _status (RenderJobStatus::Pending),
        _percentComplete (0) {
    emit status (_status);
    if (! declared) {
        qRegisterMetaType<RenderJobStatus>("RenderJobStatus");
        declared = true;
    }
}

RenderJob::~RenderJob () {

}

bool RenderJob::canRender () {

    if (_image && _source) {
        int inputs = _source -> GetSourceModuleCount ();
        if (inputs == 0) {
            return true;
        }
        for (int i = 0; i < inputs; i++) {
            try {
                if (&(_source -> GetSourceModule (i)) == nullptr) {
                    return false;
                }
            } catch (noise::ExceptionNoModule e) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

void RenderJob::setSource (noise::module::Module* source) {
    _source = source;
}

void RenderJob::startJobAsync() {
    if (canRender()) {

        render();

        if (status () != RenderJobStatus::Cancelled) {
            setStatus (RenderJobStatus::Complete);
        } else {
            _image = nullptr;
        }

    } else {
        setStatus (RenderJobStatus::Invalid);
        _image = nullptr;
    }
    if (_image) {
        emit complete();
        if (thread () == QThread::currentThread ()) {
            moveToThread (qApp->thread ());
        }
    }
}
/*
void RenderJob::render (osg::Image* image) {
    utils::Image rendered = fetchImage (image -> t(), image -> s());

    unsigned char r, g, b, a;
    noise::utils::Color c;
    unsigned char* data = image -> data();
    int index = 0;
    // Build and write each horizontal line to the osg::Image.

    for (int y = 0; y < rendered.GetHeight(); y++) {
        if (status() == RenderJobStatus::Underway) {
            for (int x = 0; x < rendered.GetWidth (); x++) {
                c = rendered.GetValue (x, y);
                r = c.red;
                g = c.green;
                b = c.blue;
                a = c.alpha;
                if (image -> t() >= y && image -> s() >= x) {
                    *(data + index++) = r;
                    *(data + index++) = g;
                    *(data + index++) = b;
                }
            }
            _percentComplete = y / rendered.GetHeight ();
            emit progress (_percentComplete);
        }
    }
}
*/
void RenderJob::render() {
    Image rendered = fetchImage (_image -> height (), _image -> width());
    int r, g, b, a;
    noise::utils::Color c;
    // Build and write each horizontal line to the QImage.

    for (int y = 0; y < rendered.GetHeight(); y++) {
        if (status() == RenderJobStatus::Underway) {
            for (int x = 0; x < rendered.GetWidth (); x++) {
                c = rendered.GetValue (x, y);
                r = c.red;
                g = c.green;
                b = c.blue;
                a = c.alpha;
                if (_image -> height() >= y && _image -> width() >= x) {
                    QColor color = QColor (r, g, b, a);
                    _image -> setPixelColor (x, y, color);
                }
            }
        }
    }
}

Image RenderJob::fetchImage (int height, int width) {
    setStatus (RenderJobStatus::Underway);
    NoiseMap heightMap;
    NoiseMapBuilderSphere* heightMapBuilder = new NoiseMapBuilderSphere();

    heightMapBuilder -> SetSourceModule (*_source);
    heightMapBuilder -> SetDestNoiseMap (heightMap);
    heightMapBuilder -> SetDestSize (width, height);
    heightMapBuilder -> SetBounds (_bounds.sw ().lonDegrees, _bounds.ne ().lonDegrees, _bounds.sw ().latDegrees, _bounds.ne ().latDegrees);
    heightMapBuilder -> Build ();

    RendererImage renderer;
    Image rendered;
    renderer.SetSourceNoiseMap (heightMap);
    renderer.SetDestImage (rendered);
    renderer.ClearGradient ();
    renderer.AddGradientPoint (-1.0000, Color (0, 0, 128, 255)); // deeps
    renderer.AddGradientPoint (-0.2500, Color (0, 0, 255, 255)); // shallow
    renderer.AddGradientPoint (0.0000, Color (0, 128, 255, 255)); // shore
    renderer.AddGradientPoint (0.0625, Color (240, 240, 64, 255)); // sand
    renderer.AddGradientPoint (0.1250, Color (32, 160, 0, 255)); // grass
    renderer.AddGradientPoint (0.3750, Color (224, 224, 0, 255)); // dirt
    renderer.AddGradientPoint (0.7500, Color (128, 128, 128, 255)); // rock
    renderer.AddGradientPoint (1.0000, Color (255, 255, 255, 255)); // snow
    renderer.EnableLight ();
    renderer.SetLightContrast (3.0);
    renderer.SetLightBrightness (2.0);
    renderer.Render ();
    return rendered;
}

void RenderJob::setStatus (RenderJobStatus s) {
    _status = s;
    emit status (s);
}

RenderJobStatus RenderJob::status() {
    return _status;
}

void RenderJob::cancel() {
    setStatus (RenderJobStatus::Cancelled);
}

void RenderJob::setImage (std::shared_ptr<QImage>& image) {
    _image = image;
}

void RenderJob::sendProgress (int p) {
    emit progress (p);
}