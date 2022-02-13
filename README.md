# Intent Recognition

## Contents
* Brief
* TL; DR
* About
* Installation
* Run
* Test
* Performance results

### Brief
Intent Recognition is a simple generic, lightweight and scalable command line recognizer. 
It is mainly written in C++ 17+ For configuration CMake is employed.
Software developed with clang and gcc compiler. 

### TL;DR

### About

### Prerequisites
- [CMake](https://cmake.org/) (version 3.5 or newer) is the recommended build system for building the intent recognition.
- Build system tested on macOS and Linux. Not yet tested on Windows.
- Software developed under gcc-10.2 and Clang-13.0 versions.

### Build
Building with CMake (Linux, macOS)
```
$ git clone -b master https://github.com/gtarim/intent_recognition
$ cd intent_recognition
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --config Release
```
### Run
```
$ ./intent_recognition
```
### Test

### Performance results