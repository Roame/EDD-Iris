mkdir -p code/build
cd code/build
rm -r *
mkdir -p bin

cmake .. -D CMAKE_TOOLCHAIN_FILE="../Toolchain-rpi.cmake"
make