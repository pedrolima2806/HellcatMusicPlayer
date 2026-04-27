#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

#include "ui/Widget.h"
#include "ui/Animation.h"

#include <chrono>

#include "../../vendored/SDL_mixer/src/SDL_mixer_internal.h"


Animation::Animation(float x, float y, float width, float height, size_t spritesheetRows, size_t spritesheetColumns,
                     size_t spriteWidth, size_t spriteHeight, MIX_Track *track)
    : rect{x, y, width, height}, spritesheetRows(spritesheetRows), spritesheetColumns(spritesheetColumns),
      spriteWidth(spriteWidth), spriteHeight(spriteHeight), track(track){
}

Animation::~Animation() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Animation::animationRectsMaker(SDL_Renderer *renderer, const std::string &path){
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load animation surface:" << SDL_GetError() << std::endl;
    }
    //remove background key color
    Uint8 keyR = 0, keyG = 0, keyB = 0, keyA = 0;
    if (!SDL_ReadSurfacePixel(surface, 0, 0, &keyR, &keyG, &keyB, &keyA)) {
        std::cerr << "Failed to read spriteSheet background color:" << SDL_GetError() << std::endl;
    }
    SDL_SetSurfaceColorKey(surface, false, SDL_MapSurfaceRGB(surface, keyR, keyG, keyB));
    //disable for png with transparent background
    //generate texture
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (!texture) {
        std::cerr << "Failed to create animation texture:" << SDL_GetError() << std::endl;
    }
    //render texture with transparent background:
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    animationRects.clear();
    //Separate the spritesheet in individual sprites
    for (size_t i = 0; i < spritesheetRows; i++) {
        for (size_t j = 0; j < spritesheetColumns; j++) {
            animationRects.push_back(SDL_FRect{
                static_cast<float>(j * spriteWidth), static_cast<float>(i * spriteHeight),
                static_cast<float>(spriteWidth), static_cast<float>(spriteHeight)
            });
        }
    }
}

void Animation::setCurrentAnimation(const std::vector<std::pair<size_t, size_t>> &animation) {
    currentAnimation = animation;
    frameIndex = 0;
    timeBuffer = 0;
}

void Animation::handleEvent(const SDL_Event &event) {
    (void) event;
}

void Animation::update(double deltaMs) {
    if (track && MIX_TrackPlaying(track)) {
        state = AnimationState::Playing;
    } else {
        state = AnimationState::Paused;
    }

    if (state != AnimationState::Playing) {
        return;
    }

    if (currentAnimation.empty()) {
        return;
    }

    timeBuffer += deltaMs;

    while (timeBuffer >= frameDurationMs) {
        frameIndex = (frameIndex + 1) % currentAnimation.size();
        timeBuffer -= frameDurationMs;
    }
}

void Animation::render(SDL_Renderer *renderer, SDL_Texture *externalTexture) {
    (void) externalTexture;
    if (animationRects.empty()) {
        std::cerr << "Animation rects must have been created" << std::endl;
        return;
    }
    if (currentAnimation.empty()) {
        std::cerr << "current Animation is empty" << std::endl;
        return;
    }
    if (frameIndex >= currentAnimation.size()) {
        frameIndex = 0;
    }
    auto currentPair = currentAnimation[frameIndex];
    size_t position = currentPair.first * spritesheetColumns + currentPair.second;

    if (position >= animationRects.size()) {
        std::cerr << "Invalid animation frame position" << std::endl;
        return;
    }

    SDL_FRect src = animationRects[position], dst = rect;

    SDL_RenderTexture(renderer, texture, &src, &dst);
}
