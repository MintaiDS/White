[![Build status](https://ci.appveyor.com/api/projects/status/e1hd8cvcxixrfg6o?svg=true)](https://ci.appveyor.com/project/glisquery/white)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/5f6661ca68a4473fa4e2440c5c272532)](https://www.codacy.com/manual/makarevich.t/White?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=glisquery/White&amp;utm_campaign=Badge_Grade)
[![BCH compliance](https://bettercodehub.com/edge/badge/glisquery/White?branch=master)](https://bettercodehub.com/)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg?sanitize=true)](https://raw.githubusercontent.com/glisquery/White/master/LICENSE)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://GitHub.com/glisquery/White/graphs/commit-activity)
[![GitHub forks](https://img.shields.io/github/forks/glisquery/White.svg?style=social&label=Fork&maxAge=2592000)](https://GitHub.com/glisquery/White/network/)
<!--- [comment]: [![GitHub pull-requests closed](https://img.shields.io/github/issues-pr-closed/glisquery/White.svg)](https://GitHub.com/glisquery/White/pulls/) --->
# Wargaming Forge 2019 Project
## Game Client
### White Team
- [Dmitry Lisay](https://github.com/403)
- [Dmitry Sipeyko](https://github.com/MintaiDS)
- [Taras Makarevich](https://github.com/glisquery)
### Build
Current build is in archive White.zip.
Unpack it into some directory.
To run the application use:
```batch
Engine.exe <username> [game_name [num_players [num_turns]]]
```
For example:  
```batch Engine.exe White TestGame 5 2000``` - to create a game  
or ```batch Engine.exe White TestGame``` - to connect to an existing game

### Short description
An educational project of the simple game client. 
A game is built with a simple game engine
including graphics, networking and input system.
### Building instructions 
#### On Windows
##### Build project in one step
- Build using CMake.
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
- Pass username as a command line argument to the run script.
```batch
 run <username> [game_name [num_players [num_turns]]]
```
### Keyboard controls
- WASD - moving in four directions.
- Space - move up.
- Control - move down.
- Hold left button and mouse move - rotate camera.
### Development status
Trains displaying and movement completed. Multiple resource consumption, multiple trains
strategy implemented. Server messages processing is separated from rendering thread.
![progress](Docs/progress.jpg) 
