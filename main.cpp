#include "engine.h"
#include <SDL3/SDL.h>
#include <iostream>

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
        std::cout << "SDL_Init() failed: " << SDL_GetError() << '\n';
        return 1;
    }

    try {
        Settings settings{"settings.txt"};
        Engine engine{settings};

        std::shared_ptr<Entity> hero = engine.create_hero();

        engine.run();
    }
    catch (std::exception& e) {
        std::cout << e.what() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Quit();
}
