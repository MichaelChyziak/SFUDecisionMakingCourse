# SFU ENSC 482: Michael Chyziak

The files located in each directory correspond to a homework assignment that was given to work on for the second submission set.

## Setup

These commands will allow all source code to be compiled and ran without any issues. The projects should run on most Linux environments but was only tested in Ubuntu 18.04.
Some executables might require the latest version of the following tools so it is recommended to update them to the latest if already installed.
```
> sudo apt-get update
> sudo apt-get install freeglut3
> sudo apt-get install freeglut3-dev
> sudo apt-get install binutils-gold
> sudo apt-get install g++ cmake
> sudo apt-get install libglew-dev
> sudo apt-get install g++
> sudo apt-get install mesa-common-dev
> sudo apt-get install build-essential
> sudo apt-get install libglew1.5-dev libglm-dev
> sudo apt-get install mesa-utils
> sudo apt-get –f install
```
For folders "homework_6" and "homework_7" this additional command is required to use FLTK (version 1.3).
```
> sudo apt install fltk1.3-dev
```

## SUBMISSION FILES
The following is the heirarchy for the files contained for this homework set.
README.pdf files for each homework question provide further detail of each question and an example demo.
```
 |
 |->homework_1->|
 |              |->histogram --> Executable file
 |              |->histogram.cc --> Source code
 |              |->poker-hand-testing.data --> Supplementary data (dataset)
 |              |->README.pdf --> README
 |->homework_2->|
 |              |->scatterplot --> Executable file
 |              |->scatterplot.cc --> Source code
 |              |->wine.data --> Supplementary data (dataset)
 |              |->README.pdf --> README
 |->homework_3->|
 |              |->least_squares --> Executable file
 |              |->least_squares.cc --> Source code
 |              |->wine.data --> Supplementary data (dataset)
 |              |->README.pdf --> README
 |->homework_4->|
 |              |->README.pdf --> README
 |->homework_5->|
 |              |->README.pdf --> README
 |->homework_6->|
 |              |->sfu_graph --> Executable file
 |              |->sfu_graph.cc --> Source code
 |              |->Burnaby_campus_cropped.png --> Supplementary data (image)
 |              |->README.pdf --> README
 |->homework_7->|
 |              |->dijkstra --> Executable file
 |              |->dijkstra.cc --> Source code
 |              |->google_map.png --> Supplementary data (image)
 |              |->README.pdf --> README
 |->README --> This file
```

Further explanations and assignment details/answers are provided by a "README.pdf" in each "homework_X" directory.
