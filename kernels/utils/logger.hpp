#pragma once

#include <iostream>
#include <string>

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";

void log(const std::string& level, const std::string& message) {
    std::string color = RESET; // 默认颜色

    if (level == "WARNING") {
        color = YELLOW;
    } else if (level == "ERROR") {
        color = RED;
    } else if (level == "INFO") {
        color = BLUE;
    } else if (level == "SUCCESS") {
        color = GREEN;
    }

    std::cout << color << "[" << level << "]: " << message << RESET << std::endl;
}