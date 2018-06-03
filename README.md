# DM42PGM - SwissMicros implementation of Free42 frontend 

Read the README file for building instructions.


# Architecture Overview
## Structure
The DM Calculator Platform (DMCP) is the OS for the DM42 calculator and comes as a binary file DMCP_flash_X.Y.bin.
The OS runs executable modules like Free42 which are called DM42-X.Y.Z.pgm.
Both binaries are found here [link to fw](www.swissmicros.com/dm42/firmware)
## DM42 Menus
The DM42 has two menus, the DMCP Menu and the System Menu.
The System Menu is the regular menu as known from previous versions, see https://www.swissmicros.com/dm42/doc/dm42_user_manual/
The DMCP Menu features several options:

![GitHub Logo](/dmcp_menu.jpg)
 1. "Program Info" display information of the program loaded in the CPU
 1. "Run Program" runs the program loaded in the CPU
 1. "Load Program" copies an executable module from the FAT partition to the flash memory of the CPU
 1. "Load QSPI from FAT" should not be needed
 1. "Settings" for Time and Date
 1. "Active USB Disk" puts the DM42 in USB mode for file copy
 1. "Enter System Menu" jumps to the main System Menu
 1. "About" displays general information

## Update procedure

1. Active USB disk in the setup menu
1. Copy both files from PC/MAC to the FAT partition of the DM42
1. Eject DM42 from PC/MAC
