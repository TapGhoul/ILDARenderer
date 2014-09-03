#ifndef QUATERNION_H
#define QUATERNION_H

class Quaternion
{
public:
    Quaternion();
    double w;
    double x;
    double y;
    double z;
    Quaternion operator*(const Quaternion other);
};

#endif // QUATERNION_H
