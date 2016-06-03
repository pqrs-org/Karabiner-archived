#pragma once

#include <cstdio>
#include <string>

namespace pqrs {
class process final {
public:
  static int launch(const std::string& command, std::string& out) {
    out.clear();

    FILE* fp = popen(command.c_str(), "r");
    if (!fp) {
      return -1;
    }

    while (!feof(fp)) {
      char buffer[256];
      if (!fgets(buffer, sizeof(buffer), fp)) {
        break;
      }
      out += buffer;
    }

    return pclose(fp);
  }
};
}
