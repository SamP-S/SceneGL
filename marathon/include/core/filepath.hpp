#pragma once

#include <string>
#include <fstream>

bool CheckFileExists(const std::string& filepath) {
    std::ifstream file(filepath);
    return file.good();
}

std::string GetFileExtension(const std::string& filepath) {
    size_t dotPos = filepath.find_last_of(".");
    if (dotPos != std::string::npos) {
        return filepath.substr(dotPos);
    }
    return "";
}

std::string GetFileName(const std::string& filepath) {
    // get filename with ext
    std::string filename;
    size_t lastSlashPos = filepath.find_last_of("/");
    if (lastSlashPos != std::string::npos) {
        filename = filepath.substr(lastSlashPos + 1);
    }
    // remove ext
    size_t dotPos = filename.find_last_of(".");
    if (dotPos != std::string::npos) {
        return filename.substr(0, dotPos);
    }
    // return filename no ext
    return filename;
}

std::string GetParentFolder(const std::string& filepath) {
    size_t lastSlashPos = filepath.find_last_of("/");
    if (lastSlashPos != std::string::npos) {
        return filepath.substr(0, lastSlashPos);
    }
    return filepath;
}