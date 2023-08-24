#pragma once

#include <optional>
#include <string>
#include <vector>

struct command {
    std::string filename;
    std::vector<std::string> arguments;
    std::vector<std::string> env;
};
