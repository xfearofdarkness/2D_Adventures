# 2D Adventures

- [Installation and Build Instructions](#build)
    - [Linux](#linux)
- [Credits](#credits)
- [Licenses](#licenses)
- [Important Information](#info)

## Installation and Build Instructions
### Linux
Install CMake and Ninja with your package manager  
#### Ubuntu
Install dependencies. You don't need ninja if you don't want to use the script.
> sudo apt install build-essential git cmake ninja-build  

Clone the git repository
>git clone https://github.com/xfearofdarkness/2D_Adventures.git  

Install [raylib](https://www.raylib.com/) (optional) and **[its dependencies](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux) (required)**   

run 
> ./build_ninja_linux.sh  

 to build the game
### Windows
Download the installer from the releases tab and run it. Skip the [Windows Smart Screen warning](#info) and follow the installation steps. Start the application from the start menu or from the desktop shortcut. 
## Credits
Credit to Ramon Santamaria (@raysan5) for the raygui header in the src folder.

## Licenses

- **Main Project License:** [MIT License](LICENSE)
- **[raygui Header](src/raygui.h) License:** [zlib License](LICENSE_raygui.txt)

## Info
The current installer for Windows under the Releases tab may trigger a Microsoft SmartScreen warning. Unfortunately, I can't afford to buy a signing certificate, so if you want to know what the installer is doing, please review the [NSIS script](installer.nsi) included in the repository.