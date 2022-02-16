# Intent Recognition

### Contents
* Intro
* TL;DR
* Installation
* Run
* Test
* Data Structure
* Algorithm
* Performance results
* Future improvements

### Intro
Intent Recognition is a simple, generic, lightweight and scalable command line intent recognizer (only english for now).

It is mainly written in C++ 17+ For configuration CMake is employed. Software developed with clang and gcc compiler.

Only hand-written source code. Didn't used any third-party lib for depencencies. (except gtest).

Tool gets input sequencely.

### TL;DR
The tool searches patterns in given inputs to find exact intent. It is too simple, client should give inputs in runtime. If you need more generic intent result, the tool needs more patterns and input types to recognize (in inputs folder).

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
$ cd test/
$ ./intent_recognition_test
```
**Note:** You should be in project's folder and must be built the project.

The test coveraged private, public and whole recognition project by prepared given inputs.

**Some of Test results:**
```
What is the weather like today? => Prints (Intent: Get Weather)
What is the weather like in Paris today? => Prints (Intent: Get Weather City)
Tell me about fact => Prints (Intent: Get Fact)
Can you open the door? => Prints (Intent: Set Door)
May you change the AC to 22 degree ? => Prints (Intent: Unrecognized intent!)
Could you open window? => Prints (Intent: Unrecognized intent!)
Tell me about weather in Paris today?=> Prints (Intent: Get Weather City)
What is the fact? => Prints (Intent: Get Fact)
```
### Project Structure

Data structure is little bit complicated. But everything is about for good performance and low memory usage. It has simple solution. The project is using a terminology that has 4 element.
- Question = What asks user?
- Operation = What will do for?
- Subject = for what?
- Object = what's what? ( simply Subject.object )

For memory consumption, I designed Iterator type of mapping algorithm.
The structure is using iterator mapping for connecting two type of input. Because when we do that we dont need any more not prevented operation search. In this way it will spend less cpu load.

![](/docs/structure.png "Data structure" )

For now there is only two operation type is informing to intent to user (get and set). It stored in `OperationList` user can change in `inputs/operations.txt`

`Questions` connected via `OperationsIter`, `OperationsIter` connected `SubjectList` via `SubjectIter`, `SubjectIter` connected `Objects` via `ObjectTypeIter`
#### File Formats
- Questions example pattern is :
```
    <Question>=<Operation>
    What is the=get
    Can you do=set
```      
- Subjects example usage is :
```
    <Subject>=<Operation1>,<Operation2>,...=<Object1>,<Object2>,...
    fact=get
    temperature=get,set=city,car
    door=set,get=car
```

### Algorithm
Firstly, main initialize the system for data structure. Data structure reads operations and questions files and connecting to first map. This map type is unordered_map. Because it has small number of element and it gives us a huge step for searching pattern in real time side. 

Then `questions` map's operations connected to subject via subjects.txt. it stores every subject element in `subjectlist` and sharing pointer (with iterator) to `subjectLinkMap`. `subjectLinkMap` multimap because it can prevent multiple operations for subjects. 

Then `objectTypeListLinkMap` gives with type of object we are interested in and which subject will search for which object type.

Then simple `objectsLinkMap` map gives the element search for subject like weather.paris.

So when initialization finished well. Everything prepared for usage.

Then user gives input for recognition. The system resolve the knots to take exact intent.

### Pattern Search Algorithm
While selection of pattern search algorithm. Focused multi and single pattern matcher algoritms.
One of the focused algorithms listed below.

- Naive
- Knuth–Morris–Pratt
- Boyer–Moore
- Rabin–Karp
- Aho-Corasick
- Two-way string-matching (libc++ strstr)

Rabin-Karp supports multiple pattern search. Rabin-Karp's performance was too close to Two-way string-matching. But i choosed `Two-way string-matching` algorithm, because strstr has too lightweight comparision.

### Performance results

Whole project benchmark tested under 2,7 GHz Dual-Core Intel Core i5, 8GB RAM Macbook Pro 13.
The benchmark results below by some iteration.

|Iteration|Cpu Time|
|-|------|
|1|14.1ms|
|2|14.3ms|
|3|14.1ms|
|4|13.9ms|
|4|14.7ms|



Performance test result under below senario:
- 220 input sentences
- 120 pattern
- search intance every for single pattern ( except std::regex )

![](/docs/perf_out.png "Performance output")
- [Quick benchmark result](https://quick-bench.com/q/J21M9tvvgdb7VfIQ2Pb78cIQ_0w)

whole recognition
### Future improvements
- I change the data structure to xml or json format with their third party tools.
- May be used great model with AI to recognition.
- Change tool syncronized to async.