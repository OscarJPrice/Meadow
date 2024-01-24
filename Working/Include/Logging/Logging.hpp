#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <fstream>
#include <string>

class Log {
    static bool initialized;
public:
    static void init();
    static std::ofstream verbose;
    static std::ofstream info;
    static std::ofstream warning;
    static std::ofstream error;
};

#endif