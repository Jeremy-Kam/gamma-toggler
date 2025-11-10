#include <windows.h>
#include <cmath>

// Stores the original LUT to restore
static WORD OriginalRamp[3][256];
static bool OriginalSaved = false;

static double new_gamma = 2.19;

// Saves the current LUT only once
void SaveOriginalLUT(HDC hdc) {
    if (!OriginalSaved) {
        GetDeviceGammaRamp(hdc, OriginalRamp);
        OriginalSaved = true;
    }
}

// Restores the original LUT
void RestoreOriginalLUT(HDC hdc) {
    if (OriginalSaved) {
        SetDeviceGammaRamp(hdc, OriginalRamp);
    }
}

// Applies a custom gamma curve
void ApplyGamma(double gamma) {
    HDC hdc = GetDC(NULL);

    SaveOriginalLUT(hdc); // capture original once

    WORD ramp[3][256];

    for (int i = 0; i < 256; i++) {
        double value = pow(i / 255.0, 1.0 / gamma) * 65535.0;
        if (value > 65535) value = 65535;
        ramp[0][i] = (WORD)value;
        ramp[1][i] = (WORD)value;
        ramp[2][i] = (WORD)value;
    }

    SetDeviceGammaRamp(hdc, ramp);
    ReleaseDC(NULL, hdc);
}

// No-console entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    // Save original LUT at startup
    HDC hdc = GetDC(NULL);
    SaveOriginalLUT(hdc);
    ReleaseDC(NULL, hdc);

    bool usingNewGamma = false;

    while (true) {

        // Toggle between original and new_gamma
        if (GetAsyncKeyState(VK_F9) & 1) {
            usingNewGamma = !usingNewGamma;
            if (usingNewGamma)
                ApplyGamma(new_gamma);      // your requested gamma
            else {
                HDC hdc2 = GetDC(NULL);
                RestoreOriginalLUT(hdc2);  // restore system's gamma
                ReleaseDC(NULL, hdc2);
            }
        }

        // Reset to 1 gamma on F11 (usual default)
        if (GetAsyncKeyState(VK_F11) & 1) {
            usingNewGamma = false;
            ApplyGamma(1.0);
        }

        // Exit program on F12 (restore original LUT)
        if (GetAsyncKeyState(VK_F12) & 1) {
            break;
        }

        Sleep(30);
    }

    // Restore original LUT on exit
    hdc = GetDC(NULL);
    RestoreOriginalLUT(hdc);
    ReleaseDC(NULL, hdc);

    return 0;
}
