#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "ui/Widgets.h"

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
    SDL_Texture *playPauseButtonTexture = textureGenerator(renderer, "../assets/images/playPause.png"),
    *nextButtonTexture = textureGenerator(renderer, "../assets/images/nextButton.png"),
    *previousButtonTexture = textureGenerator(renderer, "../assets/images/previousButton.png");


    //Buttons
    auto windowFloatWidth = static_cast<float>(width), windowFloatHeight = static_cast<float>(height);
    float buttonWidth = windowFloatWidth / 12.0f, buttonHeight = buttonWidth, buttonX = windowFloatWidth / 2 - buttonWidth / 2, buttonY = 10 * windowFloatHeight / 12;
    Button playPauseButton(buttonX, buttonY, buttonWidth, buttonHeight,
        []() {
            std::cout << "Play" << std::endl;
        });
    Button nextButton(buttonX + 2 * buttonWidth, buttonY, buttonWidth, buttonHeight,
        []() {
            std::cout << "Next" << std::endl;
        });
    Button previousButton(buttonX - 2 * buttonWidth, buttonY, buttonWidth, buttonHeight,
        []() {
            std::cout << "Previous" << std::endl;
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
        }
        playPauseButton.update(0.0f);
        nextButton.update(0.0f);
        previousButton.update(0.0f);

        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        playPauseButton.render(renderer, playPauseButtonTexture);
        nextButton.render(renderer, nextButtonTexture);
        previousButton.render(renderer, previousButtonTexture);

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
