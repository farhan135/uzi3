# uzi3
Current Status: 
Project name: uzi$, $ sign represents current working version.
------------------------------------------------------------------------------------------
We can successfully load krnldr.sys into memory and load/execute krnldr.sys from memory. 
Working on krnldr.sys to load c kernel.
------------------------------------------------------------------------------------------
bootloader.img is the MBR. It lays out the FAT12 file system. Traverses through the filesystem
to find krnldr.sys 
bootloader.img then loads krnldr.sys into memory and starts executing krnldr.sys from memory.
------------------------------------------------------------------------------------------
krndr.sys is the second stage bootloader. Like bootloader.img (or the MBR), it lays out FAT12 file system. 
It loads gdt and enables a20. It searches for kernel.sys and loads that into memory. 
Then it jumps to that location in the memory, to execute kernel.sys which is above 1 MB mark.
-------------------------------------------------------------------------------------------
kernel.sys is developed in c programming language. This will be the ultimate kernel for the OS.
-------------------------------------------------------------------------------------------
--------------------------------
Prerequisites:
A debian based linux system. 
bochs: virtual os loader.
--------------------------------
Goal:
MakeFile1:
1. Modularizing FAT12 file system traversing, procedures.
2. krnldr.sys will prepare for kernel and load it into memory, sector by sector.
MakeFile2:
1. kernel.sys is the C kernel .
--------------------------------
BuildAndRun:
make -f MakeFile2 (only once)
make -f MakeFile1
sudo bochs -f bochsrc.txt
CleanUp:
sudo pskill -9 bochs
sudo umount /mnt/floppy
sudo losetup -d /dev/loop0
--------------------------------
To kill bochs, if not shutting down properly:
sudo pskill -9 bochs
--------------------------------
