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
- [Specifications](#specifications-may-vary-from-real-implementation)
  - [Entities](#entities)
  - [Assets](#assets)
  - [Player](#player)
  - [Animations](#animations)
  - [Decoration Entities](#decoration-entities)
  - [Tiles](#tiles)
    - [Brick Tiles](#brick-tiles)  
    - [Question Tiles](#question-tiles)
  - [GUI](#gui)
  - [Rendering](#rendering)
  - [Bonus](#bonus)
  - [Misc](#misc)
  - [Level Creation](#level-creation)
  - [Config File Specification](#config-file-specification)
    - [Window Section Specification](#window-section-specification)
    - [Bullet Section Specification](#bullet-section-specification)
    - [Level Section Specification](#level-section-specification)
    - [Font Section Specification](#font-section-specification)
    - [Texture Section Specification](#texture-section-specification)
    - [Sound Section Specification](#sound-section-specification)
    - [Music Section Specification](#music-section-specification)
    - [Animation Section Specification](#animation-section-specification)
  - [Level File Specification](#level-file-specification)
    - [Tile Entity Specification](#tile-entity-specification)
    - [Decoration Entity Specification](#decoration-entity-specification)
    - [Player Specification](#player-specification)
  - [Assignment Hints](#assignment-hints)
- [Credits](#credits)
  - [Musics](#musics)
  - [Sound effects](#sound-effects)
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

## Specifications (MAY VARY FROM REAL IMPLEMENTATION)

### Entities

- [x] All entity positions denote the center of their rectangular sprite. It also denotes the center of the bounding box, if it has one. This is set via sprite.setOrigin() in the Animation class constructor

### Assets

- [x] Entities in the game will be rendered using various Textures and Animations which we will be calling Assets (along with Fonts)
- [x] Assets are loaded once at the beginning of the program and stored in the Assets class, which is stored by the GameEngine class
- [x] ~~All Assets are defined in assets.txt, with the syntax defined below~~ **All assets are defined in config.toml, with the keys defined below**

### Player

- [x] The player Entity in the game is represented by Megaman, which has several different Animations: Stand, Run, and Air. You must determine which state the player is currently in and assign the correct Animation.
- [x] The player moves with the following controls:
  Left: A key, Right: D key, Jump: W key, Shoot: Space key
- [x] The player can move left, move right, or shoot at any time during the game. This means the player can move left/right while in the air.
- [x] ~~The player can only jump if it is currently standing on a tile~~ **The player can still jump after falling from a tile**
- [x] If the jump key is held, the player should not continuously jump, but instead it should only jump once per button press. If the player lets go of the jump key mid-jump, it should start falling back down immediately
- [x] If the player moves left/right, the player's sprite will face in that direction until the other direction has been pressed
- [x] Bullets shot by the player travel in the direction the player is facing.
- [x] The player collides with 'Tile' entities in the level (see level syntax) and cannot move through them, The player land on a Tile entity and stand in place if it falls on it from above.
- [x] The player does not collide with 'Dec' (decoration) entities in the level
- [x] If the player falls below the bottom of the screen, they respawn at the start of a level
- [x] The player should have a Gravity component which constantly accelerates it downward on the screen until it collides with a tile
- [x] The player has a maximum speed specified in the Level file (see below) which it should not exceed in either x or y direction.
- [x] The player will be given a CBoundingBox of a size specified in the level file.
- [x] The player's sprite and bounding box are centered on the player's position

### Animations

- [x] See below for Animation asset specification
- [x] Animations are implemented by storing multiple frames inside a texture
- [x] The Animation class handles frame advancement based on animation speed
- [x] You need to implement Animation::update() to properly progress animations
- [x] You need to implement Animation:hasEnded() which returns true if an animation has finished its last frame, false otherwise.
- [x] Animations can be repeating (loop forever) or non-repeating (play once)
- [x] Any entity with a non-repeating animation should be destroyed once its Animation's hasEnded() returns true (has finished one cycle)

### Decoration Entities

- [x] Decoration entities ('Dec' in a level file) are simply drawn to the screen, and do not interact with any other entities in the game in any way
- [x] Decorations can be given any Animation in the game, but intuitively they should be reserved for things like clouds, bushes, etc.

### Tiles

- [x] Tiles are Entities that define the level geometry and interact with players
- [x] Tiles can be given any Animation that is defined in the assets file
- [x] Tiles will be given a CBoundingBox equal to the size of the animation, which can be retrieved with:

    ```cpp
    tile->get<CAnimation>().animation.getSize()
    ```

- [x] The current animation displayed for a tile can be retrieved with:

    ```cpp
    tile->get<CAnimation>().animation.getName()
    ```

- [x] Tiles have different behavior depending on which Animation they are given

#### Brick Tiles

- [x] Brick tiles are given the 'Brick' Animation when created
- [x] When a brick tile collides with a bullet, or is hit by a player from below:
  - [x] Its animation should change to 'Explosion' (non-repeat) **when a bullet hits the brick, or change to 'Debris' (non-repeat) when the player hit the brick from below**
  - [x] Non-repeating animation entities are destroyed when hasEnded() is true
  - [x] Its CBoundingBox component should be removed
  - [x] **A 'Debris' or 'Explosion' sound effect is played on destruction**

#### Question Tiles

- [x] Question tiles are given the 'Question' Animation when created
- [x] When a Question tile is hit by a player from below, 2 things happen:
  - [x] A temporary lifespan entity with the 'Coin' animation should appear for 30 frames, 64 pixels above the location of the Question entity
  - [x] **The player cannot interact interact anymore with the Question tile (i.e. re-trigger the coin animation) by changing its Animation component**
  - [x] **A 'Coin' sound effect is played on collision**

#### **Spike Tiles**

- [x] **Spike tiles are given the 'Spike' Animation when created**
- [x] **When the player collides with a spike, he is teleported back to the beginning of the level**
- [x] **Spike tiles uses a BoundingConvex component, representing a convex polygon**
- [x] **Collision detection and resolution with a spike is done via the Separated Axis Theorem (SAT)**
- [x] **A 'Hurt' sound effect is played on collision**

### **Flagpole Tiles**

- [x] **Flagpole tiles are given the 'Flagpole' Animation when created**
- [x] **The flagpole has a particular hitbox, only the block on the bottom can be collided with**
- [x] **When the player hits the flagpole's block from the top, he is teleported back to the beginning of the level**
- [x] **A text should appear on screen indicating the player finished the level**
- [x] **A 'Win' sound effect is played when the player finishes the level**

### GUI

- [x] The GUI must display options to turn off each system independently
- [x] For each entity in the game, the GUI must list the ID, tag and position of that entity
- [x] You must display a list of all entities, as well as lists of entities by their tag
- [x] You must have some way of destroying a given entity by interacting with the UI element associated with it

### Rendering

- [x] Entity rendering has been implemented for you, no need to change that system
- [x] Entities are rendered in the order that they are stored in the EntityManager, with later entities being drawn on top of previous ones.

### Bonus

- [x] ~~Any special effects which do not alter game play can be added for up to 5% bonus marks on the assignment.~~ **Added musics and sound effects**
- [x] ~~You may develop a 'special weapon' that has special effects which can also contribute to another 5% bonus marks available on the assignment.~~ **Added the Spike tile, and an associated hitbox component CBoundingConvex**
- [x] **Added audio volume control**
- [x] **Added the Flagpole tile**

### Misc

- [x] The 'P' key should pause the game
- [x] Pressing the 'T' key toggles drawing textures
- [x] Pressing the 'C' key toggles drawing bounding boxes of entities
- [x] Pressing the 'G' key toggles drawing of the grid. This should be very helpful for debugging
- [x] The 'ESC' key should go 'back' to the Main Menu, or quit if on the Main Menu

### Level Creation

- [ ] For this assignment you are also required to create your own level. This level should include some interesting gameplay

### **Config File Specification**

- [x] **The config file will be a TOML file divided in multiple sections: window, bullet, level, font, texture, sound and animation. These sections are described below**

#### **Window Section Specification**

- [x] **The window section will contain the following:**
  - [x] **Width: Window's width, stored as unsigned**
  - [x] **Height: Window's height, stored as unsigned**
  - [x] **Title: Window's title, stored as string**
  - [x] **Framerate: Maximum framerate, stored as unsigned**
  - [x] **Color: Window's background color, stored as array<int, 4>**

#### **Bullet Section Specification**

- [x] **The bullet section will contain the following:**
  - [x] **Speed: Bullet's speed, stored as float**
  - [x] **Radius: Bullet's radius, stored as float**
  - [x] **Lifespawn: Bullet's lifespan, stored as unsigned**

#### **Level Section Specification**

- [x] **The level section will contain multiple subsections of levels. Each subsection will contain the following:**
  - [x] **Name: Level's name, stored as string**
  - [x] **Path: Level's filepath, stored as string**

#### **Font Section Specification**

- [x] **The font section will contain multiple subsections of fonts. Each subsection will contain the following:**
  - [x] **Name: Font's name, stored as string**
  - [x] **Path: Font's filepath, stored as string**

#### **Texture Section Specification**

- [x] **The texture section will contain multiple subsections of textures. Each subsection will contain the following:**
  - [x] **Name: Texture's name, stored as string**
  - [x] **Path: Tetxure's filepath, stored as string**

#### **Sound Section Specification**

- [x] **The sound section will contain multiple subsections of sounds. Each subsection will contain the following:**
  - [x] **Name: Sound's name, stored as string**
  - [x] **Path: Sound's filepath, stored as string**

#### **Music Section Specification**

- [x] **The music section will contain multiple subsections of musics. Each subsection will contain the following:**
  - [x] **Name: Music's name, stored as string**
  - [x] **Path: Music's filepath, stored as string**

#### **Animation Section Specification**

- [x] **The animation section will contain multiple subsections of animation. Each subsection will contain the following:**
  - [x] **Name: Animation's name, stored as string**
  - [x] **Texture: Texture used for the animation, stored as string**
  - [x] **Frames: Number of frames in the animation, stored as int**
  - [x] **Speed: Number of game frames between each animation frames, stored as int**

### Level File Specification

Game levels will be specified by a Level file, which will contain a list of entity specifications, one per line. It will also contain a single line which specifies properties of the player in that level. In this way, you can define an entire level in the data file, rather than in programming code. The syntax of lines of the Level file are as follows.

IMPORTANT NOTE:
All (GX, GY) positions given in the level specification file are given in 'grid' coordinates. The 'grid' cells are of size 64 by 64 pixels, and the entity should be positioned such that the bottom left corner of its texture is aligned with the bottom left corner of the given grid coordinate. The grid starts an (0,0) in the bottom-left of the screen, and can be seen by pressing the 'G' key while the game is running.

#### Tile Entity Specification

- Tile N GX GY  
  - Animation Name    N       std::string (Animation asset name for this tile)  
  - GX Grid X Pos     GX      float  
  - GY Grid Y Pos     GY      float  

#### Decoration Entity Specification

- Dec N X Y  
  - Animation Name     N      std::string (Animation asset name for this dec)  
  - X Grid X Pos       X      float  
  - Y Grid Y Pos       Y      float  

#### **Spike Entity Specification**

- **Spike N X Y**
  - **Animation Name    N     std::string (Animation asset name for this spike)**
  - **X Grid X Pos      X     float**
  - **Y Grid Y Pos      Y     float**

#### Player Specification

- Player GX GY CW CH SX SY SM GY B  
  - GX, GY Grid Pos       X, Y    float, float (starting position of player)  
  - BoundingBox W/H       CW, CH  float, float (in pixels)  
  - Left/Right Speed      SX      float  
  - Jump Speed            SY      float  
  - Max Speed             SM      float  
  - Gravity               GY      float  
  - Bullet Animation      B       std::string (Animation asset to use for bullets)

### Assignment Hints

I recommend approaching this assignment in the following order, which will help you debug your program along the way with minimal errors. Remember to do one step at a time and test whether what you have just implemented is working properly before moving on to any additional steps.

- Rendering system has already been set up for you, to help you debug:
  You can press the T key to toggle drawing textures
  You can press the C key to toggle drawing bounding boxes
  You can press the G key to toggle drawing the grid

- You can implement Animation::update() and Animation::hasEnded() at any time, it will not affect the gameplay mechanics whatsoever, just animation

- Implement Scene_Play::loadLevel()
  Since rendering is already completed, once you correctly read in the different types of entities, add them to the EntityManager and they should automatically be drawn to the screen. Add the correct bounding boxes to Tile entities, and no bounding boxes to the Dec entities. Remember you can toggle debug viewing of bounding boxes with the T and C keys
  As part of this step, implement the Scene_Play::gridToMidPixel() function, which takes in as parameters a grid x,y position and an Entity, and returns the Vec2 position of the CENTER of that Entity. You must use the Animation size of the Entity to determine where its center point should be. Keep in mind that this means your Entity MUST have its CAnimation component added first, so that it can be used to calculate the midpoint in this function.

- Implement Scene_Play::spawnPlayer()
  Read the player configuration from the level file and spawn the player. This is where the player should restart when they die.

- Implement some basic WASD u/l/d/r movement for the player entity so that you can use this to help test collisions in the future. Remember that you must use registerAction to register a new action for the scene. See the actions already registered for you, and the sDoAction() function for syntax on how to perform actions.

- Implement Scene_Play::spawnBullet()
  Bullet should shoot when Space is pressed in same direction player facing. Holding down the space button should not continuously fire bullets. A new bullet can only be fired after the space key has been released. Use the entity's CInput.canShoot variable to implement this

- Implement Physics::GetOverlap()
  This function should return the overlap dimensions between the bounding boxes of two entities. This is the same as the purple rectangle in notes. GetPreviousOverlap should be a copy/paste of this solution except using the previous positions instead of the current positions of th entity. If either input entity has no bounding box, then return Vec2(0,0).

- Implement collision checking with bullets / brick tiles such that the brick is destroyed when a bullet collides with it. Remember, a collision occurs when the overlap is non-zero in both the X and Y component. Bullets should always be destroyed when they collide with any non-decorative tile.

- Implement collision resolution such that when the player collides with a non-decorative tile, the player cannot enter it over overlap it. When the player collides with a tile from below, its y-velocity should be set to zero so that it falls back downward and doesn't 'hover' below the tile.

- Implement a way of detecting which side the player collided with the tile

- Change the controls such that they are the proper left/right/jump style.
  Note: All movement logic should be in the movement system. The sDoAction system is ONLY used to set the proper CInput variables. If you modify the player's speed or position anywhere inside the sDoAction system, you will lose marks as this is potentially unsafe / produces unwanted effects.

- Implement gravity such that the player falls toward the bottom of the screen and lands on tiles when it collides with a tile from above. Note that when the player lands on a tile from above, you should set its vertical speed to zero so that gravity does not continue to accelerate the player downward.

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
