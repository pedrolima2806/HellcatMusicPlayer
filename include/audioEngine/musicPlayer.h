#pragma once

#include <filesystem>
#include <functional>

namespace fs = std::filesystem;

class MusicPlayer {
    public:
    void *playTrack (MIX_Mixer *mixer, const std::vector <fs::path> &trackList );



};