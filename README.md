![Symbol-cpp](./doc/symbolcpp_200.png)

# symbol-sdk-cpp

TL; DR: Foundation C++ library for interacting with REST API nodes on the NEM's Symbol Blockchain.

## Why Symbol-sdk-cpp

The SDK Provides a foundation for C++ Application Development on the NEM's Symbol blockchain.

### Dependencies:
* nemtech/[symbol-sdk-core-cpp](https://github.com/nemtech/symbol-sdk-core-cpp "GitHub link")  -  Symbol algorithms.
* mm-s/[restc-cpp](https://github.com/mm-s/restc-cpp "GitHub link")  -  Patched original jgaa/[restc-cpp](https://github.com/jgaa/restc-cpp.

### Build:
Once configured proceed to build the project.

```sh
mkdir _build; cd _build
cmake  ..
make
```
You can pass options to cmake to specify alternative locations of the symbol-sdk-core using
```
cmake -DSYMBOLCORE_ROOT=<path_to_symbol_sdk_core_cpp> ..
```
Paths shall point to a directory containing standard directories bin, lib and include.

## Install:
In order to install the library and cliente application in the system type:
```sh
sudo make install
```

CMake accepts the useful definition for specifying the install target location.
```sh
cmake -DCMAKE_INSTALL_PREFIX=<target_directory> ..
```

## Project Governance:
Ships: 
* branch main: 
  * Release Manager TBD

* branch dev:
  * developer marcos.mayorga@nem.software
  * [apply^]



