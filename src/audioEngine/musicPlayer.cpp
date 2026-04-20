#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "audioEngine/musicPlayer.h"

#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

void *playTrack (MIX_Mixer *mixer, const std::vector <fs::path> &trackList, int musicNumber) {
    const std::string pathString = trackList[musicNumber].c_str();
    MIX_Audio *audio = MIX_LoadAudio(mixer, pathString.c_str(), false);
    if (!audio) {
        std::cerr << "Could not load audio from " << pathString << std::endl;
    }

    MIX_Track *track = MIX_CreateTrack(mixer);
    if (!track) {
        std::cerr << "Could not create track from " << pathString << std::endl;
    }

    if (!MIX_SetTrackAudio(track, audio)) {
        std::cerr << "Could not set track audio from " << pathString << std::endl;
    };

    if (!MIX_PlayTrack(track, 0)) {
        std::cerr << "Could not play track from " << pathString << std::endl;
    };

    return nullptr;
};