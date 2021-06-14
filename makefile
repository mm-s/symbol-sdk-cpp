all:
	scripts/configure.sh
	mkdir -p _build
	cd _build; cmake ..
	echo "_build"

clean: cmake_clean
	rm catapult-server -rf
	rm _build -rf


cmake_clean:
	cd _build; make clean;

.phony: clean cmake_clean
