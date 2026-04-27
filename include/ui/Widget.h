#pragma once
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"

class Widget {
    public:
    virtual ~Widget() = default;

    virtual void handleEvent(const SDL_Event &event) = 0;
    virtual void update(double dt) = 0;
    virtual void render(SDL_Renderer *renderer, SDL_Texture *texture) = 0;
};
