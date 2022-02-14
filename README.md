# Intent Recognition

### Contents
* Brief
* TL; DR
* About
* Installation
* Run
* Test
* Algorithm
* Performance results

### Brief
Intent Recognition is a simple generic, lightweight and scalable command line intent recognizer (only english).
It is mainly written in C++ 17+ For configuration CMake is employed.
Software developed with clang and gcc compiler. 

### TL;DR

### About

- Only hand-written source code. Didn't used any third-party lib for depencencies.

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
**Note:** You should be in project folder and must be built the project.
### Test
```
$ ./test/intent_recognition_test
```
**Note:** You should be in project folder and must be built the project.

**Some of Test results:**
```
What is the weather like today? => Prints (Intent: Get Weather)
What is the weather like in Paris today? => Prints (Intent: Get Weather City)
Tell me about fact => Prints (Intent: Get Fact)
Can you open the door? => Prints (Intent: Set Door)
May you change the AC to 22 degree ? => Prints (Intent: Unrecognized intent!)
Could you open window? => Prints (Intent: Unrecognized intent!)
```
### Algorithm
While selection of pattern search algorithm. Focused multi and single pattern matcher algoritms.
One of the focused algorithms listed below.

- Naive
- Knuth–Morris–Pratt
- Boyer–Moore
- Rabin–Karp
- Aho-Corasick
- Two-way string-matching (libc++ strstr)

### Performance results
Performance test result under below senario:
- 220 input sentences
- 120 pattern
- search intance every for single pattern ( except std::regex )

![Performance output](/docs/perf_out.png "Performance output")

