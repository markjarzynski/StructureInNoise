#!/bin/bash

hashes=(city_2 city_3 city_4 esgtsa_lincomb esgtsa_linear esgtsa_nested fast hybridtaus ign iqint1_lincomb iqint1_linear iqint1_nested iqint2 iqint2.y iqint2.z iqint3 jkiss32 lcg_lincomb lcg_linear lcg_nested lookup3 lookup3.y lookup3.z lowbias32_lincomb lowbias32_linear lowbias32_nested md5 md5.w md5.y md5.z md5_2 md5_2.w md5_2.y md5_2.z md5_3 md5_3.w md5_3.y md5_3.z murmur3_2 murmur3_3 murmur3_4 newintegerhash_lincomb newintegerhash_linear newintegerhash_nested one_at_a_time_2 one_at_a_time_3 one_at_a_time_4 pcg2d pcg2d.y pcg3d pcg3d.y pcg3d.z pcg3d16 pcg3d16.y pcg3d16.z pcg4d pcg4d.w pcg4d.y pcg4d.z pcg_lincomb pcg_linear pcg_nested pixar_randfloat prospector_best_lincomb prospector_best_linear prospector_best_nested pseudo pxq_lincomb pxq_linear pxq_nested ranlim32_lincomb ranlim32_linear ranlim32_nested superfast_2 superfast_3 superfast_4 tea2 tea2.y tea3 tea3.y tea4 tea4.y tea5 tea4.y trig wang_lincomb wang_linear wang_nested xorshift128 xorshift128.w xorshift128.y xorshift128.z xorshift32_lincomb xorshift32_linear xorshift32_nested xoshiro128plusplus xoshiro128starstar xxhash32_2 xxhash32_3 xxhash32_4) 

width=256
height=128

dir=images/bitplane

mkdir -p $dir

for hash in ${hashes[@]} ;
do
    ./rng2img -b -w $width -h $height $hash
    convert $hash.ppm $hash.bitplane.png
    rm $hash.ppm
    mv $hash.bitplane.png $dir
done

./rng2img -b -w $width -h $height -o devrandom.ppm -f /dev/random
convert devrandom.ppm devrandom.bitplane.png
rm devrandom.ppm
mv devrandom.bitplane.png $dir
