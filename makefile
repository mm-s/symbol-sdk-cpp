all: _buildd

core/catapult/NamespaceIdGenerator.cpp:
	scripts/configure.sh

_buildd: core/catapult/NamespaceIdGenerator.cpp
	mkdir -p _buildd
	cd _buildd && cmake -DCMAKE_BUILD_TYPE=Debug ..
	echo "_buildd"

clean: cmake_clean
	rm catapult-server -rf
	rm _buildd -rf
	rm core/catapult/NamespaceIdGenerator.cpp -f


cmake_clean:
	cd _buildd && make clean;

.phony: clean cmake_clean debug
