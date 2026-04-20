#include <iostream>
#include<filesystem>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "ui/Widgets.h"
#include "audioEngine/playlist.h"

namespace fs = std::filesystem;

SDL_Texture* textureGenerator(SDL_Renderer *renderer, const std::string &path);

int main() {
    //Initialization
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (!MIX_Init()) {
        std::cerr << "MIX_Init failed." << std::endl;
    }
    //Playlist folder
    playlist playlist;
    const auto playlistFolder = fs::path("/home/pedro/Music");
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

    //Buttons surfaces
    SDL_Texture *playPauseButtonTexture = textureGenerator(renderer, "../assets/images/ui/playButtonOrange.png"),
    *nextButtonTexture = textureGenerator(renderer, "../assets/images/ui/nextButtonOrange.png"),
    *previousButtonTexture = textureGenerator(renderer, "../assets/images/ui/previousButtonOrange.png"),
    *menuButtonTexture = textureGenerator(renderer, "../assets/images/ui/catMenuLogoOrange.png");


    //Buttons
    auto windowFloatWidth = static_cast<float>(width), windowFloatHeight = static_cast<float>(height);
    float buttonWidth = windowFloatWidth / 12.0f, buttonHeight = buttonWidth, playButtonX = windowFloatWidth / 2 - buttonWidth / 2, playButtonY = 10 * windowFloatHeight / 12, menuButtonX = windowFloatWidth / 24, menuButtonY = windowFloatHeight / 24;
    Button playPauseButton(playButtonX, playButtonY, buttonWidth, buttonHeight,
        []() {
            std::cout << "Play" << std::endl;
        });
    Button nextButton(playButtonX + 2 * buttonWidth, playButtonY, buttonWidth, buttonHeight,
        []() {
            std::cout << "Next" << std::endl;
        });
    Button previousButton(playButtonX - 2 * buttonWidth, playButtonY, buttonWidth, buttonHeight,
        []() {
            std::cout << "Previous" << std::endl;
        });
    Button menuButton(menuButtonX, menuButtonY, buttonWidth, buttonHeight,
        []() {
            std::cout << "Menu" << std::endl;
        });

    //loop cycle
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
        }
        playPauseButton.update(0.0f);
        nextButton.update(0.0f);
        previousButton.update(0.0f);
        menuButton.update(0.0f);

        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        playPauseButton.render(renderer, playPauseButtonTexture);
        nextButton.render(renderer, nextButtonTexture);
        previousButton.render(renderer, previousButtonTexture);
        menuButton.render(renderer, menuButtonTexture);

        SDL_RenderPresent(renderer);
    }


    //Finalization
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

SDL_Texture* textureGenerator(SDL_Renderer *renderer, const std::string &path) {
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image:" << SDL_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (!texture) {
        std::cerr << "Failed to create texture:" << SDL_GetError() << std::endl;
        return nullptr;
    }
    return texture;
}
