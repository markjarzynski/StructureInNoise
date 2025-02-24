all: bitmatch

match: match.cpp include/*.h
	gcc -O2 -I include -o match match.cpp

bitmatch: bitmatch.cpp include/*.h
	gcc -O2 -I include -o bitmatch bitmatch.cpp

test:
	for i in city esgtsa_linear esgtsa_nested iqint1_linear iqint1_nested iqint2 iqint3 jkiss32_linear jkiss32_nested lcg_linear lcg_nested md5 murmur3 pcg3d pcg4d ranlim32_linear ranlim32_nested superfast tea2 tea3 tea4 tea5 wang_linear wang_nested xorshift128 xorshift32_linear xorshift32_nested xxhash32 hybridtaus; \
    do \
        ./bitmatch ../rng2img/bitplanes/$$i.ppm ; \
    done

.PHONY: clean
clean:
	rm -rf match bitmatch
