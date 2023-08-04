#include "logger.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <ios>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {

  if (argc <= 1) {
    Logger::error("no hay parametros");
    exit(1);
  }

  std::string filename = argv[1];

  if (!std::filesystem::exists(filename)) {
    Logger::error("archivo " + filename + " no existe");
    exit(1);
  }

  std::filesystem::path filepath(std::filesystem::absolute(filename));

  std::ifstream in_stream(filepath.string());
  std::ofstream out_stream("raw_" + filepath.stem().string() + ".hpp");
  if (!in_stream.is_open()) {
    Logger::error("archivo no se puede abrir");
    exit(1);
  }

  in_stream.seekg(0, std::ios::end);
  size_t length = in_stream.tellg();
  in_stream.seekg(0, std::ios::beg);

  std::string upper_stem = "bin_" + filepath.stem().string();
  std::transform(upper_stem.begin(), upper_stem.end(), upper_stem.begin(),
                 ::toupper);

  // HEADER
  out_stream << "#ifndef _" << upper_stem << "_HPP_"
             << "\n";
  out_stream << "#define _" << upper_stem << "_HPP_"
             << "\n\n";
  out_stream << "const unsigned int " << upper_stem << "[] = {\n";
  out_stream << "\t";

  // BYTES
  int counter = 0;
  int new_len = 0;
  std::stringstream buffer{""};
  buffer << in_stream.rdbuf();
  for (char c : buffer.str()) {
    out_stream << "0x" << std::setfill('0') << std::setw(2) << std::hex
               << (int)c;
    out_stream << ", ";
    new_len++;
    if (counter++ == 15) {
      out_stream << "\n";
      out_stream << "\t";
      counter = 0;
    }
  }
  out_stream << "\n};\n";

  // ARRAY SIZE
  out_stream << "const unsigned int " << upper_stem << "_LEN = " << std::dec
             << new_len << ";\n\n";

  // FILENAME
  out_stream << "const char         " << upper_stem << "_FILENAME[] = " << filepath.filename()
             << ";\n";
  out_stream << "const unsigned int " << upper_stem
             << "_FILENAME_LEN = " << filepath.filename().string().size()
             << ";";
  out_stream << "\n\n";

  out_stream << "#endif // !_" << upper_stem << "_HPP_"
             << "\n";

  in_stream.close();
  out_stream.close();

  return 0;
}
