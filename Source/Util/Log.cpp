#include "Log.hpp"
#include "Config.h"
#include <filesystem>

bool Log::initialized = false;

std::ofstream Log::verbose;
std::ofstream Log::info;
std::ofstream Log::warning;
std::ofstream Log::error;
std::ofstream Log::unexpected;

void Log::init() {
    if (!initialized) {
        initialized = true;
        std::filesystem::create_directory(LOG_DIR);
        Log::verbose.open("/Users/oscarprice/code/own_p/Meadow/logs/" "verbose.log");
        Log::info.open(LOG_DIR "info.log");
        Log::warning.open(LOG_DIR "warning.log");
        Log::error.open(LOG_DIR "error.log");
    }
}