# GammaToggler

GammaToggler is a small Windows utility that toggles your monitor’s gamma level using a hotkey.  
It uses the Win32 GDI API (`SetDeviceGammaRamp`) and requires no external dependencies.

## Features
- Toggle gamma ON/OFF with a single hotkey
- Saves and restores the system’s original gamma ramp
- Extremely lightweight and portable
- No installation or background services

## Hotkeys
| Key | Action |
|-----|--------|
| **F3** | Toggle gamma ON/OFF |
| **F12** | Exit (restores original gamma) |

## How to Build
Requires MinGW or another GCC-based compiler.

### Installing MinGW
If you do not have MinGW installed, you can follow [this YouTube video](https://www.youtube.com/watch?v=DamuE8TM3xo) (not my video, but I have followed his guides for a while).

The short of it is:
1. go to install [MingGW](https://sourceforge.net/projects/mingw/files/).
2. Use all defaults until you get to a page with checkboxes. Make sure mingw-developer-tools, mingw32-base, mingw32-gcc-g++, msys-base are checked (it will check other things as well). 
3. Go to Installation -> Apply Changes. Then click Apply.
4. Edit the environment variables (using Windows search).
5. Edit Path, add new entries: ```C:\MinGW\bin``` and ```C:\MinGW\MSYS\1.0\bin```
6. To test if it worked, go to command prompt and type in ```g++ --version``` and you should see an output stating the version you have. If not, something went wrong and you should probably watch the video.

```bat
g++ GammaToggler.cpp -o GammaToggler.exe -lgdi32 -mwindows
```

## Usage
Originally developed for using it with ARC Raiders or other competitive video games with dark environments, where having a higher gamma can give you an advantage. Morally, this is in a grey area, but it doesn't mess with the game itself. All it does is change the display, 

## Night Vision Toggler
The extended version of this program. Also changes digital vibrance and contrast to the optimal Escape from Tarkov settings, but requires more dependencies and is harder to set up.
