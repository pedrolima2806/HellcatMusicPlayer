#pragma once

#include <filesystem>
#include <functional>

namespace fs = std::filesystem;

class musicPlayer {
private:
    MIX_Mixer *mixer;
    MIX_Track *track;
    std::vector<fs::path> trackList;
    int &musicNumber;

public:
    musicPlayer(MIX_Mixer *mixer, MIX_Track *track, const std::vector<fs::path> &trackList, int &musicNumber);

    void play();
    void pause();
    void resume();
    void next();
    void previous();

};