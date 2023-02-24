# rnbo-maxapi-getset

This is a small example project, showcasing how to use the Max API to get and set parameters on rnbo~ objects. 

Additionally this project demonstrates a possible way of how to setup a project for a Min API (https://github.com/Cycling74/min-api) based MaxMSP external. 


## Requirements
All dependencies are added as submodules, so this project should build out of the box. 


## Clone
`$ git clone https://github.com/Yum-Audio/rnbo.maxapi.getset --recurse-submodules`

Make sure to either clone using `--recurse-submodules` or alternatively call `$ git submodule update --init --recursive` after cloning, so all submodules are correctly initialized


## MaxMSP Project setup
Add `<your-local-repo-path>/external` to your MaxMSP search paths, so MaxMSP can find the built external. 


## Build
First, have a look at [Source/rnbo.maxapi.getset.h](Source/rnbo.maxapi.getset.h). 
On the very top you will find two definitions, where you can select either to test the Setters OR the Getters. 

**The Setters** will change the value on both rnbo~ test parameters every two seconds, which can be monitored via the connected `[attrui]` objects.
**The Getters** will print the current value - as set through `[attrui]` - to the Max Console


### Build the project
Use the included `Build.sh` script. (On Windows you might have to install Git Bash or a similar environment).

The script will automatically kill any open instance of Max once it starts building the external and will open Max after the successful build. 
If you don't want that, invoke the script like so: `$ ./Build.sh dontExecuteMaxByScript`
To clean build you can do: `$ ./Build.sh dontExecuteMaxByScript` or `$ ./Build.sh clean dontExecuteMaxByScript` 


### VS Code support
Additionally to build by script you have the option to build and debug using VS Code. 

- Install the `Task Runner` extension, if you don't have it already. 
- Open the entire repository folder with VS Code. 
    - Task Runner will now have a few Workspace Build processes
    - You can build the external and launch MaxMSP with attached debugger (F5 or Play button, after you selected your appropriate OS platform in the Debugger Tab)