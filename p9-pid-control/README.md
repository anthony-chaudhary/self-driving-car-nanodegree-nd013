# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: Usage `./pid.exe kp ki kd speed_goal throttle_kp throttle_ki throttle_kd`

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets) == 0.13, but the master branch will probably work just fine
  * Follow the instructions in the [uWebSockets README](https://github.com/uWebSockets/uWebSockets/blob/master/README.md) to get setup for your platform. You can download the zip of the appropriate version from the [releases page](https://github.com/uWebSockets/uWebSockets/releases). Here's a link to the [v0.13 zip](https://github.com/uWebSockets/uWebSockets/archive/v0.13.0.zip).
  * If you run OSX and have homebrew installed you can just run the ./install-mac.sh script to install this
* Simulator. You can download these from the [project intro page](https://github.com/udacity/CarND-PID-Control-Project/releases) in the classroom.


## Windows Install Instructions 

1. Install, in your root `c:/` directory, vcpkg https://github.com/Microsoft/vcpkg   (15 - 30 minutes)
2. Be sure while installing vcpkg to carefully follow all instructions! This is NOT an easy install process.
3. Install python 2.7 (dependency for libuv) 
4. cd to directory with vcpkg .exe and `./vcpkg install uWebsockets` (20 min, mostly automatic)
5. Open CMakeSetting.json, check if `C:/vcpkg/scripts/buildsystems/vcpkg.cmake` is the correct directory to your vcpkg and `DCMAKE_TOOLCHAIN_FILE` matches the output from vcpkg integrate.
6. Open in VS17 community edition, build pid.exe in x86 debug.
