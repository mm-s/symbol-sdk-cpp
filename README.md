![Symbol-cpp](./doc/symbolcpp_200.png)

# symbol-sdk-cpp

TL; DR: Foundation C++ library for:
  * using the NEM's Symbol Blockchain core (offline) algorithms, like key management and creating or signing transactions.
  * interacting with REST API nodes of the NEM's Symbol Blockchain.

## Why Symbol-sdk-cpp

The SDK Provides a foundation for C++ Application Development on the NEM's Symbol blockchain.

## High level components
```
           SYMBOL SDK component overview
┌────────────────────────────────────────────────┐
│                                                │
│           ┌──────────────────────────────────┐ │
│           │                                  │ │
│    core   │    ┌─────┐    use    ┌─────┐     │ │
│ (offline) │    │ lib │◄──────────┤ HMI │     │ │
│shared lib │    └─────┘           └─────┘     │ │
│           │        ▲               ▲         │ │
│           │        │               │         │ │
│           └────────┼───────────────┼─────────┘ │
│                    │  inheritance  │           │
│           ┌────────┼───────────────┼─────────┐ │
│           │        │               │         │ │
│    rpc    │    ┌───┴─┐           ┌─┴───┐     │ │
│ (online)  │    │ lib │◄──────────┤ HMI │     │ │
│shared lib │    └─────┘   use     └─────┘     │ │
│           │                                  │ │
│           └──────────────────────────────────┘ │
│                                                │
└────────────────────────────────────────────────┘
lib - model classes
classes for building applications

                   HMI Human-Machine-Interface
                   clases for console interaction
```


### Clone:
```sh
git clone https://github.com/nemtech/symbol-sdk-cpp
cd symbol-sdk-cpp
```

Configure will clone catapult-server and cherry-pick files from it.

### Configure
The configure.sh script has been tested on Debian / Ubuntu Linux.

install system-wide packages:
```
sudo scripts/configure.sh install system_reqs
```

Install manually the following library: (it is not yet included ubn configure.sh)
```
https://github.com/mm-s/restc-cpp 
```

Create the _build directory
builds and installs extra dependencies if needed.
```
scripts/configure.sh make
```

Note: For exploring more detailed functions access the main menu:
```
sudo scripts/configure.sh
```

### Build:
Once configured proceed to build the project.

```sh
mkdir _build; cd _build
cmake  ..
make
```
You can pass options to cmake to specify alternative locations of the restc-cpp library using
```
cmake -DRESTCCPP_ROOT=<path_to_restc-cpp_cpp> ..
```
Paths shall point to a prefix directory containing standard directories bin, lib and include.

## Install:
In order to install the library and clients applications in the configured install prefix type:
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
  * developer mohammad.jowkar@nem.software
  * you [apply^]


