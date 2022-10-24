all:
	@clear;
	cmake -S . -B build/
	make -C build/
	@./bin/sat-colision
