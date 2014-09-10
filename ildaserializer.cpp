#include "ildaserializer.h"
#include <string.h>
#define ILDAHEADER 'I', 'L', 'D', 'A', 0x0, 0x0, 0x0
#define COMPANYNAME { 'A', 'N', 'D', 'R', 'E', 'W', '.', 'S' }
#define reverse16(i) { (i & 0xff00) >> 8, i & 0xff }

using namespace std;

ILDASerializer::ILDASerializer()
{
}

colour_data colourData(__uint8_t r, __uint8_t g, __uint8_t b)
{
    colour_data output;
    output.r = r;
    output.g = g;
    output.b = b;
    return output;
}

coordinate_data ILDASerializer::setPoint(__int16_t x, __int16_t y, __int8_t colour, bool blanking) {
    coordinate_data output;
    output.x = x;
    output.y = y;
    output.colour = colour;
    output.blanking = blanking;
    return output;
}

char * coordinateHeader(u_int16_t totalPoints, u_int16_t totalFrames, u_int16_t frameNo) {
    char * output = new char[32];
    char header[8] = {ILDAHEADER, 0x1};
    char frameName[8] = {'F', 'R', 'M', '0', '0', '0', '0', '0'};
    char companyName[8] = COMPANYNAME;


    char totalPointsc[2] = reverse16(totalPoints);
    string frameNos = to_string(frameNo);
    char frameNoc[2] = reverse16(frameNo);
    char totalFramesc[2] = reverse16(totalFrames);

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

vector<char> ILDASerializer::coordinates(vector<vector<coordinate_data>> coords) {
    vector<char> output;
    unsigned int outputPos = 0;
    unsigned int totalFrames = coords.size();
    for (int frame = 0; frame < totalFrames; frame++) {
        unsigned int totalPoints = coords[frame].size();
        char * header = coordinateHeader(totalPoints, totalFrames, frame);
        //output.resize(output.size() + 32 + (totalPoints * 6));
        output.insert(output.end(), header, header + 32);
        //copy(&header[0], &header[32], &output[outputPos]);
        delete[] header;
        outputPos += 32;
        for (int pointIndex = 0; pointIndex < totalPoints; pointIndex++) {
            coordinate_data * point = &coords[frame][pointIndex];

            char pointX[2] = reverse16(-point->x); // X axis is inverted for some reason
            char pointY[2] = reverse16(point->y);
            char status[2] = { point->blanking << 6 | (pointIndex == totalPoints - 1) << 7, point->colour };

            output.insert(output.end(), pointX, pointX + 2);
            output.insert(output.end(), pointY, pointY + 2);
            output.insert(output.end(), status, status + 2);

            outputPos += 6;
        }
    }
    char * footer = coordinateHeader(0, 0, 0);
    output.insert(output.end(), footer, footer + 32);
    delete[] footer;
    return output;
}

char * colourHeader(u_int16_t totalColours, u_int16_t paletteNumber) {
    char * output = new char[32];
    char header[8] = {ILDAHEADER, 0x2};
    char paletteName[8] = { 'P', 'L', 'T', '0', '0', '0', '0', '0' };
    char companyName[8] = COMPANYNAME;

    char totalColoursc[2] = reverse16(totalColours);
    string paletteNumbers = to_string(paletteNumber);
    char paletteNumberc[2] = reverse16(paletteNumber);

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

vector<char> ILDASerializer::colourTable() {
    colour_data colourArr[6] = {colourData(0, 0, 0), colourData(255, 255, 255), colourData(255, 0, 0), colourData(0, 255, 0), colourData(0, 0, 255), colourData(255, 0, 255)};
    vector<char> output;
    unsigned int outputPos = 0;
    u_int16_t totalPalettes = 1;
    for (int palette = 0; palette < totalPalettes; palette++) {
        u_int16_t totalColours = sizeof(colourArr)/sizeof(colour_data);
        char * header = colourHeader(totalColours, palette);
        output.insert(output.end(), header, header + 32);
        delete[] header;
        outputPos += 32;
        for (int colour = 0; colour < totalColours; colour++) {
            output.push_back(colourArr[colour].r);
            output.push_back(colourArr[colour].g);
            output.push_back(colourArr[colour].b);
            outputPos += 3;
        }
    }
    return output;
}
