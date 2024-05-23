#include "./headers/file_utils.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#endif

void sprout(std::string source_relative_path) {
    std::string destination_path = "./";

    try {
        std::string exe_path = get_executable_path();
        std::string source_path = exe_path + PATH_SEPARATOR + source_relative_path;

        copy_directory(source_path, destination_path);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void create(std::string source, std::string name) {
    try {
        copy_directory(source, get_executable_path() + PATH_SEPARATOR + name);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void remove(std::string directory) {
    try {
        delete_directory(get_executable_path() + PATH_SEPARATOR + directory);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void view() {
    try {
        std::string directory = get_executable_path();
        std::vector<std::string> folders = list_folders(directory);

        std::cout << "Sprout templates:\n";
        for (const std::string &folder : folders) {
            std::cout << folder << "\n";
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void help() {
    std::cout << "Commands:\n"
              << "-h            |   Opens the help menu\n"
              << "-s {name}     |   Instantiate (sprout) a template\n"
              << "-c {name}     |   Creates a new template based on the current directory\n"
              << "-d {name}     |   Deletes the template with the given name\n"
              << "-v            |   Displays all templates" << std::endl;
}