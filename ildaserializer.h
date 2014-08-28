#ifndef ILDASERIALIZER_H
#define ILDASERIALIZER_H

#include <QObject>

class ILDASerializer : public QObject
{
    Q_OBJECT
public:
    explicit ILDASerializer(QObject *parent = 0);
    static uchar* coordinateHeader();

signals:

public slots:

};

#endif // ILDASERIALIZER_H
