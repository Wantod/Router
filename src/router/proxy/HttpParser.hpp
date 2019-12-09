#pragma once

#include "../Type.hpp"

struct ParserRequest {
    char address[1024];
    char port[10];
};
