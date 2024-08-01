# CS2 Kernel Mode cheat
*Just my attempt to break CS2 in kernel mode app for win32/64*

**Repo health**
![Main branch - latest push](https://github.com/kbrddestroyer/CS2.KernelCheat/blob/main/.github/workflows/msbuild.yml/badge.svg?branch=main)

[Discord](https://discord.com/invite/FQvZhSeHrr)

# Disclaimer 

I am **NOT** responsible for your system crashes, VAC blocks, etc. This software is educational purposes only.

## I just want to show that VAC still doesn't work properly and anyone with coding skills can break Counter Strike game. I am not supporting game hacking, cheating, exploiting etc.

#fixcs2 #fixcsgo

# Credits
## Used software
- dear ImGui 
- [kdmapper](https://github.com/TheCruZ/kdmapper) added as usermode library
- patched kdmapper that I used in development 
- a2x and cs2 offsets dumper
- cazzwastaken and YouTube tutorial for basic KernelMode application 
## Active contributors
- [Leksa667](https://github.com/Leksa667)

# Requirements
- [KDMapper](https://github.com/TheCruZ/kdmapper)
- Windows Driver Kit [*Install Guide*](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk)
- Windows SDK

### Project build
> Project building from source code requires dearImGui library, that can be installed from [here](https://github.com/ocornut/imgui)

# Install
All up-to-date releases can be found [here](https://github.com/kbrddestroyer/CS2.KernelCheat/releases)

## Mapping driver
1. Download kdmapper release
2. Whitelist kdmapper/project directory for your antimalware software
3. Download cheat release from *Releases* tab
4. Drag kernelmode.sys in kdmapper.exe
If there're no errors - you're done installing

**Please note that you need to map kernelmode.sys every time after your PC restart**

# Usage

Please, make sure that project directory is whitelisted.
- Run usermode.exe as Administrator 
- Run cs2.exe 
- Press **Attach** button

> GUI is in development, this section will be written soon

# Modding & Configuring

If you want to contribute, please read [contribution guideline](CONTRIBUTING.md)

> I want to add custom GUI layouts, GUI elements customisation (e.g. Radarhack entity customisation). Also I want to add cheat configs w/ all the settings stored.

> This section will be added soon

