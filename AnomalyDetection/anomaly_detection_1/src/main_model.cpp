/*
 * Copyright (c) 2017, Thibaud Ehret <ehret.thibaud@gmail.com>
 * All rights reserved.
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later
 * version. You should have received a copy of this license along
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include <string>
#include <sstream>
#include <sys/stat.h>

#include "Utilities/LibImages.h"
#include "Utilities/cmd_option.h"
#include "Utilities/PatchManager/patchManager.h"
#include "Utilities/nfa.h"
#include "Utilities/PartitionTree/prmTree.h"
#include "Utilities/PartitionTree/vptree.h"
#include "model/model.h"

// Define the fixed parameters here
#define NBTREES 4
#define hsigma 10.

using namespace std;

/**
 * @file   main.cpp
 * @brief  Main executable
 *
 * @author THIBAUD EHRET  <ehret.thibaud@gmail.com>
 **/

int main(int argc, char **argv)
{
	clo_usage("Detect anomalies at the current scale");
	clo_help(" NOTE: Input (<) and output (>) sequences are specified by their paths in printf format.\n");

	//! Paths to input/output sequences
	using std::string;
	const string input_path = clo_option("-i"     , ""                , "< input image");
	const string ref_path   = clo_option("-ref"   , input_path.c_str(), "< reference image, if not provided input image is used");
	const string model_path = clo_option("-model" , "model.tiff"      , "> model image");
	const string res_path   = clo_option("-res"   , "residual.tiff"   , "> residual image");

	//! General parameters
	const bool same         = (bool) clo_option("-same"     , true , "< if input and reference images are the same images");

	//! Model parameters
	const int patch_size  = clo_option("-px",8, "> Spatial patch size");
	const int num_patches  = clo_option("-np",16, "> Number of similar patches");


	//! Check inputs
	if (input_path == "")
	{
		fprintf(stderr, "%s: no input image.\nTry `%s --help' for more information.\n",
				argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	if (patch_size == 0)
	{
		fprintf(stderr, "%s: the patch size needs to be positive.\nTry `%s --help' for more information.\n",
				argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	//! Declarations
	std::vector<float> candidate, reference;
	ImageSize imSize, refSize;
	loadImage(input_path.c_str(), candidate, imSize);
	loadImage(ref_path.c_str(), reference, refSize);

    if(imSize.nChannels != refSize.nChannels)
    {
		fprintf(stderr, "%s: the number of channels needs to be the same for the image and the reference.\nTry `%s --help' for more information.\n",
				argv[0], argv[0]);
		return EXIT_FAILURE;

    }

	//! Compute denoising default parameters
	Params prms;
	prms.sizePatch = patch_size;
	prms.nbTrees = NBTREES;
	prms.nSimilarPatches = num_patches;
	prms.same = same;
	prms.excR = 2*patch_size;
	prms.h = hsigma;

	std::vector<float> model(imSize.whc);
	std::vector<float> residual(imSize.whc);

	//! Run denoising algorithm
	computeModel(candidate, imSize, reference, refSize, model, prms);
	saveImage(model_path.c_str(), model, imSize);

	//! Compute the residual
	computeDiff(candidate, model, residual);
	saveImage(res_path.c_str(), residual, imSize);

	return EXIT_SUCCESS;
}
