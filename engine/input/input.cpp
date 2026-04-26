#include "input.h"
#include <SDL3/SDL.h>
#include <stdexcept>

Input::Input()
    :mouse_x{-1}, mouse_y{-1} {
    if (!SDL_Init(SDL_INIT_EVENTS)) {
        std::string msg{"Unable to initialize SDL Events: "};
        msg += SDL_GetError();
        SDL_Quit();
        throw std::runtime_error(msg);
    }
}

Input::~Input() {
    SDL_Quit();
}

void Input::set_last_keypress(const std::string& key) {
    last_keypress = key;
}
std::string Input::get_last_keypress() {
    std::string key = last_keypress;
    last_keypress.clear();
    return key;
}

[[maybe_unused]] std::pair<int, int> Input::get_last_mouse_click() {
    return {mouse_x, mouse_y};
}

std::vector<std::string> Input::get_all_input_events() {
    std::vector<std::string> inputs;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            inputs.emplace_back("Quit");
        }
        else if (event.type == SDL_EVENT_KEY_DOWN) {
            SDL_Keycode key = event.key.key;
            std::string key_name = SDL_GetKeyName(key);
            inputs.push_back(key_name);
        }
        else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                inputs.emplace_back("Click");
                mouse_x = event.motion.x;
                mouse_y = event.motion.y;
            }
        }
    }
    return inputs;
}
