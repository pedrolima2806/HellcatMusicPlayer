#pragma once

#include <string>
#include <vector>
#include <utility>

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "Widget.h"

enum class AnimationState {
    Playing,
    Paused
};

class Animation : public Widget {
private:
    SDL_FRect rect{};
    SDL_Texture *texture = nullptr;

    AnimationState state = AnimationState::Paused;

    size_t spritesheetRows{};
    size_t spritesheetColumns{};
    size_t spriteWidth{};
    size_t spriteHeight{};

    size_t frameIndex = 0;

    MIX_Track *track = nullptr;

    std::vector<std::pair<size_t, size_t>> currentAnimation;
    std::vector<SDL_FRect> animationRects;

    double frameDurationMs = 50.0;
    double timeBuffer = 0.0;

public:
    Animation(float x, float y, float width, float height,
              size_t spritesheetRows, size_t spritesheetColumns,
              size_t spriteWidth, size_t spriteHeight,
              MIX_Track *track);

    ~Animation() override;

    void animationRectsMaker(SDL_Renderer *renderer, const std::string &path);

    void setCurrentAnimation(const std::vector<std::pair<size_t, size_t>> &currentAnimation);

    void handleEvent(const SDL_Event &event) override;
    void update(double deltaMs) override;
    void render(SDL_Renderer *renderer, SDL_Texture *texture) override;
};