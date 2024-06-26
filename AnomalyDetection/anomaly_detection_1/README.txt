% Reducing Anomaly Detection in Images to Detection in Noise.

# ABOUT

* Author    : EHRET Thibaud <ehret.thibaud@gmail.com>
* Copyright : (C) 2018 IPOL Image Processing On Line http://www.ipol.im/
* Licence   : GPL v3+, see gpl.txt

Version 1.0, released on November 28, 2018

# OVERVIEW

This source code provides an implementation of the anomaly detection method developped in "Davy, Axel, et al. "Reducing Anomaly Detection in Images to Detection in Noise." 2018 25th IEEE International Conference on Image Processing (ICIP). IEEE, 2018.", which has been studied in details in http://www.ipol.im

# UNIX/LINUX/MAC USER GUIDE

The code is compilable on Unix/Linux and Mac OS. 

- Compilation. 
Automated compilation requires the cmake and make programs.

- Library. 
This code requires the libpng, libtiff, libjpeg and fftw3 library. The visualization tool also requires the python cairo library. The scipy python library is also needed.

- Image format. 
Only the TIFF, JPEG and PNG formats are supported. 
 
-------------------------------------------------------------------------
Usage:
1. Download the code package and extract it. Go to that directory. 

2. Configure and compile the source code using cmake and make. 
It is recommended that you create a folder for building:

UNIX/LINUX/MAC:
$ mkdir build; cd build
$ cmake ..
$ make

Binaries will be created in build/bin folder.

NOTE: By default, the code is compiled with OpenMP multithreaded
parallelization enabled (if your system supports it). 
The number of threads used by the code is defined in model/model.cpp.

3. Usage instruction:
Running "./anomaly_detection.sh image" computes the result using the method and parameters from "Davy, Axel, et al. "Reducing Anomaly Detection in Images to Detection in Noise." 2018 25th IEEE International Conference on Image Processing (ICIP). IEEE, 2018". The program "main" correspond to the core of the method, "./main --help" list all available options available when using it  as a stand-alone.

4. The "visu.py" produces a comprehensive visual result of the detections in a pdf file "detections.pdf" merging all scales and where the circle radius represents the detection scale and the color the strength of the detection (NFA). White corresponds to a weak detection (NFA test value between $2\times 10^{-3}$ and $10^{-2}$), cyan to a good detection (NFA between $4\times 10^{-8}$ and $2\times 10^{-3}$), green to a very strong detection (NFA between $8.10^{-21}$ and $4\times 10^{-8}$) and orange to an extremely salient detection (NFA smaller than $8\times 10^{-21}$). Red corresponds to the detection with lowest NFA..

5. This project contains the following source files:
	src/CMakeLists.txt
	src/main.cpp
	src/Utilities/iio.h
	src/Utilities/iio.c
	src/Utilities/freemem.c
	src/Utilities/LibImages.h
	src/Utilities/LibImages.cpp
	src/Utilities/cmd_option.h
	src/Utilities/comparators.h
	src/Utilities/decompose.py
	src/Utilities/convert_laplacian_to_gaussian.py
	src/Utilities/mt19937ar.h
	src/Utilities/mt19937ar.c
	src/Utilities/nfa.h
	src/Utilities/nfa.cpp
	src/Utilities/visu.py
	src/Utilities/PartitionTree/forestManager.h
	src/Utilities/PartitionTree/forestManager.cpp
	src/Utilities/PartitionTree/partitionTree.h
	src/Utilities/PartitionTree/prmTree.h
	src/Utilities/PartitionTree/vptree.h
	src/Utilities/PartitionTree/vptree.cpp
	src/Utilities/PatchManager/databasePatchManager.h
	src/Utilities/PatchManager/databasePatchManager.cpp
	src/Utilities/PatchManager/imagePatchManager.h
	src/Utilities/PatchManager/imagePatchManager.cpp
	src/Utilities/PatchManager/patchManager.h
	src/Utilities/PatchSearch/localPatchPropagation.h
	src/Utilities/PatchSearch/localRefinement.h
	src/Utilities/PatchSearch/localRefinement.cpp
    src/model/params.h
    src/model/model.h
    src/model/model.cpp


6. The files that have been reviewed for IPOL publication are
TODO ONCE REVIEW IS DONE


# ABOUT THIS FILE

Copyright 2018 IPOL Image Processing On Line http://www.ipol.im/

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.
