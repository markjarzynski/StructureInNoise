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

#include "Utilities/cmd_option.h"
#include "Utilities/nfa.h"

#define R 2
#define l 2

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
	const string res_path   = clo_option("-res"   , "residual.tiff"   , "> residual image on which the detection will be done");
	const string og_path    = clo_option("-og"    , res_path.c_str()     , "< original image, uses the residual as default");
	const string pxlNFA_path= clo_option("-pxlnfa", "pixelNFA.tiff"   , "> image with the logNFA in each pixel");
	const string rdNFA_path = clo_option("-rdnfa" , "radiusNFA.tiff"  , "> image with the radius corresponding to the logNFA in each pixel");
	const int patch_size  = clo_option("-px",8, "> Spatial patch size");

	//! Check inputs
	if (res_path == "")
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
	std::vector<float> residual, original;
	ImageSize imSize, origSize;
	loadImage(res_path.c_str(), residual, imSize);
	loadImage(og_path.c_str(), original, origSize);

	//! Detection in the residual
	std::vector<float> pixelNFA(imSize.width*imSize.height);
	std::vector<float> radiusNFA(imSize.width*imSize.height);
	ImageSize nfaSize;
	nfaSize.width = imSize.width;
	nfaSize.height = imSize.height;
	nfaSize.nChannels = 1;
	nfaSize.wh = nfaSize.width * nfaSize.height;
	nfaSize.whc = nfaSize.width * nfaSize.height * nfaSize.nChannels;
	coloredNoiseStatistic(residual, imSize, R, l, pixelNFA, radiusNFA, origSize.width, origSize.height, patch_size/2);
	saveImage(pxlNFA_path.c_str(), pixelNFA, nfaSize);
	saveImage(rdNFA_path.c_str(), radiusNFA, nfaSize);

	return EXIT_SUCCESS;
}
