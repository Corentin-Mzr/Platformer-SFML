# MegaMario SFML C++

## Description

Implementation of a platformer game using C++, SFML and ImGui  
Based on [**COMP4300 - Game Programming - Lecture 11 - Assignment 3**](https://www.youtube.com/watch?v=k2ckoLsD7ZU) by Dave Churchill  
Uses an implementation of an Entity-Component-System (ECS) architecture  

## Gameplay

WIP

## Table of contents

- [MegaMario SFML C++](#megamario-sfml-c)
- [Description](#description)
- [Gameplay](#gameplay)
- [Table of Contents](#table-of-contents)
- [Installation](#installation)
  - [Prerequisites](#prerequisites)
  - [Clone the repository](#clone-the-repository)
  - [Build the project](#build-the-project)
  - [Run the program](#run-the-program)
- [Libraries](#libraries)
- [Controls](#controls)
- [Specifications](#specifications)
- [Documentation](#documentation)
- [Credits](#credits)
  - [Musics](#musics)
  - [Sound effects](#sound-effects)
  - [Textures](#textures)
- [License](#license)

## Installation

### Prerequisites

- C++/C compiler (gcc, g++, cl, clang)
- CMake 3.28 or newer

For Linux/Ubuntu users, you also need to install the following dependencies:  

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake
sudo apt-get install -y libx11-dev \
                        libxcursor-dev \
                        libxi-dev \
                        libxrandr-dev \
                        libudev-dev \
                        libgl1-mesa-dev \
                        libfreetype6-dev \
                        libjpeg-dev \
                        libopenal-dev \
                        libflac-dev \
                        libvorbis-dev \
                        libxcb1-dev \
                        libxcb-image0-dev \
                        libxcb-randr0-dev \
                        libxcb-xtest0-dev
```

### Clone the repository

```bash
git clone https://github.com/Corentin-Mzr/Platformer-SFML
```

### Build the project

From the root folder, execute the following commands

```bash
cmake -B build
cmake --build build
```

### Run the program

To run the program, launch it from the build/bin folder

```bash
cd build/bin
./Megamario-SFML.exe
```

## Libraries

The following libraries have been used for this program

- [**SFML 3.0**](https://github.com/SFML/SFML)
- [**toml11**](https://github.com/ToruNiina/toml11)
- [**ImGui**](https://github.com/ocornut/imgui)

## Controls

WASD / Arrows: Move and Jump
Spacebar: Shoot

## Specifications

Specifications are defined in the [**specification file**](SPECIFICATIONS.md).

## Documentation

The Doxygen Generated Documentation can be found in [**docs**](docs).

Access to the [**HTML version**](docs/html/index.html) of the documentation.

## Credits

### Musics

- [**Menu: Koronba - Jellyfish (instrumental)**](https://www.youtube.com/watch?v=rL88w2uHlqs)

### Sound effects

- [**Pixabay**](https://pixabay.com/sound-effects)
- [**jsfxr**](https://sfxr.me/)

### Textures

- [**Super Mario Bros NES Assets - The Spriters Resources**](https://www.spriters-resource.com/fullview/52571/)

## License

This program is under the [**MIT License**](LICENSE.md)
