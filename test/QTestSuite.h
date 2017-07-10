#ifndef QTESTSUITE_H
#define QTESTSUITE_H

#include <QObject>
#include <vector>

class QTestSuite : public QObject {
Q_OBJECT
public:

public:
    explicit QTestSuite ();

    static std::vector<QObject*>& m_Suites ();
};

#endif // QTESTSUITE_H