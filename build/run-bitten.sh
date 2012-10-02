#!/bin/sh

conffile="`mktemp`"
url="http://lol.zoy.org/builds"

append() {
  echo "$*" >> "$conffile"
}

#
# Check for command line
#

if [ "$#" != 2 ]; then
    echo "Usage: $0 <username> <password>"
    exit 1
fi

#
# Operating system information
#

append "[os]"
append "name = `uname -srvmo`"
append "version = 0"
family="`uname -s | tr A-Z a-z`"
case "$family" in
  mingw*) family="windows" ;;
  darwin*) family="osx" ;;
esac
append "family = $family"
append "token = $RANDOM"
append ""

#
# Hardware information
#

append "[machine]"
name="`uname -n | tr A-Z a-z`"
case "$name" in
  d*e*s*o*v*) name="putois" ;;
esac
append "name = $name"
processor="`uname -m`"
case "$processor" in
  x86_64) processor="amd64" ;;
  i*86) processor="i386" ;;
esac
# Windows defines a lot of crazy shit, 
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
# Xbox development kit
#

append "[xdk]"
if [ -n "$XEDK" ]; then
  # FIXME: we don't know how to check the version
  append "version = 2.0.20675.0"
fi
append ""

#
# PS3 development kit
#

append "[ps3sdk]"
if [ -n "$SN_PS3_PATH" ]; then
  append "version = 410"
fi
append ""

#
# Launch everything
#

while : ; do
    bitten-slave "$url" -f "$conffile" --name "$name"
    sleep 10
done

rm -f "$conffile"

