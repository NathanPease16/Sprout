#ifndef FILES_UTILS_H
#define FILES_UTILS_H

#include <string>
#include <vector>

std::string get_executable_path();

void copy_file (const std::string &source, const std::string &destination);

void copy_directory(const std::string &source, const std::string &destination);

void delete_directory(const std::string &directory);

std::vector<std::string> list_folders(const std::string &directory);

#endif