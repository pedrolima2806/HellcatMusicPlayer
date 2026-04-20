#include <iostream>
#include <SDL3_mixer/SDL_mixer.h>
#include "audioEngine/musicPlayer.h"

#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

musicPlayer::musicPlayer(MIX_Mixer *mixer, MIX_Track *track, const std::vector<fs::path> &trackList) : mixer(mixer), track(track), trackList(trackList){};

void musicPlayer::play() {
    if (trackList.empty()) return;
    std::string pathString = trackList[musicNumber].string();
    if (MIX_Audio *audio = MIX_LoadAudio(mixer, pathString.c_str(), false)) {
        MIX_SetTrackAudio(track, audio);
        MIX_PlayTrack(track, 0);
    }
    else {
        std::cerr << "Could not load audio from " << pathString << std::endl;
    }
}

void musicPlayer::pause() {
    if (trackList.empty()) return;
    MIX_PauseTrack(track);
    std::cout << "Paused" << musicNumber << std::endl;
}

void musicPlayer::resume() {
    if (trackList.empty()) return;
    MIX_ResumeTrack(track);
    std::cout << "Resumed" << musicNumber << std::endl;
}

void musicPlayer::next() {
    if (trackList.empty()) return;
    pause();
    musicNumber = (musicNumber + 1) % static_cast<int>(trackList.size());
    play();
    std::cout << "Next" << musicNumber << std::endl;
}

void musicPlayer::previous() {
    if (trackList.empty()) return;
    pause();
    musicNumber = (musicNumber - 1 + static_cast<int>(trackList.size())) % static_cast<int>(trackList.size());
    play();
    std::cout << "Previous" << musicNumber << std::endl;
}
