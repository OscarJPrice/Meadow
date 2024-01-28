#include "Logging.hpp"
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
        std::filesystem::create_directory("Logs");
        Log::verbose.open("Logs/verbose.log");
        Log::info.open("Logs/info.log");
        Log::warning.open("Logs/warning.log");
        Log::error.open("Logs/error.log");
    }
}