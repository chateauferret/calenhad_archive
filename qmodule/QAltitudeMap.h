//
// Created by martin on 31/03/17.
//

#ifndef CALENHAD_QALTITUDEMAP_H
#define CALENHAD_QALTITUDEMAP_H

#include <QtCore/QString>
#include <QtXml/QDomElement>
#include "QModule.h"

class AltitudeMapEditor;

enum CurveType { AltitudeCurve=1, TerraceCurve=2, InvertedTerraceCurve=3 };

class QAltitudeMap : public QModule {
    Q_OBJECT
public:
    static QAltitudeMap* newInstance();
    virtual ~QAltitudeMap();
    void initialise() override;

    QAltitudeMap* clone () override;
    virtual QString nodeType () override;
    QVector<QPointF> getEntries() const;
    virtual void inflate (const QDomElement& element) override;
    virtual void serialise (QDomDocument& doc) override;
    void addEntry (const double& in, const double& out = 0);
    void removeEntry (const double& key);

public slots:

    void updateEntries();
    void editingFinished();
    void resetMap ();
    void editAltitudeMap();

protected:
    QAltitudeMap (QWidget* parent = 0);
    AltitudeMapEditor* _editor;
    QMap<CurveType, noise::module::Module*> _modules;


    void clearMap ();

    QString _oldXml;
};


#endif //CALENHAD_QALTITUDEMAP_H
