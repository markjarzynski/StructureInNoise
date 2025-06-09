all: structure-in-noise

structure-in-noise: structure-in-noise.cpp include/*.h
	g++ -O2 -I include -o structure-in-noise structure-in-noise.cpp

rng2img: rng2img.cpp include/*.h
	g++ -O2 -I include -o rng2img rng2img.cpp

quicktest:
	for i in city esgtsa_linear esgtsa_nested iqint1_linear iqint1_nested iqint2 iqint3 jkiss32_linear jkiss32_nested lcg_linear lcg_nested md5 murmur3 pcg3d pcg4d ranlim32_linear ranlim32_nested superfast tea2 tea3 tea4 tea5 wang_linear wang_nested xorshift128 xorshift32_linear xorshift32_nested xxhash32 hybridtaus "-f /dev/random"; \
    do \
        ./structure-in-noise -r -w 32 -h 32 -k 3 $$i; \
    done

testdevrandom:
	./structure-in-noise -w 128 -h 128 -k 3 -f devrandom

.PHONY: convert archive clean clobber
convert:
	for i in $(wildcard *.ppm) ; \
	do \
		convert $$i $$(basename $$i .ppm).png ; \
	done

archive:
	mkdir -p results
	mv *.png results

clean:
	rm -rf *.ppm *.png

clobber: clean
	rm -rf structure-in-noise rng2img
