#ifndef ILDASERIALIZER_H
#define ILDASERIALIZER_H

#include <QObject>

class ILDASerializer : public QObject
{
    Q_OBJECT
public:
    explicit ILDASerializer(QObject *parent = 0);
    static std::vector<uchar> * coordinates();
    static std::vector<uchar> * colourTable();

signals:

public slots:

private:
    //static uchar * coordinateHeader();

};

#endif // ILDASERIALIZER_H
