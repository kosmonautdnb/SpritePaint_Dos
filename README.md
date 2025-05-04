# SpritePaint_Dos
A basic sprite paint program for Dos written in WatcomC++.

![ScreenShot](https://raw.github.com/kosmonautdnb/SpritePaint_Dos/main/DESC.PNG)

try: _sprite.exe it's the compiled program.

I hope your computer supports Vesa 640x480x32.

If you want to recompile it with WatcomC++. Also depack _build.zip in this folder and place the ramdisk in your autoexec.bat.

There is an example config.sys and autoexec.bat in _build.zip supplied.

## You can create a bootable USB-Stick with FreeDos with Rufus

https://rufus.ie/de/

FAT32 max 32GB, FreeDos. The MS-Dos Systemfiles are hidden in Windows but the Autoexec.bat etc. are written to the stick.

You have to disable secure boot and enable Legacy or CSM booting.

## You may need this change to your config.sys

DEVICE=C:_DOS_\BIN\HIMEMX.exe

DEVICE=C:_DOS_\BIN\JEMM386.exe /SB /MAX=262144

/MAX=262144 means 256MB may be available.
