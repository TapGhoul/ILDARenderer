#include "3d/quaternion.h"

Quaternion::Quaternion()
{
    w = 1;
    x = 0;
    y = 0;
    z = 0;
}

Quaternion Quaternion::operator *(const Quaternion other)
{
    Quaternion out;
    out.w = (w * other.w) - (x * other.x) - (y * other.y) - (z * other.z);
    out.x = (w * other.x) + (x * other.w) + (y * other.z) - (z * other.y);
    out.y = (w * other.y) - (x * other.z) + (y * other.w) + (z * other.x);
    out.z = (w * other.z) + (x * other.y) - (y * other.x) + (z * other.w);
    return out;
}
