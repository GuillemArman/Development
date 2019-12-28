# Santa
We are two students from UPC CITM Barcelona developing a 2D platformer game in C++, using SDL and XML libraries, aswell as Tiled for the maps.

## Installation
Decompress the .zip file in any directory and execute the executable inside the decompressed folder.


## Controls:

#### A: Move left
#### D: Move right
#### SPACEBAR: Jump(you can double jump)
#### W/S(ONLY WHILE GODMODE): You can move up && down in the air

## Debug Keys:

#### F1: Start from the very first level
#### F2: Start from the second level
#### F3: Start from the beginning of the current level
#### F5: Save state
#### F6: Load previous saved state
#### F8: Debug UI
#### F9: See colliders and pathfinding
#### F10: Godmode
#### F11: Enable/Disable FPS cap to 30
#### º: Console

## Innovation:
Assignment 3:

- Pause Mode.
- Sliders for Volume and Fx + option to Windowed or Fullscreen.
- Measurable: You can see the final score after completing the game.

Previous Assignments:

- Simple entity system for player, thought to hold more entity types.

- Logic for entities(animations, speed, jump force and collider size and position) is defined in tiled.

	Their position on the map is also defined in tiled.

- When an enemy does not found a path to the player, after 1.5 seconds it will start flying/walking around.

## Bugs:

At the end of level 1 you might need to jump to detect the collider to get to level 2. 

Sometimes animations don't loop corectly so with cap frames activated look weird. (Press F11 to have a better experience) 

## Authors:

- Guillem Arman(https://github.com/GuillemArman)


## Link to the Github Repository: 

https://github.com/GuillemArman/Development

## Credits:

#### SPRITES: www.gameart2d.com/freebies.html
#### MUSIC: https://freesound.org/people/Lemoncreme/sounds/
#### FX: https://opengameart.org/

## License:

MIT License

Copyright (c) 2019  Guillem Arman 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.