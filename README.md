# SpritePaint_Dos
A basic sprite paint program for Dos written in WatcomC++.

try: _sprite.exe it's the compiled program.

![ScreenShot](https://raw.github.com/kosmonautdnb/SpritePaint_Dos/blob/main/DESC.PNG)

A video of using this tool: https://www.youtube.com/watch?v=1TvwSr7-zCg

A 25 min video with a tutorial/howto: https://www.youtube.com/watch?v=8JZ11G1h4Mw

I hope your computer supports Vesa 640x480x32.

If you want to recompile it with WatcomC++. Also depack _build.zip in this folder and place the ramdisk in your autoexec.bat.

There is an example config.sys and autoexec.bat in _build.zip supplied.

### Sprite FileFormat
BYTE 'S','P',Width,Height

DWORDARRAY[Width*Height] rgbaPixelData

### SpriteSheet FileFormat
BYTE 'S','S'

SHORT spriteCount

BYTEARRAY[10] Name

BYTE 'S','P',Width,Height

DWORDARRAY[Width*Height] rgbaPixelData

BYTEARRAY[10] Name

BYTE 'S','P',Width,Height

DWORDARRAY[Width*Height] rgbaPixelData

...

### Palette Format

DWORDARRAY rgbacolors (max. 256 colors)

## You can create a bootable USB-Stick with FreeDos with Rufus

https://rufus.ie

FAT32 max 32GB, FreeDos. The MS-Dos Systemfiles are hidden in Windows but the Autoexec.bat etc. are written to the stick.

You have to disable secure boot and enable Legacy or CSM booting.

## You may need this change to your config.sys

DEVICE=C:_DOS_\BIN\HIMEMX.exe

DEVICE=C:_DOS_\BIN\JEMM386.exe /SB /MAX=262144

/MAX=262144 means 256MB may be available.

You may compile it with Dos4GW (instead of PMode/w) but Dos4GW only supports 32MB of memory. (No memory checks are implemented in the sources.)
