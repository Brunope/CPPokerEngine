subsystem:
	cd core && $(MAKE)
	cd ui && qmake && make

test:
	cd core && $(MAKE) test

clean:
	cd core && make clean
	cd ui && make clean
