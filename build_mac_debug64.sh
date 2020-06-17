# Build AWS SDK
BITNESS=64

# Compare Bitness for 32
# $ARCH="Win32"

cd src
git clone -b "1.7.329" "https://github.com/aws/aws-sdk-cpp.git"
cd ..

prefix_path=$(pwd)
mkdir cmake-build${BITNESS}
cd cmake-build${BITNESS}
cmake ../src -D CMAKE_INSTALL_PREFIX=${prefix_path}/AWSSDK/ -D CMAKE_BUILD_TYPE=Debug -D BUILD_ONLY="core" -D CUSTOM_MEMORY_MANAGEMENT="OFF" -D ENABLE_RTTI="OFF" -D ENABLE_TESTING="OFF"
cd ..
# -DCMAKE_EXE_LINKER_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_C_FLAGS=-m32


# # Configure Project
# cmake -S src -B cmake-build${BITNESS} -A Win32 -D CMAKE_INSTALL_PREFIX=sdk-build${BITNESS}\AWSSDK\ -D BUILD_WITH_TESTS=ON

# # Build Project
cmake --build ./cmake-build${BITNESS}

# msbuild cmake-build32\PACKAGE.vcxproj -p:Configuration=Debug