# Game for CSC-231

This starting code consists of a game engine that was written by WLC
Computer Science Faculty. You will use it to create a turn-based,
dungeon-crawling game, similar to a
[Roguelike](https://en.wikipedia.org/wiki/Roguelike) game.

The engine makes it easy to work with entities, items, graphics,
audio, and creates a random dungeon for you. The engine itself does
not contain any game content, besides the dungeon, but instead
provides many interfaces that you can extend in order to fill your
game with content.

---

# Folder structure

Since the project is large, we use a hierarchical CMake build, and
cleanly separate `engine` code from game `content`. Additionally,
there is an `assets` folder which contains images and audio files,
and a `settings.txt` file for general settings for the game, including
screen resolutions and zoom levels, locations of asset files (images
of characters and others), and some properties for the dungeon.

## Engine

The C++ code for the game engine is located inside `engine`. **You
will not need to modify any code within the engine folder** and for
your sanity, please don't. However, you will need to **read lots of
code within the engine** in order to know how to use it. You will
usually only need to consult header files to know *how to use* an
object/interface, and rarely peek into a cpp file to see *how
something works*.

## Content

Place all of your code within the folder `content`. CMake will handle
include paths to files within the engine, so you should be able to
just include the name of the file you wish to use, e.g. `#include
"engine.h"`, and it will just work. **Do not use relative file paths,
e.g. `#include "../../engine/graphics/sprite.h`!**

## Assets

The engine can load 2D graphical sprites from png images (called a
[spritesheet](https://en.wikipedia.org/wiki/Texture_atlas)) using a
simple asset file. The first line of the asset file must be the file
name of the png image. The rest of the file can contain a list of
named sprites and their locations (x, y, width, height) within the
image file. Optionally, if a sprite is animated the number of frames
in the animation can appear after the location. It is assumed that
frames appear horizontally in the image and are equally-spaced. Here's
the format:

```
image_filename.png

name_of_sprite x y width height
animated_sprite x y width height frames
```

The art for the dungeon was designed by and can be purchased from
[SecretHideout](https://secrethideout.itch.io/rogue-dungeon-tileset-16x16).

The heros, monsters, weapons, and items were created by and can be
purchased from [Robert(0x72)](https://0x72.itch.io/dungeontileset-ii).

**You do not have permission to distribute these files, so don't push
them to github!**