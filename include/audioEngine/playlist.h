#pragma once

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class playlist {
    private:
    std::vector<fs::path> trackList;

    public:
    void getPlaylist (const std::string &folderPath);
    std::vector<fs::path> &getTrackList ();
};