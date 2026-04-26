#pragma once

#include <vector>
#include <string>

class Input {
public:
    Input();
    ~Input();

    // Returns name of last key that was pressed, names are capitalized: Space, Right, A, B, etc.
    // Mouse clicks are indicated by returning Click.
    std::string get_last_keypress();

    // Returns the pixel location of the last mouse click
    [[maybe_unused]] std::pair<int, int> get_last_mouse_click();

    // Register keypress as the most recent
    void set_last_keypress(const std::string& key);
    
    // Returns list of all input events since last call
    std::vector<std::string> get_all_input_events();


private:
    std::string last_keypress;
    
    // pixel location of last click
    int mouse_x, mouse_y;
};
