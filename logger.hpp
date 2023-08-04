#include <iostream>
#include <string>

struct Logger {

  static void info(std::string msg) { show("info", msg); }
  static void error(std::string msg) { show("error", msg); }
  static void warn(std::string msg) { show("warn", msg); }
  static void debug(std::string msg) { show("debug", msg); }

private:
  static inline std::string m_format{"[%s]\t %s"};

  static void show(std::string level, std::string msg) {
    std::string formatted = format(level, msg);
    std::cout << formatted << "\n";
  }

  static std::string format(std::string level, std::string msg) {
    char buffer[100];
    int size =
        std::sprintf(buffer, m_format.c_str(), level.c_str(), msg.c_str());
    return std::string(buffer, buffer + size);
  }
};
