# Multiconsult-ppcc
Software for processing load curve data.

## What is this?
This software processes load curve data that was obtained from power consumption measurements. The program allows to import different measurement files and categorize the data in different sectors and subcategories, that can also be customized (e.g. industrial-chemistry, commercial-education, etc.). After importing measurements there are various functions to analyze the data, for example to calculate the average of the measurements of specific days or categories, to visualize the calculated values in a diagram and to export the diagram either as a picture, an excel or a csv file.

The program is being developed in C++11 with QtCreator 6.0.1, using Qt 5.15.2 and the QXlsx library to read/write excel files. It was tested with Windows 10.

GitHub repo:
https://github.com/ErnestoVivas/Multiconsult-ppcc

## How to install and run
There are two methods to install the program:

### Executable file
If you obtained the folder containing the executable file and the corresponding libraries, just copy the folder to a desired location and run the program by executing ppcc.exe.

### Build from source
If you just have the source code you need a working installation of Qt5 first (the program was developed with Qt 5.15.2 but most Qt5 versions should be fine). The source code already contains the QXlsx header/cpp files so you won't need to download the library separately.
Open the project in Qt Creator and click on run, this will build and run the program. If you want to run the program from outside of QtCreator locate the executable file (ppcc.exe) in the generated build folder. To execute the program, following libraries need to be in the same directory as the the executable file:
- libgcc_s_seh-1.dll
- libstdc++-6.dll
- libwinpthread-1.dll
- Qt5Charts.dll
- Qt5Core.dll
- Qt5Gui.dll
- Qt5Widgets.dll
- platforms/
  - qdirect2.dll
  - qminimal.dll
  - qoffscreen.dll
  - qwindows.dll

The libraries are either part of the Qt or the MinGW installation, they can be found by searching in the windows file explorer.


## Format of the measurement files
The program imports excel files that contain the measurements. The provided excel files need to follow the following structure:

Date        | Time      | kW  
01/01/2001  | 00:00:00  | 100.0  
01/01/2001  | 00:15:00  | 150.6  
01/01/2001  | 00:30:00  | 120.0  
01/01/2001  | 00:45:00  | 110.5  
01/01/2001  | 01:00:00  | 100.5  
    .            .         .  
    .            .         .  
    .            .         .  
02/01/2001  | 00:00:00  | 104.2  
    .            .         .  
    .            .         .  
    .            .         .  

The date has to be in the format dd/mm/yyyy, the time has to be in 24 hour format. If the provided files are not in the specified format, the program will not be able to analyze the data correctly and might result in crash. The program does not check thouroughly if the input files are in the correct format. Also the program does not check the units and assumes all values to be in kW.