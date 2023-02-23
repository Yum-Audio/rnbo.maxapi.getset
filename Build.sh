killall Max
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

if [ "$1" = "notarize" ]; then
  BUILD_CONF="Release"
  DONT_EXE_MAX=1
fi

if [ $currentos = Mac ]; then
	EXPORTER="Xcode"
  numCpuCores=$(sysctl -n hw.ncpu)
elif [ $currentos = Win ]; then
	EXPORTER="Visual Studio 17 2022"
  numCpuCores=$(powershell.exe -nologo -noprofile -command "& {Get-WmiObject -class Win32_processor | ft NumberOfLogicalProcessors}") 
  numCpuCores=${numCores//[!0-9]/}
elif [ $currentos = Linux ]; then
	EXPORTER="Unix Makefiles"
    # todo figure out actual number of cores on windows
  numCpuCores=8
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

if [ "$1" = "notarize" ]; then
  if [ $currentos = Mac ]; then
    "./Mac_SignAndZip.sh" || exit 11
  elif [ $currentos = Win ]; then
    "./Win_SignAndZip.sh" || exit 11
  fi
fi

if [ $DONT_EXE_MAX -eq 0 ]; then
    "/Applications/Max.app/Contents/MacOS/Max"
fi

