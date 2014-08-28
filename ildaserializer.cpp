#include "ildaserializer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ILDAHEADER 'I', 'L', 'D', 'A', 0x0, 0x0, 0x0
#define COMPANYNAME 'A', 'N', 'D', 'R', 'E', 'W', '.', 'S'
#define reverse16(i) (i & 0xff00) >> 8, i & 0xff

using namespace std;

ILDASerializer::ILDASerializer(QObject *parent) :
    QObject(parent)
{
}

uchar *ILDASerializer::coordinateHeader() {
    uchar output[32] = {};
    uchar header[8] = {ILDAHEADER, 0x1};
    uchar framename[8] = {'F', 'R', 'M', '0', '0', '0', '0', '0'};
    uchar companyname[8] = {COMPANYNAME};
    uint16_t totalPoints = 17435;
    uint16_t frameno = 1;
    uint16_t totalframes = 100;


    uchar totalPointsc[2] = { reverse16(totalPoints) };
    string framenos = to_string(frameno);
    uchar framenoc[2] = { reverse16(frameno) };
    uchar totalframesc[2] = { reverse16(totalframes) };

    memcpy(framename+3+(5-framenos.length()), &framenos[0], framenos.length());
    memcpy(output, header, 8);
    memcpy(output+8, framename, 8);
    memcpy(output+16, companyname, 8);
    memcpy(output+24, totalPointsc, 2);
    memcpy(output+26, framenoc, 2);
    memcpy(output+28, totalframesc, 2);
    output[30] = 0;
    output[31] = 0;
    return NULL;
}
