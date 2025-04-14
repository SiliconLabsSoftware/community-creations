# Silicon Labs CircuitPython Applications #

This repository contains example projects that demonstrate various applications using CircuitPython on Silicon Labs Development Kits.
All examples in this repository are considered to be EXPERIMENTAL QUALITY, which implies that the code provided in the repository has not been formally tested and is provided as-is. It is not suitable for production environments.

## Get CircuitPython firmware ##

Official binaries for all Silicon Labs supported boards are available through
[circuitpython.org/downloads](https://circuitpython.org/downloads?q=silabs). The site includes stable, unstable and
continuous builds. Full release notes are available through
[GitHub releases](https://github.com/adafruit/circuitpython/releases) as well.


> **_NOTE:_** The examples in this repository require CircuitPython v8.2.0 or higher.

## Project Examples ##
[Circuitpython Applications Examples](https://github.com/SiliconLabsSoftware/circuitpython_applications)

## Project structure

* **device_root**
  * This folder contains all of the required files to run the project. The files and folders should be copied into the root folder of the device.
  * **lib**
    * The lib folder contains precompiled libraries provided by Adafruit, this folder provides the application logic related files too. These libraries and files are required to run the project.


Ensure that the files and folders from the device_root folder are copied into the root of the target board's file system. 


## Documentation ##

Getting started with [CircuitPython on EFR32 boards](doc/running_circuitpython.md).

For more information, visit the [Developer documentation](https://docs.silabs.com/application-examples/latest/) portal of Silicon Laboratories.

## Reporting Bugs/Issues and Posting Questions and Comments ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of this repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of this repo.