#!/bin/bash
debs="g++ cmake git libboost1.74-all-dev"
sudo apt install $debs

git clone https://github.com/mm-s/restc-cpp
cd restc-cpp
git checkout marc-os
mkdir _build
cd _build
cmake -DRESTC_CPP_USE_CPP20=ON -DRESTC_CPP_PIC=ON ..
sudo make install

exit 0

