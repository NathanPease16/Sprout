#include "./headers/file_utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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

void delete_directory(const std::string &directory) {
    WIN32_FIND_DATAA find_file_data;
    HANDLE hFind = FindFirstFileA((directory + "\\*").c_str(), &find_file_data);

    if (hFind == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to open directory");
    }

    do {
        const std::string file_name = find_file_data.cFileName;
        if (file_name == "." || file_name == "..") continue;

        const std::string full_path = directory + "\\" + file_name;

        if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            delete_directory(full_path);
        } else {
            if (!DeleteFileA(full_path.c_str())) {
                throw std::runtime_error("Failed to delete file: " + full_path);
            }
        } 
    } while (FindNextFileA(hFind, &find_file_data) != 0);
    FindClose(hFind);

    if (!RemoveDirectoryA(directory.c_str())) {
        throw std::runtime_error("Failed to remove directory: " + directory);
    }
}

std::vector<std::string> list_folders(const std::string &directory) {
    std::vector<std::string> folders;

    WIN32_FIND_DATAA find_file_data;
    HANDLE hFind = FindFirstFileA((directory + "\\*").c_str(), &find_file_data);

    if (hFind == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to open directory");
    }

    do {
        const std::string file_name = find_file_data.cFileName;
        if (file_name == "." || file_name == "..") continue;

        if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            folders.push_back(file_name);
        }
    } while (FindNextFileA(hFind, &find_file_data) != 0);
    FindClose(hFind);

    return folders;
}