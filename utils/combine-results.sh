hashes=(city_2 city_3 city_4 esgtsa_linear esgtsa_nested fast ign iqint1_linear iqint1_nested iqint2 iqint3 jkiss32_linear jkiss32_nested lcg_linear lcg_nested md5_2 md5_3 md5 murmur3_2 murmur3_3 murmur3_4 pcg_linear pcg_nested pcg2d pcg3d pcg3d16 pcg4d pseudo ranlim32_linear ranlim32_nested superfast_2 superfast_3 superfast_4 tea2 tea3 tea4 tea5 trig wang_linear wang_nested xorshift128 xorshift32_linear xorshift32_nested xxhash32_2 xxhash32_3 xxhash32_4 hybridtaus white black checkerboard devrandom)

dir=$1

for hash in ${hashes[@]} ;
do
	echo -n $hash
	for i in 1 2 3 4 5 6 7 8 ;
	do
		val=`cut -d ',' -f2 $dir/$i/$hash.txt`
		echo -n ",$val"
	done
	echo ""
done
