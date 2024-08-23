# CS2 Kernel Mode cheat
*Just my attempt to break CS2 in kernel mode app for win32/64*

# Brief information 
## Repository health status

| Branch | Health |
| ------ | ------ |
| main | ![Main branch - latest push](https://github.com/kbrddestroyer/CS2.KernelCheat/actions/workflows/msbuild.yml/badge.svg?branch=main)
| Release | ![Release branch](https://github.com/kbrddestroyer/CS2.KernelCheat/actions/workflows/msbuild.yml/badge.svg?branch=release)
| Tag build | ![Latest tag](https://github.com/kbrddestroyer/CS2.KernelCheat/actions/workflows/msbuild_release.yml/badge.svg?branch=release) |

## Socials

[Discord](https://discord.com/invite/FQvZhSeHrr)

# Disclaimer 

I am **NOT** responsible for your system crashes, VAC blocks, etc. This software is educational purposes only.
I just want to show that VAC still doesn't work properly and anyone with coding skills can break Counter Strike game. I am not supporting game hacking, cheating, exploiting etc. #fixcs2

# Credits

## Tools & Libraries

- [dear ImGui](https://github.com/ocornut/imgui) and d3d9 for backend as GUI framework
- [kdmapper](https://github.com/TheCruZ/kdmapper) added as usermode library
- [a2x's](https://github.com/a2x/cs2-dumper) offsets dumper

## Tutorials & other information 

- [cazzwastaken](https://youtube.com/@cazz?si=juMpTGDWsf86qhqw) and his [YouTube tutorial for basic kernel driver](https://youtu.be/n463QJ4cjsU?si=QQDnUOpgAzZ1oLV3)

## Dependencies

- Windows Driver Kit [*Install Guide*](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk)
- Windows SDK
- [DirectX SDK](https://www.microsoft.com/ru-ru/download/details.aspx?id=6812)

---

# Installation guide

Currently the installation section is in project wiki tab, all releases can be found [here](https://github.com/kbrddestroyer/CS2.KernelCheat/releases)

# Project build

## Building usermode and kernelmode binaries

- Install Windows SDK and Windows Driver Kit by following the [official tutorial](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk)
- Install [DirectX SDK](https://www.microsoft.com/ru-ru/download/details.aspx?id=6812)
- Clone the repository and whitelist it folder in your antivirus software
- Run Microsoft Visual Studio 2022 and open the project
- That should be enough, but you can tweak project settings altering project directories, etc.
- Run build from VS2022 GUI or by `msbuild` command

## Known issues
-`libucrtd.lib(checkcfg.obj) : error LNK2001: unresolved external symbol guard_check_icall$fo$` - remove security checks from compilation configuration 

## Mapping the driver

If native mapping from usermode.exe does not work or leads to system failure you can follow this steps:
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

