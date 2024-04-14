
# Overview

The primary goal of the anomaly detection test is to identify reoccuring patterns
in the noise images to gauge the strength of the hash algorithm used in generation.
As opposed to other methods being implemented in the project, the anomaly detection
tests will attempt to leverage a learning model to identify similarities in the 
input images. 

# Implementation
	
While the paper linked above is for anomoly detection with noise in the
background, I believe, with small tweaks, that this model could be
used to locate similar features in noise (if rare enough). Mainly, 
the implementation for our use case will pull the set feature extraction 
techniques described in the paper _"How to Reduce Anomaly Detection in Images to Anomaly
 Detection in Noise"_. 

The paper uses a a two part apprach to determining anomalous features in their
input data, a similar-feature extraction and statistical detection. Their input data
consists of repeated patterns painted across a background
of noise. Whereas, our use case for this algorithm is to identify lke structures in the
noise itself. The feature extraction technique in the paper compares and removes
statistically high occuring patterns which leads to the an output containing only those
anomalous regions (derived from a patch-based denoising algorithm). Ideally, we could
utilize this same method with a slight modification to empahsize similar regions rather
than removing them. With this, if structures or patterns occur more than one time in 
the image, whether rare or common depending on the strength of the hash image used, 
the model will include them in the final output as "anomalous" for our case.

In an ideal scenario, the images that we are testing will not contain high frequency of reapeating patterns, if any at all. Any hits may need to pass through other tests depending on the level of tuning that is possible, however in the worse case, this
approach should greatly reduce the search space.

## Key points in paper
- Utlizing Vantage Point (VP) trees for fast pattern lookup
- Gloabl search of the entire image against the patch, with the original patch location removed
- Avoided the use of small square regions for similar patch search
- Generate a residual image containing on the noise and anomaly
- Utlize statistical a contrario model to reduce false positives


# Development Notes (Temporary Section)

- A residual image with the patch removed is used to compare if the
patch has been seen in the rest of the image or not
- Will need to validate the statistical test aspect and see if
if could be removed or not
- Pull the parameters they used and see if it works for our 
implementation
- Need to look more into the VP trees used and how similar patches
are deemed as "similar"

(Might need some help with the math of it and possibly understanding
the tree structure that is being used)


# References

Implementing feature from patch extraction algorithm discussed in 
https://www.ipol.im/pub/art/2019/263/article_lr.pdf





