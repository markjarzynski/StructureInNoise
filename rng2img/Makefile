build: rng2img.cpp
	g++ -o rng2img -I include rng2img.cpp

quadrants: build
	for i in city esgtsa_linear esgtsa_nested iqint1_linear iqint1_nested iqint2 iqint3 jkiss32_linear jkiss32_nested lcg_linear lcg_nested md5 murmur3 pcg3d pcg4d ranlim32_linear ranlim32_nested superfast tea2 tea3 tea4 tea5 wang_linear wang_nested xorshift128 xorshift32_linear xorshift32_nested xxhash32 hybridtaus; \
	do \
		./rng2img -q $$i ; \
	done

bitplanes: build
	for i in city esgtsa_linear esgtsa_nested iqint1_linear iqint1_nested iqint2 iqint3 jkiss32_linear jkiss32_nested lcg_linear lcg_nested md5 murmur3 pcg3d pcg4d ranlim32_linear ranlim32_nested superfast tea2 tea3 tea4 tea5 wang_linear wang_nested xorshift128 xorshift32_linear xorshift32_nested xxhash32 hybridtaus; \
	do \
		./rng2img -b $$i ; \
	done

all: build
	for i in city esgtsa_linear esgtsa_nested iqint1_linear iqint1_nested iqint2 iqint3 jkiss32_linear jkiss32_nested lcg_linear lcg_nested md5 murmur3 pcg3d pcg4d ranlim32_linear ranlim32_nested superfast tea2 tea3 tea4 tea5 wang_linear wang_nested xorshift128 xorshift32_linear xorshift32_nested xxhash32 hybridtaus; \
	do \
		./rng2img $$i ; \
	done

.PHONY: convert clean clobber
convert:
	for i in *.ppm ; \
	do \
		convert $$i $$(basename -s .ppm $$i).png ; \
	done

clean:
	rm -f rng2img *.ppm

clobber: clean
	rm -f *.png
