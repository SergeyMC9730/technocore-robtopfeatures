UPWD=$PWD

TASK_WINDOWS() {
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

  rm -rf ${UPWD}/build
  mkdir -p ${UPWD}/build

  # you can also use `-G Ninja` here
  cmake \
    -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_REPO/clang-cl-msvc.cmake \
    -DCMAKE_BUILD_TYPE=Release -B build
    
  cmake --build ${UPWD}/build --config Release
  cp ${UPWD}/build/CrossTemplate.dll "/home/dog/.wine/drive_c/Program Files (x86)/VOIDSERVER/GDVS/gdvs-dll/techno.21.dll"
}
TASK_ANDROID() {
  export NDK="/home/dog/ASDK/r17c"
  adb shell pm uninstall -k com.technogdps.geometryjump &
  rm -rf ${UPWD}/build_a ${UPWD}/out_a gd_a gdMOD.apk
  mkdir ${UPWD}/build_a
  mkdir ${UPWD}/out_a
  mkdir ${UPWD}/out_a/x86
  mkdir ${UPWD}/out_a/armeabi-v7a
  mkdir ${UPWD}/out_a/armeabi
  cd ${UPWD}/build_a
  cmake ${UPWD} -DCMAKE_ANDROID_NDK=$NDK -DCMAKE_ANDROID_ARCH_ABI=x86 -DANDROID=1 -G "Ninja"
  cmake --build ${UPWD}/build_a
  cd ${UPWD}
  mv ${UPWD}/out_a/libgdmod.so ${UPWD}/out_a/x86
  rm -rf ${UPWD}/build_a
  mkdir ${UPWD}/build_a
  cd ${UPWD}/build_a
  cmake ${UPWD} -DCMAKE_ANDROID_NDK=$NDK -DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a -DANDROID=1 -G "Ninja"
  cmake --build ${UPWD}/build_a
  cd ${UPWD}
  mv ${UPWD}/out_a/libgdmod.so ${UPWD}/out_a/armeabi-v7a
  rm -rf ${UPWD}/build_a
  mkdir ${UPWD}/build_a

  apktool d ${UPWD}/Binary/gd.apk
  mv ${UPWD}/gd ${UPWD}/gd_a
  cp -r ${UPWD}/out_a/* ${UPWD}/gd_a/lib/
  cp ${UPWD}/GeometryJump.smali ${UPWD}/gd_a/smali/com/robtopx/geometryjump/
  cp ${UPWD}/AndroidManifest.xml ${UPWD}/gd_a/
  apktool b ${UPWD}/gd_a
  cp ${UPWD}/gd_a/dist/gd.apk ${UPWD}/gdMOD.apk
  rm -rf ${UPWD}/gd_a &
  java -jar ${UPWD}/Binary/uber-apk-signer-1.2.1.jar --apks ${UPWD}/gdMOD.apk
  rm ${UPWD}/gdMOD.apk
  mv ${UPWD}/gdMOD-aligned-debugSigned.apk gdMOD.apk
  rm -rf ${UPWD}/build_a ${UPWD}/out_a &

  adb install ${UPWD}/gdMOD.apk
  adb shell am start -a android.intent.action.MAIN -n com.technogdps.geometryjump/com.technogdps.geometryjump.GeometryJump
}

TASK_WINDOWS &
TASK_ANDROID