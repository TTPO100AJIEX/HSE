rm -rf build
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=./libtorch ..
cmake --build .
./main ../../../models/$1.pt $2