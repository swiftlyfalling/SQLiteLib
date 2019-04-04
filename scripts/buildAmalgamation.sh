#!/bin/sh

# This script configures and generates the SQLite amalgamation

set -e

echo "Configure and generate SQLite amalgamation files"

pattern=" |'"
if [[ ${PROJECT_DIR} =~ $pattern ]]
    then
    echo "Unsafe PROJECT_DIR for make - contains spaces."
    echo "PROJECT_DIR=${PROJECT_DIR}"
    exit 1
fi

# Ensure we always run configure/make as if we were compiling for macOS (as parts of the
# configure/make process require building & running an executable)
#
# The generated sqlite3.c will be built for the correct platform by Xcode via the
# 'sqlitecustom' target
SDK_PLATFORM_NAME="macosx"
MACOSX_VERSION_MIN="$(sw_vers -productVersion | cut -d '.' -f 1,2)"


SDKROOT="$(xcrun --sdk $SDK_PLATFORM_NAME --show-sdk-path)"
CC="$(xcrun --sdk $SDK_PLATFORM_NAME -f clang)"
CXX="$(xcrun --sdk $SDK_PLATFORM_NAME -f clang++)"
CFLAGS="-arch x86_64 -isysroot $SDKROOT -mmacosx-version-min=$MACOSX_VERSION_MIN $OTHER_CFLAGS"
CXXFLAGS=$CFLAGS
export CC CXX CFLAGS CXXFLAGS

mkdir -p "${DERIVED_SOURCES_DIR}"
cd "${DERIVED_SOURCES_DIR}"

echo "Clean-up previous files if necessary"
[ -e Makefile ] && make distclean

echo "Configure makefile"
"${PROJECT_DIR}/sqlite/configure" --srcdir="${PROJECT_DIR}/sqlite/"

echo "Make SQLite amalgamation"
make sqlite3.c

# Check for SQLite-SEE files in source root
# If present, merge with amalgamation
hasSEE=false
if [ -e "${SQLITE_SEE_PREFIX}" ] && [ -e "${DERIVED_SOURCES_DIR}/sqlite3.c" ] && [ -e "${SQLITE_SEE_CODE}" ]
    then
    echo "Found sqlite-see"
    hasSEE=true
    cat "${SQLITE_SEE_PREFIX}" "${DERIVED_SOURCES_DIR}/sqlite3.c" "${SQLITE_SEE_CODE}" > "${DERIVED_SOURCES_DIR}/sqlite3-see.c"
fi

# copy generated amalgamation files (sqlite3.c and sqlite3.h)
# to BUILD_DIR/sqlite
# Overwrite with SQLite-SEE if present
SQLITE_OUTPUT_DIR="${BUILD_DIR}/sqlite"
mkdir -p "${SQLITE_OUTPUT_DIR}"
echo "Copying sqlite3.c to ${SQLITE_OUTPUT_DIR}/sqlite3.c"
cp ./sqlite3.c "${SQLITE_OUTPUT_DIR}/sqlite3.c"
echo "Copying sqlite3.h to ${PROJECT_DIR}/sqlite3.h"
cp ./sqlite3.h "${PROJECT_DIR}/sqlite3.h"
if [ "$hasSEE" = true ] ;
    then
    echo "Overwriting sqlite3.c to with sqlite3-see.c"
    cp ./sqlite3-see.c "${SQLITE_OUTPUT_DIR}/sqlite3.c"
fi

echo "Finished generating SQLite amalgamation files"

