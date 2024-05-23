#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir _mkdir
#define PATH_SEPARATOR '\\'
#endif

std::string get_executable_path() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

void copy_file (const std::string &source, const std::string &destination) {
    std::ifstream src(source, std::ios::binary);
    std::ofstream dst(destination, std::ios::binary);
    dst << src.rdbuf();
}

void copy_directory(const std::string &source, const std::string &destination) {
    struct stat info;

    // Check if source directory actually exists or not
    if (stat(source.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR)) {
        throw std::runtime_error("Source directory does not exist or is not a directory");
    }

    // Create the destination directory if it does not exist
    if (stat(destination.c_str(), &info) != 0) {
        if (mkdir(destination.c_str()) != 0) {
            throw std::runtime_error("Unable to create destination directory");
        }
    }

    WIN32_FIND_DATAA file_data;
    HANDLE hFind = FindFirstFileA((source + "\\*").c_str(), &file_data);
    if (hFind == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to read source directory");
    }

    do {
        const std::string file_name = file_data.cFileName;
        if (file_name == "." || file_name == "..") continue;

        const std::string full_source_path = source + "\\" + file_name;
        const std::string full_destination_path = destination + "\\" + file_name;

        if (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            copy_directory(full_source_path, full_destination_path);
        } else {
            copy_file(full_source_path, full_destination_path);
        }
    } while (FindNextFileA(hFind, &file_data) != 0);
    FindClose(hFind);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Correct Usage:\n'sprout {starter}'\nExample: 'sprout express'" << std::endl;
        return 1;
    }
    

    std::string source_relative_path(argv[1]);
    std::string destination_path = "./";

    try {
        std::string exe_path = get_executable_path();
        std::string source_path = exe_path + PATH_SEPARATOR + source_relative_path;

        copy_directory(source_path, destination_path);
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}