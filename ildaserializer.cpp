#include "ildaserializer.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <iostream>
#include <fstream>
#define ILDAHEADER 'I', 'L', 'D', 'A', 0x0, 0x0, 0x0
#define COMPANYNAME { 'A', 'N', 'D', 'R', 'E', 'W', '.', 'S' }
#define reverse16(i) { (i & 0xff00) >> 8, i & 0xff }

using namespace std;

ILDASerializer::ILDASerializer(QObject *parent) :
    QObject(parent)
{
}

uchar * coordinateHeader(uint16_t totalPoints, uint16_t totalFrames, uint16_t frameNo) {
    uchar *output = new uchar[32];
    uchar header[8] = {ILDAHEADER, 0x1};
    uchar frameName[8] = {'F', 'R', 'M', '0', '0', '0', '0', '0'};
    uchar companyName[8] = COMPANYNAME;


    uchar totalPointsc[2] = reverse16(totalPoints);
    string frameNos = to_string(frameNo);
    uchar frameNoc[2] = reverse16(frameNo);
    uchar totalFramesc[2] = reverse16(totalFrames);

    memcpy(frameName+3+(5-frameNos.length()), &frameNos[0], frameNos.length());
    memcpy(output, header, 8);
    memcpy(output+8, frameName, 8);
    memcpy(output+16, companyName, 8);
    memcpy(output+24, totalPointsc, 2);
    memcpy(output+26, frameNoc, 2);
    memcpy(output+28, totalFramesc, 2);
    output[30] = 0;
    output[31] = 0;
    return output;
}

uchar * ILDASerializer::coordinates() {
    uint16_t totalFrames = 100;
    ofstream outfile("test.ild", ofstream::binary);
    if (!outfile) {
        // Error opening file!
        return NULL;
    }
    for (int frame = 0; frame < totalFrames; frame++) {
        uint16_t totalPoints = 100;
        uchar * header = coordinateHeader(totalPoints, totalFrames, frame);
        outfile.write((const char*)&header[0], 32);
        uchar points[totalPoints*6] = {};
        for (int point = 0; point < totalPoints; point++) {
            __int16_t x = 'X' << 8 | 'x';
            __int16_t y = 'Y' << 8 | 'y';
            __int16_t status = 'S' << 8 | 's';

            uchar pointX[2] = reverse16(x);
            uchar pointY[2] = reverse16(y);
            uchar pointStatus[2] = reverse16(status);
            memcpy(points+point*6, pointX, 2);
            memcpy(points+point*6+2, pointY, 2);
            memcpy(points+point*6+4, pointStatus, 2);
        }
        outfile.write((const char*)&points[0], totalPoints*6);
    }
    uchar * footer = coordinateHeader(0, 0, 0);
    outfile.write((const char*)&footer[0], 32); // May actually be 26, gotta confirm the file specs
    outfile.close();
    return NULL;
}

uchar * colourHeader(uint16_t totalColours, uint16_t paletteNumber) {
    uchar *output = new uchar[32];
    uchar header[8] = {ILDAHEADER, 0x2};
    uchar paletteName[8] = { 'P', 'L', 'T', '0', '0', '0', '0', '0' };
    uchar companyName[8] = COMPANYNAME;

    uchar totalColoursc[2] = reverse16(totalColours);
    string paletteNumbers = to_string(paletteNumber);
    uchar paletteNumberc[2] = reverse16(paletteNumber);

    memcpy(paletteName+3+(5-paletteNumbers.length()), &paletteNumbers[0], paletteNumbers.length());
    memcpy(output, header, 8);
    memcpy(output+8, paletteName, 8);
    memcpy(output+16, companyName, 8);
    memcpy(output+24, totalColoursc, 2);
    memcpy(output+26, paletteNumberc, 2);
    output[28] = 0;
    output[29] = 0;
    output[30] = 0; // Scanner head (0-255)
    output[31] = 0;

    return output;
}

uchar * ILDASerializer::colourTable() {
    uint16_t totalPalettes = 1;
    for (int palette = 0; palette < totalPalettes; palette++) {
        uint16_t totalColours = 10;
        uchar * header = colourHeader(totalColours, palette);
        uchar colours[totalColours*3]= {};
        for (int colour = 0; colour < totalColours; colour++) {
            __uint8_t r = 'R';
            __uint8_t g = 'G';
            __uint8_t b = 'B';

            memcpy(colours+colour*3, &r, 1);
            memcpy(colours+colour*3+1, &g, 1);
            memcpy(colours+colour*3+2, &b, 1);
        }
    }
    return NULL;
}
