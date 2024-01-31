#ifndef _MEADOW_TrackING_H_
#define _MEADOW_TrackING_H_

#include <map>
#include <vector>
#include "Any.hpp"
#include <chrono>
#include <iostream>
#include <stdint.h>

#ifdef NDEBUG
#define LogTime(s, var) s; /** Macro for logging execution time in release mode. */
#else

/**
 * @def time(s, var)
 * @brief Macro for logging execution time in debug mode.
 * @param s The code to be executed.
 * @param var The variable to store the execution time.
 */
#define time(s, var) \
        auto start = std::chrono::high_resolution_clock::now(); \
        s; \
        std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start; \
        var = elapsed.count();
#endif

extern std::map<std::pair<const char*, const char*>, Any> data;

extern

struct Tracking {


    template <typename T>
    static void add(const char* category, const char* key, T& variable) {
        auto& var = data[std::make_pair(category, key)] = variable;
    }


};

#ifdef NDEBUG
    #define track(category, variable) /** Macro for Tracking data in release mode. */
#else
#define track(category, variable) Tracking::add(category, #variable, variable); /** Macro for Tracking data in debug mode. */
#endif

#endif