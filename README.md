[![GitHub license](https://img.batchields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/glisquery/white/master/LICENSE)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/5f6661ca68a4473fa4e2440c5c272532)](https://www.codacy.com/manual/makarevich.t/White?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=glisquery/White&amp;utm_campaign=Badge_Grade)
[![Coverage Status](https://coveralls.io/repos/github/glisquery/White/badge.svg)](https://coveralls.io/github/glisquery/White)
[![BCH compliance](https://bettercodehub.com/edge/badge/glisquery/White?branch=master)](https://bettercodehub.com/)
[![Build Status](https://travis-ci.com/glisquery/White.svg?branch=master)](https://travis-ci.com/glisquery/White)
![Code Inspector](https://www.code-inspector.com/project/1816/status/svg?sanitize=true)
# Wargaming Forge 2019 Project
## Game Client
### White Team
- Dmitry Sipeyko
- Dmitry Lisay
- Taras Makarevich
### Short description
An educational project of the simple game client. 
A game is built with a simple game engine
including graphics, networking and input system.
### Building instructions 
#### On Windows
##### Build project and grab test data in one step
- Build using CMake tool and download test data required for 
the first task, alose creates test script in root directory.
```batch
build 
```
##### Manual building
- Perform all building steps manually.
```batch
cmake -E make_directory Build Build/Bin Build/Obj
cd Build
cmake ../Source
cd ..
cmake --build Build
```
### Running application
 - Pass a path to the json file as a command line argument to run script.
```batch
 run
```
- Display all graphs from test data set.
```batch
test
```

