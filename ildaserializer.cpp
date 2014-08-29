#include "ildaserializer.h"
#define ILDAHEADER 'I', 'L', 'D', 'A', 0x0, 0x0, 0x0
#define COMPANYNAME { 'A', 'N', 'D', 'R', 'E', 'W', '.', 'S' }
#define reverse16(i) { (i & 0xff00) >> 8, i & 0xff }

using namespace std;

ILDASerializer::ILDASerializer()
{
}

unsigned char * coordinateHeader(u_int16_t totalPoints, u_int16_t totalFrames, u_int16_t frameNo) {
    unsigned char output[32];
    unsigned char header[8] = {ILDAHEADER, 0x1};
    unsigned char frameName[8] = {'F', 'R', 'M', '0', '0', '0', '0', '0'};
    unsigned char companyName[8] = COMPANYNAME;


    unsigned char totalPointsc[2] = reverse16(totalPoints);
    string frameNos = to_string(frameNo);
    unsigned char frameNoc[2] = reverse16(frameNo);
    unsigned char totalFramesc[2] = reverse16(totalFrames);

    copy(&frameNos[0], &frameNos[frameNos.length()], &frameName[3 + (5 - frameNos.length())]);
    copy(header, &header[8], &output[0]);
    copy(frameName, &frameName[8], &output[8]);
    copy(companyName, &companyName[8], &output[16]);
    copy(totalPointsc, &totalPointsc[2], &output[24]);
    copy(frameNoc, &frameNoc[2], &output[26]);
    copy(totalFramesc, &totalFramesc[2], &output[28]);
    output[30] = 0;
    output[31] = 0;
    return output;
}

vector<unsigned char> * ILDASerializer::coordinates() {
    vector<unsigned char> output;
    uint outputPos = 0;
    u_int16_t totalFrames = 100;
    for (int frame = 0; frame < totalFrames; frame++) {
        u_int16_t totalPoints = 100;
        unsigned char * header = coordinateHeader(totalPoints, totalFrames, frame);
        unsigned char points[totalPoints*6] = {};
        for (int point = 0; point < totalPoints; point++) {
            __int16_t x = 'X' << 8 | 'x';
            __int16_t y = 'Y' << 8 | 'y';
            __int16_t status = 'S' << 8 | 's';

            unsigned char pointX[2] = reverse16(x);
            unsigned char pointY[2] = reverse16(y);
            unsigned char pointStatus[2] = reverse16(status);
            copy(pointX, &pointX[1], &points[point * 6]);
            copy(pointY, &pointY[1], &points[point * 6 + 2]);
            copy(pointStatus, &pointStatus[1], &points[point * 6 + 4]);
        }
        output.resize(output.size() + 32 + (totalPoints * 6));
        copy(header, header + 32, &output[outputPos]);
        outputPos += 32;
        copy(points, points + (totalPoints * 6), &output[outputPos]);
        outputPos += totalPoints * 6;
    }
    unsigned char * footer = coordinateHeader(0, 0, 0);
    output.resize(output.size() + 32);
    copy(footer, footer + 32, &output[0] + outputPos);
    return &output;
}

unsigned char * colourHeader(u_int16_t totalColours, u_int16_t paletteNumber) {
    unsigned char output[32];
    unsigned char header[8] = {ILDAHEADER, 0x2};
    unsigned char paletteName[8] = { 'P', 'L', 'T', '0', '0', '0', '0', '0' };
    unsigned char companyName[8] = COMPANYNAME;

    unsigned char totalColoursc[2] = reverse16(totalColours);
    string paletteNumbers = to_string(paletteNumber);
    unsigned char paletteNumberc[2] = reverse16(paletteNumber);

    copy(&paletteNumbers[0], &paletteNumbers[paletteNumbers.length()], &paletteName[3 + (5 - paletteNumbers.length())]);
    copy(header, &header[8], &output[0]);
    copy(paletteName, &paletteName[8], &output[8]);
    copy(companyName, &companyName[8], &output[16]);
    copy(totalColoursc, &totalColoursc[2], &output[24]);
    copy(paletteNumberc, &paletteNumberc[2], &output[26]);
    output[28] = 0;
    output[29] = 0;
    output[30] = 0; // Scanner head (0-255)
    output[31] = 0;

    return output;
}

vector<unsigned char> * ILDASerializer::colourTable() {
    vector<unsigned char> output;
    uint outputPos = 0;
    u_int16_t totalPalettes = 4;
    for (int palette = 0; palette < totalPalettes; palette++) {
        u_int16_t totalColours = 10;
        unsigned char * header = colourHeader(totalColours, palette);
        unsigned char colours[totalColours*3]= {};
        for (int colour = 0; colour < totalColours; colour++) {
            __uint8_t r = 'R';
            __uint8_t g = 'G';
            __uint8_t b = 'B';

            copy(&r, &r + 1, &colours[colour * 3]);
            copy(&g, &g + 1, &colours[colour * 3 + 1]);
            copy(&b, &b + 1, &colours[colour * 3 + 2]);
        }
        output.resize(output.size() + 32 + (totalColours * 3));
        copy(header, header + 32, &output[outputPos]);
        outputPos += 32;
        copy(colours, colours + (totalColours * 3), &output[outputPos]);
        outputPos += totalColours * 3;
    }
    return &output;
}
