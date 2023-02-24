#!/bin/sh

reset

unameOut="$(uname -s)"
case "${unameOut}" in

    Linux*)     machine=Linux;;
    Darwin*)    machine=Mac;;
    CYGWIN*)    machine=Cygwin;;
    MINGW*)     machine=Win;;
    *)          machine="UNKNOWN:${unameOut}"
esac

currentos=${machine}
echo "Current OS: $currentos"


BUILD_CONF="Debug"
DONT_EXE_MAX=0

if [ "$1" = "dontExecuteMaxByScript" ]; then
  DONT_EXE_MAX=1
fi

if [ "$1" = "clean" ]; then
  if [  "$2" = "dontExecuteMaxByScript" ]; then
    DONT_EXE_MAX=1
  fi
fi

if [ $DONT_EXE_MAX -eq 0 ]; then
  killall Max
fi

if [ $currentos = Mac ]; then
	EXPORTER="Xcode"
  numCpuCores=$(sysctl -n hw.ncpu)
elif [ $currentos = Win ]; then
	EXPORTER="Visual Studio 17 2022"
  numCpuCores=$(powershell.exe -nologo -noprofile -command "& {Get-WmiObject -class Win32_processor | ft NumberOfLogicalProcessors}") 
  numCpuCores=${numCores//[!0-9]/}
else
	echo "error: unsupported OS"
	exit 1
fi

cd $(dirname "$0")

if [ ! -d "build" ]; then
  mkdir build
fi

cd build/

if [ "$1" = "clean" ]; then
  rm -rf *
fi

cmake -G "$EXPORTER" ..

if [ $currentos = Mac ]; then
  cmake --build . --config "$BUILD_CONF" -j${numCpuCores} || exit 10
elif [ $currentos = Win ]; then
  cmake --build . --target ALL_BUILD --config "$BUILD_CONF" -j${numCpuCores} || exit 10
fi

cd .. 

if [ $DONT_EXE_MAX -eq 0 ]; then
  if [ $currentos = Mac ]; then
      "/Applications/Max.app/Contents/MacOS/Max"
  elif [ $currentos = Win ]; then
      "C:/Program Files/Cycling '74/Max 8/Max.exe"
  fi
fi

