#pragma once

#include "audio.h"
#include "camera.h"
#include "dungeon.h"
#include "entities.h"
#include "events.h"
#include "graphics.h"
#include "input.h"
#include "settings.h"

class Engine {
public:
    explicit Engine(const Settings& settings);

    // Add a hero or monster to the game
    std::shared_ptr<Entity> create_hero();
    std::shared_ptr<Entity> create_monster();

    // Remove entity from the game
    void remove_entity(Entity& entity);

    // Starting and stopping the game
    void run();
    void stop();     // immediately stops game
    void end_game(); // shows ending screen

    // Main engine components
    Input input;        // handles button presses and mouse clicks
    Audio audio;        // plays audio
    Entities entities;  // manages entities and when they can take their turn
    Graphics graphics;  // loads sprites and animated sprites
    Camera camera;      // draws content on screen given camera position
    Dungeon dungeon;    // contains the tiles of the game
    Events events;      // event system manage

    // Current hero in the game
    std::shared_ptr<Entity> hero;
    
private:
    bool running;
    void handle_input();
    void update();
    void render();
};

