# remember to actually set these..
export SPLAT_DIR=~/MSVC/Splat
export TOOLCHAIN_REPO=~/MSVC/Toolchain

export HOST_ARCH=x86
export MSVC_BASE=$SPLAT_DIR/crt
export WINSDK_BASE=$SPLAT_DIR/sdk
# TODO: get this from the thing..
export WINSDK_VER=10.0.20348
# change this to your llvm version!!!
export LLVM_VER=12
export CLANG_VER=$LLVM_VER

mkdir -p build

# you can also use `-G Ninja` here
cmake \
  -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_REPO/clang-cl-msvc.cmake \
  -DCMAKE_BUILD_TYPE=Release -B build
  
cmake --build build --config Release
cp build/CrossTemplate.dll "/home/dog/.wine/drive_c/Program Files (x86)/VOIDSERVER/GDVS/gdvs-dll/techno.21.dll"
