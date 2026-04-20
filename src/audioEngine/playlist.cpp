#include <iostream>
#include <filesystem>
#include <string>
#include "audioEngine/playlist.h"

#include <algorithm>

namespace fs = std::filesystem;

void playlist::getPlaylist (const std::string &folderPath) {
    trackList.clear();
    if (!fs::is_directory(folderPath) || !fs::exists(folderPath)) {
        std::cout << "Incorrect or invalid directory" << std::endl;
        return;
    }
    for (const auto &entry: fs::directory_iterator(folderPath)) {
        if (!entry.is_regular_file()) continue;
        auto ext = entry.path().extension().string();
        //lowercase ext
        std::ranges::transform(ext, ext.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (ext == ".mp3" || ext == ".wav" || ext == ".ogg" || ext == ".flac") {
            trackList.push_back(entry.path());
        }
    }
    std::cout << "Selected folder: " << folderPath << "\nPlaylist size: " << trackList.size() << std::endl;
    for (const auto &music : trackList) {
        std::cout << music << '\n';
    }
};

std::vector<fs::path> &playlist::getTrackList () {
    return trackList;
}
