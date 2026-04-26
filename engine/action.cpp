#include "action.h"

Result success() {
    return {true, nullptr};
}
    
Result failure() {
    return {false, nullptr};
}
