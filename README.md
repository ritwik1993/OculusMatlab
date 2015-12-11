OculusMatlab
==================

This is an application written that exploits an interface between Oculus Code written in C++ and a client script running in matlab. As of now, this is still under development so do not expect everything to run straight out of the box when cloned and installed. The project setup and a lot of Macros have been adapted from this amazing book [Oculus Rift in Action](http://oculusriftinaction.com), whose main example repo can be found at [OculusRiftInAction](https://github.com/OculusRiftInAction/OculusRiftInAction)

# Notes

This application is designed to use static linking throughout.  This means static linking against the C/C++ runtimes 
on windows (to avoid issues with missing Visual Studio redistributables) and static linking against all of the 
included submodules.  Dynamic linking is only used for system libraries (other than the C++ runtime on Windows) and 
for the resource DLL on windows.  In order to avoid having each executable contain a distinct copy of the (currently 
30MB) resource files, a single DLL is created to hold them, and the common library functionality has utility methods 
to load them.

# Instructions for building (all platforms)

## Checking out 

	git clone https://github.com/ritwik1993/OculusMatlab.git --recursive

This command is likely to take a while. It's a big project with lots of submodules

Install Boost and other essentials on your system (if it isn't already installed)

 - For Linux

	      	 sudo apt-get install build-essential cmake libboost-all-dev libudev-dev

		 
NOTE! The following steps MIGHT not work if you do not have boost installed in your system. I will soon add instructions on what to do about that on a windows system

## Creating project files

	cd OculusMatlab
	mkdir build
	cd build
	cmake .. [-G <your preferred toolset>]

CMake will create project files for a given toolset.  What toolset you use depends on your platform, and there's usually a 
default.  On a Windows machine, it defaults to Visual Studio, although if you have more than one version installed it may 
require you to specify which one you want.  You can specify a specific platform using -G.  

    cmake -h

Will show you a list of supported generators on your platform.

## Building

Depends on your build tool.  If you're using Visual Studio or XCode you can just say 'File, Open Project' or something to that effect and point it at the OculusRiftInAction/build directory.  

# Supported platforms

## On Windows 8 with Visual Studio 2013 Desktop Edition

The following generators have all been found to work

Suitable generators

* "Visual Studio 12" 
* "Visual Studio 12 Win64" 

Note that "Visual Studio 12" actually corresponds to Visual Studio 2013.  

It's worthwhile to note that CMake may not show the Win64 options in the list of generators, even if they're supported.
Win64 refers to the creation of projects that produce 64 bit binaries, NOT the version of Visual Studio itself. 

The libraries needed to build the core examples are included and built as part of the project, but some examples will 
only be built if additional libraries are present, such as OpenCV.  If you want to build these, it's important to make 
sure you match up the 32/64 bit nature of the project you create with the installed version of OpenCV, etc.  

## On OSX with XCode

* "Xcode"

## On Ubuntu using Eclipse (Kepler) 

Suitable generators

* "Eclipse CDT4 - Unix Makefiles"
* "Eclipse CDT4 - Ninja"
* "Unix Makefiles"
* "Ninja"

Note that the CMake Eclipse generators will complain about a build directory that is a child of the source directory, 
but I have not experienced any issues with this. 

I highly recommend using the Ninja builder instead of makefiles.  It's dramatically faster.  Note that the Ubuntu package for the ninja builder is `ninja-build` *not* `ninja`, which is an unrelated tool.  

# Optional dependencies

The examples should include all the *required* dependencies for making the basic examples.  However, if you want to build the examples from Chapter 9, you will require Qt 5.4 or higher.  For the Chapter 13 examples, you will required OpenCV 2.x.  If you wish to use these you can set the Qt5_DIR and OpenCV_DIR values when running CMake in order to allow it to find them, although it will make an effort to find them independently.  

However, if you are using either of these optional libraries, keep in mind CMake will not attempt to copy the binaries from the library.  

# Running the code

Once the binaries are built, they can be located in the build/output folder. The two main binaries used for the electrolocation demo are *MainApp* and *SimulatedMainApp*. The other binaries are just for development tests. By default, all communication occurs on port 1700 - but this can be changed very easily.

##MainApp
This application creates a listener to a tcp client and works only when a tcp connection is found and accepted. If it doesn't run with a "Failed to initialise Oculus SDK", rerun the application after launching the headset management interface (*ovrd* in Linux). Once communication begins - it sends the headset's translational X, translational Y and angular Yaw data (or any other message you want) across the connection. It also listens for data coming in from the tcp client. For a simple working demo use the matlab script *quickDemo* located in /mscripts. This script recieves data from the tcp server (the c++ application in this case) and relays the first message (yaw) back to the application. This can easily be upgraded to work with the NxR 4DOF sensorpod similar to the matlab scripts  *demoGantry*, or *demoGantryTraj*

##SimulatedMainApp
This works in a simlar way as MainApp except that only one message is currently sent from the c++ application to the tcp client. Using the matlab script *SimulatedDataDemo*, we can visualise previously collected data.