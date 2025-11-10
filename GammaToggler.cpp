#include <windows.h>
#include <cmath>
#include <fstream>
#include <iostream>

static WORD OriginalLUT[3][256];
static bool OriginalSaved = false;

const char* STATE_FILE = "state.dat";
const char* LUT_FILE   = "original_lut.bin";

// Save LUT to file
void SaveLUTToFile(WORD lut[3][256]) {
    std::ofstream out(LUT_FILE, std::ios::binary);
    out.write((char*)lut, sizeof(WORD) * 3 * 256);
}

// Load LUT from file
bool LoadLUTFromFile(WORD lut[3][256]) {
    std::ifstream in(LUT_FILE, std::ios::binary);
    if (!in.good()) return false;
    in.read((char*)lut, sizeof(WORD) * 3 * 256);
    return true;
}

// Write modified=0 or modified=1
void WriteState(bool modified) {
    std::ofstream out(STATE_FILE);
    out << "modified=" << (modified ? "1" : "0") << "\n";
}

// Read state file
bool ReadState() {
    std::ifstream in(STATE_FILE);
    if (!in.good()) return false;

    std::string line;
    std::getline(in, line);
    return (line == "modified=1");
}

// Save current original LUT from system
void SaveOriginalLUT(HDC hdc) {
    if (!OriginalSaved) {
        GetDeviceGammaRamp(hdc, OriginalLUT);
        SaveLUTToFile(OriginalLUT);
        WriteState(false);
        OriginalSaved = true;
    }
}

// Restore LUT from memory and file
void RestoreOriginalLUT(HDC hdc) {
    WORD temp[3][256];
    if (LoadLUTFromFile(temp)) {
        SetDeviceGammaRamp(hdc, temp);
    }
    WriteState(false);
}

void ApplyGamma(double gamma) {
    HDC hdc = GetDC(NULL);
    SaveOriginalLUT(hdc);

    WORD ramp[3][256];

    for (int i = 0; i < 256; i++) {
        double value = pow(i / 255.0, 1.0 / gamma) * 65535.0;
        if (value > 65535) value = 65535;
        ramp[0][i] = ramp[1][i] = ramp[2][i] = (WORD)value;
    }

    SetDeviceGammaRamp(hdc, ramp);
    WriteState(true);
    ReleaseDC(NULL, hdc);
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE p, LPSTR cmd, int show) {

    // Detect crash from last run
    if (ReadState()) {
        HDC hdc = GetDC(NULL);
        RestoreOriginalLUT(hdc); // restore on program start
        ReleaseDC(NULL, hdc);
    }

    // Ready for normal operation
    bool usingGamma = false;

    while (true) {

        // Toggle gamma
        if (GetAsyncKeyState(VK_F9) & 1) {
            usingGamma = !usingGamma;
            if (usingGamma) ApplyGamma(2.19);
            else {
                HDC hdc = GetDC(NULL);
                RestoreOriginalLUT(hdc);
                ReleaseDC(NULL, hdc);
            }
        }

        // Exit on F12
        if (GetAsyncKeyState(VK_F12) & 1) break;

        Sleep(30);
    }

    // clean exit
    HDC hdc = GetDC(NULL);
    RestoreOriginalLUT(hdc);
    ReleaseDC(NULL, hdc);

    return 0;
}
