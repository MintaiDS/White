[![Codacy Badge](https://api.codacy.com/project/badge/Grade/5f6661ca68a4473fa4e2440c5c272532)](https://www.codacy.com/manual/makarevich.t/White?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=glisquery/White&amp;utm_campaign=Badge_Grade)
[![CodeFactor](https://www.codefactor.io/repository/github/glisquery/white/badge)](https://www.codefactor.io/repository/github/glisquery/white)
[![Coverage Status](https://coveralls.io/repos/github/glisquery/White/badge.svg)](https://coveralls.io/github/glisquery/White)
[![BCH compliance](https://bettercodehub.com/edge/badge/glisquery/White?branch=master)](https://bettercodehub.com/)
[![Build Status](https://travis-ci.com/glisquery/White.svg?branch=master)](https://travis-ci.com/glisquery/White)
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
- build - Build using CMake tool and download test data required for 
the first task.
##### Build manually
- cmake -E make_directory Build Build/Bin Build/Obj
- cd Build
- cmake ../Source
- cd ..
- cmake --build Build
### Running application
- run - Pass a path to the json file as a command line argument.
- test - Display all graphs from test data set.
