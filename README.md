# CS2-Kernel-Mode-Custom-Edit-from-kbrddestroyer

Attempt to improve and contribute to the development of the kbrddestroyer project on CS2 in Kernel. Still needs improvement, this is for educational purposes obviously! Be careful with your use and legal or ethical prerogatives, cheating is not good.

**FORK FROM:** [kbrddestroyer/CS2.KernelCheat](https://github.com/kbrddestroyer/CS2.KernelCheat)

## CS2 Kernel Mode Cheat

Just my attempt to break CS2 in kernel mode app for win32/64.

### Disclaimer

I am **NOT** responsible for your system crashes, VAC blocks, etc. This software is for educational purposes only.

I just want to show that VAC still doesn't work properly and anyone with coding skills can break Counter-Strike game. I am not supporting game hacking, cheating, exploiting, etc. 
**#fixcs2 #fixcsgo**

## Credits

- dear ImGui
- kdmapper project
- patched kdmapper that I used in development
- a2x and cs2 offsets dumper
- cazzwastaken and YouTube tutorial for basic KernelMode application

## Requirements

- KDMapper
- Windows Driver Kit
- Windows SDK

## Install Guide

### Project Build

Project building from source code requires dearImGui library, that can be installed from [here](https://github.com/ocornut/imgui).

### Install

All up-to-date releases can be found [here](https://github.com/your-repo/releases).

### Mapping Driver

1. Download kdmapper release
2. Whitelist kdmapper/project directory for your antimalware software
3. Download cheat release from Releases tab
4. Drag `kernelmode.sys` into `kdmapper.exe`
5. If there're no errors - you're done installing

*Please note that you need to map `kernelmode.sys` every time after your PC restarts.*

## Usage

1. Please, make sure that the project directory is whitelisted.
2. Run `usermode.exe` as Administrator
3. Run `cs2.exe`
4. Press `Attach` button

*GUI is in development, this section will be updated soon.*

## Modding & Configuring

If you want to contribute, please read the [contribution guideline](CONTRIBUTING.md).

I want to add custom GUI layouts, GUI elements customization (e.g., Radarhack entity customization). Also, I want to add cheat configs with all the settings stored.

*This section will be added soon.*

So I have made the ImGui :)
