/**
 * @file Logging.hpp
 * @brief Header file for the Logging module.
 */

#pragma once

#include <chrono>
#include <fstream>
#include <string>

/**
 * @class Log
 * @brief Class for managing logging functionality.
 */
class Log {
  static bool initialized;

public:
  static void init();              /**< Initialize the logging module. */
  static std::ofstream verbose;    /**< Output stream for verbose logs. */
  static std::ofstream info;       /**< Output stream for info logs. */
  static std::ofstream warning;    /**< Output stream for warning logs. */
  static std::ofstream error;      /**< Output stream for error logs. */
  static std::ofstream unexpected; /**< Output stream for unexpected logs. */
  inline static void flush() {
    if (!initialized)
      init();
    verbose.flush();
    info.flush();
    warning.flush();
    error.flush();
    unexpected.flush();
  }
};

#ifdef NDEBUG
#define LogTime(s, var)                                                        \
  s; /** Macro for logging execution time in release mode. */
#else
/**
 * @def time(s, var)
 * @brief Macro for logging execution time in debug mode.
 * @param s The code to be executed.
 * @param var The variable to store the execution time.
 */
#define time(s, var)                                                           \
  auto start = std::chrono::high_resolution_clock::now();                      \
  s;                                                                           \
  std::chrono::duration<double> elapsed =                                      \
      std::chrono::high_resolution_clock::now() - start;                       \
  var = elapsed.count();
#endif
