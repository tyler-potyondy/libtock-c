#!/usr/bin/env bash

## NOTE: this script expects to be invoked from the ot-tock base directory. ## 

# Create build directory and invoke CMake to create and execute build system. We specify that
# the platform used will be external (Tock specific). For now, we specify the multiple
# instance flag. This may change in future iterations of this script.
mkdir -p build && cd build && cmake .. -DOT_PLATFORM=external -DOT_MULTIPLE_INSTANCE="multiple instances" -Wno-dev  && cmake --build . --target openthread-ftd && cd .. 

# Specify the directory containing the .a files
library_directory="build/openthread"

# Specify where libraries will be placed to follow Tock's external library structure. This
# structure is specified in:
# [https://github.com/tock/libtock-c/blob/master/doc/compilation.md].
TEMPLATE_BUILD_DIR="build/cortex-m4"
DST_DIR="build/lib"

libs=("libopenthread-ftd" "libmbedx509" "libmbedcrypto" "libmbedtls" "libtcplp-ftd")
for lib in "${libs[@]}"; do
	mkdir -p "build/lib/$lib/$TEMPLATE_BUILD_DIR"
done

# Copy each static library from the openthread build to the Tock external library directory

# libopenthread-ftd
cp $library_directory/src/core/libopenthread-ftd.a $DST_DIR/libopenthread-ftd/build/cortex-m4

# libmbedx509
cp $library_directory/third_party/mbedtls/repo/library/libmbedx509.a $DST_DIR/libmbedx509/build/cortex-m4

# libmbedcrypto
cp $library_directory/third_party/mbedtls/repo/library/libmbedcrypto.a $DST_DIR/libmbedcrypto/build/cortex-m4

# libmedtls
cp $library_directory/third_party/mbedtls/repo/library/libmbedtls.a $DST_DIR/libmbedtls/build/cortex-m4

# libtcplp
cp $library_directory/third_party/tcplp/libtcplp-ftd.a $DST_DIR/libtcplp-ftd/build/cortex-m4


