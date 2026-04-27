#include <complex>
#include <iostream>
#include<filesystem>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "ui/Widgets.h"
#include "audioEngine/playlist.h"
#include "audioEngine/musicPlayer.h"
#include "ui/TextRenderer.h"

namespace fs = std::filesystem;

MIX_Track *audioToTrack(MIX_Mixer *mixer, const std::vector <fs::path> &trackList, MIX_Track *track, int &musicNumber);

int main() {
    //Initialization
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (!MIX_Init()) {
        std::cerr << "MIX_Init failed." << std::endl;
    }
    if (!TTF_Init()) {
        std::cerr << "TTF_Init failed." << std::endl;
    }

    //Playlist folder
    const auto playlistFolder = fs::path("/home/pedro/Music");
    playlist playlist;
    playlist.getPlaylist(playlistFolder);
    const auto trackList = playlist.getTrackList();

    //Window e renderer
    int width = 640, height = 480;
    SDL_Window *window = SDL_CreateWindow("hellcat_mp", width, height, 0);
    if (!window) {
        std::cerr << "Failed to create window:" << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Failed to create renderer:" << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //Mixer device
    MIX_Mixer *mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer) {
        std::cerr << "Failed to open audio device:" << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //Font
    TTF_Font *font = TTF_OpenFont("../assets/fonts/Quantico/Quantico-Regular.ttf", 16);
    if (!font) {
        std::cerr << "Failed to open font:" << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //Create and play track
    int musicNumber = 0;
    std::string pathString = trackList[musicNumber].c_str();
    MIX_Audio *audio = MIX_LoadAudio(mixer, pathString.c_str(), false);
    if (!audio) {
        std::cerr << "Could not load audio from " << pathString << std::endl;
    }
    MIX_Track *track = MIX_CreateTrack(mixer);
    if (!track) {
        std::cerr << "Could not create track from " << pathString << std::endl;
    }

    //Text display
    TextRenderer textRenderer(renderer, font);
    std::string text;
    int trackTitleWidth, trackTitleHeight;

    //Music player initialization
    musicPlayer hcMusicPlayer(mixer, track, trackList, musicNumber);
    hcMusicPlayer.play();

    //Buttons
    auto windowFloatWidth = static_cast<float>(width), windowFloatHeight = static_cast<float>(height);
    float buttonWidth = windowFloatWidth / 12.0f, buttonHeight = buttonWidth, playButtonX = windowFloatWidth / 2 - buttonWidth / 2, playButtonY = 10 * windowFloatHeight / 12, menuButtonX = windowFloatWidth / 24, menuButtonY = windowFloatHeight / 24;
    Button playPauseButton(playButtonX, playButtonY, buttonWidth, buttonHeight,
        [&hcMusicPlayer, track]() {
            if (MIX_TrackPlaying(track)) {
                hcMusicPlayer.pause();
            } else if (!MIX_TrackPlaying(track)) {
                hcMusicPlayer.resume();
            }
        });
    Button nextButton(playButtonX + 2 * buttonWidth, playButtonY, buttonWidth, buttonHeight,
        [&hcMusicPlayer]() {
            hcMusicPlayer.next();
        });
    Button previousButton(playButtonX - 2 * buttonWidth, playButtonY, buttonWidth, buttonHeight,
        [&hcMusicPlayer]() {
            hcMusicPlayer.previous();
        });
    Button menuButton(menuButtonX, menuButtonY, buttonWidth, buttonHeight,
        []() {
            std::cout << "Menu" << std::endl;
        });

    //Buttons textures
    SDL_Texture *playPauseButtonTexture = Button::textureGenerator(renderer, "../assets/images/ui/Buttons/playButtonOrange.png"),
    *nextButtonTexture = Button::textureGenerator(renderer, "../assets/images/ui/Buttons/nextButtonOrange.png"),
    *previousButtonTexture = Button::textureGenerator(renderer, "../assets/images/ui/Buttons/previousButtonOrange.png"),
    *menuButtonTexture = Button::textureGenerator(renderer, "../assets/images/ui/Buttons/catMenuLogoOrange.png");

    //Animation parameters
    size_t spritesheetRows = 2, spritesheetColumns = 5, spriteWidth = 128, spriteHeight = 128;
    Animation musicPlayerAnimation(playButtonX - buttonWidth/2, playButtonY-4.5*buttonHeight, 2*buttonWidth, 2*buttonHeight, spritesheetRows, spritesheetColumns, spriteWidth, spriteHeight, track);
    musicPlayerAnimation.animationRectsMaker(renderer, "../assets/images/ui/Animations/musicPlayerAnimation.png");

    //Define animations
    std::vector<std::pair<size_t, size_t>> animation1 = {{0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {1,0},{1,1},{1,2},{1,3},{1,4}};


    //set animation
    musicPlayerAnimation.setCurrentAnimation(animation1);

    //Loop cycle
    auto previousTime = std::chrono::steady_clock::now();
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT ||
                event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                running = false;
            }
            playPauseButton.handleEvent(event);
            nextButton.handleEvent(event);
            previousButton.handleEvent(event);
            menuButton.handleEvent(event);

            musicPlayerAnimation.handleEvent(event);
        }
        //background
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        //text rendering
        text = TextRenderer::pathStrToText(trackList[musicNumber].c_str());
        TTF_GetStringSize(font, text.c_str(), 0, &trackTitleWidth, &trackTitleHeight);
        textRenderer.renderText(text, windowFloatWidth/2 - static_cast<float>(trackTitleWidth)/2, playButtonY - buttonHeight / 2 - windowFloatHeight / 24);

        //deltaMs
        auto currentTime = std::chrono::steady_clock::now();
        double deltaMs = std::chrono::duration<double, std::milli>(currentTime - previousTime).count();
        previousTime = currentTime;

        //update
        musicPlayerAnimation.update(deltaMs);

        //button rendering
        playPauseButton.render(renderer, playPauseButtonTexture);
        nextButton.render(renderer, nextButtonTexture);
        previousButton.render(renderer, previousButtonTexture);
        menuButton.render(renderer, menuButtonTexture);

        //animation rendering
        musicPlayerAnimation.render(renderer, nullptr);

        SDL_RenderPresent(renderer);
    }

    //Finalization
    MIX_DestroyMixer(mixer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    MIX_Quit();
    SDL_Quit();

    return 0;
}