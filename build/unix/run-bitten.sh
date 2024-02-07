#!/bin/sh

tmpdir="$TMPDIR"
if [ -z "${tmpdir}" ]; then tmpdir="$TEMP"; fi
if [ -z "${tmpdir}" ]; then tmpdir="$TEMPDIR"; fi
if [ -z "${tmpdir}" ]; then tmpdir="$TMP"; fi
if [ -z "${tmpdir}" ]; then tmpdir="/tmp"; fi

conffile="`mktemp -q "${tmpdir}/lol-bitten-XXXXXXXX" 2>/dev/null`"
if [ "${conffile}" = "" ]; then
    conffile="`mktemp 2>/dev/null`"
fi
scriptfile=""
builddir="${tmpdir}/lol-bitten-`whoami`"
url="http://lolengine.net/builds"

#
# Utility functions
#

append() {
    echo "$*" >> "${conffile}"
}

cleanup() {
    rm -f "${conffile}" "${scriptfile}"
    rm -rf "${builddir}"
}

bailout() {
    cleanup
    # Exit gracefully
    exit 0
}

trap bailout EXIT HUP INT QUIT ABRT KILL ALRM TERM

#
# Fork if necessary
#

if [ "$1" = "--forked" ]; then
    shift
    scriptfile="$1"
    shift
else
    cp "$0" "${conffile}"
    chmod +x "${conffile}"
    exec "${conffile}" --forked "${conffile}" "$@"
    exit 0
fi

#
# Check for command line
#

if [ "$#" != 2 ]; then
    echo "Usage: run-bitten.sh <username> <password>"
    exit 1
fi

#
# Clean up working directory
#
cleanup
if [ -e "${builddir}" ]; then
    echo "Error: cannot get rid of ${builddir}"
    exit 1
fi

#
# Operating system information
#

append "[os]"
append "name = `uname -srmo 2>/dev/null || uname -srm`"
append "version = 0"
family="`uname -s | tr A-Z a-z`"
case "$family" in
  mingw*) family="windows" ;;
  darwin*) family="osx" ;;
esac
append "family = $family"
# This random token prevents HTTP conflicts when several instances
# are run from the same machine.
append "token = $$$RANDOM"
append ""

#
# Hardware information
#

append "[machine]"
name="`uname -n | tr A-Z a-z | sed 's/[.].*//'`"
case "$name" in
  d*e*s*o*v*) name="putois" ;;
esac
append "name = $name"
processor="`uname -m`"
case "$processor" in
  x86_64) processor="amd64"
    if test "`getconf LONG_BIT 2>/dev/null`" = 32; then
      processor="i386"
    fi ;;
  i*86) processor="i386" ;;
esac
# Windows defines a lot of crazy shit, try to make sense of it
case "$PROCESSOR_ARCHITECTURE" in
  amd64|AMD64) processor="amd64" ;;
  x86|X86) processor="i386" ;;
esac
case "$PROCESSOR_ARCHITEW6432" in
  amd64|AMD64) processor="amd64" ;;
  x86|X86) processor="i386" ;;
esac
append "processor = $processor"
append ""

#
# Authentication information
#

append "[authentication]"
append "username = $1"
append "password = $2"
append ""

#
# Visual Studio configuration
#

# FIXME: we also need to check for the Visual Studio SDK
append "[msvc]"
if [ -n "$VS100COMNTOOLS" ]; then
    append "version = 10"
elif [ -n "$VS110COMNTOOLS" ]; then
    append "version = 11"
elif [ -n "$VS90COMNTOOLS" ]; then
    append "version = 9"
fi
append ""

#
# iOS development kit
#

append "[ios]"
if [ -d "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS6.1.sdk" ]; then
    append "version = 6.1"
fi
append ""

#
# mingw32 / mingw-w64
#

append "[mingw64]"
if x86_64-w64-mingw32-g++ --version >/dev/null 2>&1; then
    append "version = $(x86_64-w64-mingw32-g++ --version | sed -ne 's/.*g++ *([^)]*) *//p')"
fi
append ""

append "[mingw32]"
if i686-w64-mingw32-g++ --version >/dev/null 2>&1; then
    append "version = $(i686-w64-mingw32-g++ --version | sed -ne 's/.*g++ *([^)]*) *//p')"
fi
append ""

#
# Emscripten
#

append "[emscripten]"
if em++ --version >/dev/null 2>&1; then
    append "version = $(em++ --version | sed -ne 's/.*emcc.* \([0-9.]\{1,\}\).*/\1/p')"
fi
append ""

#
# Android NDK
#

append "[ndk]"
if [ "$family" != "windows" ]; then
    if [ -f "$ANDROID_NDK_ROOT/RELEASE.TXT" ]; then
        append "version = $(cat "$ANDROID_NDK_ROOT/RELEASE.TXT")"
    fi
fi
append ""

#
# Raspberry Pi cross-compiler
#

append "[raspi]"
if [ "$family" != "windows" ]; then
    if [ -d "$RASPI_SDK_ROOT/tools" ]; then
        append "version = 0"
    fi
fi
append ""

#
# Show what we just did here
#

cat "${conffile}"

#
# Fix system
#

if [ "$family" = "osx" ]; then
    # The version of Subversion shipped by Apple is antique; try to
    # use the one in /usr/local/bin instead.
    PATH="/usr/local/bin:$PATH"
    export PATH
    # HACK: the version of llvm-g++ shipped by Apple has an insane
    # memory leak; use clang++ instead.
    CC="clang"
    export CC
    CXX="clang++"
    export CXX
fi

#
# Launch everything
#

while : ; do
    bitten-slave "$url" \
        -f "${conffile}" \
        --name "$name" \
        --work-dir="${builddir}"
    rm -rf "${builddir}"
    sleep 10
done

exit 0

