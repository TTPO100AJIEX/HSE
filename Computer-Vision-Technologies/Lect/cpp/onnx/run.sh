rm -rf build
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=./onnxruntime ..
cmake --build .
./main ../../../models/$1.onnx $2