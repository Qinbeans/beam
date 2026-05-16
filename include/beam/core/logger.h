#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace beam {

// inherit from std::ostream
namespace logger {
class Error : public std::ostream {
public:
  Error() : std::ostream(&buffer) {}

  // Custom message output for strings
  Error &operator<<(const std::string &message) {
    std::cerr << "[ERROR] " << message << std::endl;
    return *this;
  }

  // Support for other types (int, float, etc.)
  template <typename T> Error &operator<<(const T &value) {
    std::cerr << "[ERROR] " << value << std::endl;
    return *this;
  }

  // Support for stream manipulators like std::endl
  Error &operator<<(std::ostream &(*manip)(std::ostream &)) {
    std::cerr << "[ERROR] ";
    manip(std::cerr);
    return *this;
  }

private:
  std::stringbuf buffer;
};

class Warning : public std::ostream {
public:
  Warning() : std::ostream(&buffer) {}

  // Custom message output for strings
  Warning &operator<<(const std::string &message) {
    std::cerr << "[WARNING] " << message << std::endl;
    return *this;
  }

  // Support for other types (int, float, etc.)
  template <typename T> Warning &operator<<(const T &value) {
    std::cerr << "[WARNING] " << value << std::endl;
    return *this;
  }

  // Support for stream manipulators like std::endl
  Warning &operator<<(std::ostream &(*manip)(std::ostream &)) {
    std::cerr << "[WARNING] ";
    manip(std::cerr);
    return *this;
  }

private:
  std::stringbuf buffer;
};

class Info : public std::ostream {
public:
  Info() : std::ostream(&buffer) {}

  // Custom message output for strings
  Info &operator<<(const std::string &message) {
    std::cout << "[INFO] " << message << std::endl;
    return *this;
  }

  // Support for other types (int, float, etc.)
  template <typename T> Info &operator<<(const T &value) {
    std::cout << "[INFO] " << value << std::endl;
    return *this;
  }

  // Support for stream manipulators like std::endl
  Info &operator<<(std::ostream &(*manip)(std::ostream &)) {
    std::cout << "[INFO] ";
    manip(std::cout);
    return *this;
  }

private:
  std::stringbuf buffer;
};

class Debug : public std::ostream {
public:
  Debug() : std::ostream(&buffer) {}

  // Custom message output for strings
  Debug &operator<<(const std::string &message) {
    std::cout << "[DEBUG] " << message << std::endl;
    return *this;
  }

  // Support for other types (int, float, etc.)
  template <typename T> Debug &operator<<(const T &value) {
    std::cout << "[DEBUG] " << value << std::endl;
    return *this;
  }

  // Support for stream manipulators like std::endl
  Debug &operator<<(std::ostream &(*manip)(std::ostream &)) {
    std::cout << "[DEBUG] ";
    manip(std::cout);
    return *this;
  }

private:
  std::stringbuf buffer;
};

// Global logger instances
extern Error error;
extern Warning warning;
extern Info info;
extern Debug debug;
} // namespace logger

} // namespace beam
