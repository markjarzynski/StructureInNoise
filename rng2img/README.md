RNG2IMG
=======

Convert a PRNG or hash function into a 2D image in the ppm format.

Usage
-----

rng2img will output hash.ppm

```
$ rng2img hash
```

Use ImageMagick to convert it to a PNG

```
$ convert hash.ppm hash.png
```
