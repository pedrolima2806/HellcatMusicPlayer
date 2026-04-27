#pragma once

#include <functional>
#include "Widget.h"

enum class ButtonState {
    Normal,
    Hover,
    Pressed
};

class Button : public Widget {
private:
    SDL_FRect rect;
    ButtonState state;
    std::function<void()> onClick;

public:
    Button(float x, float y, float width, float height, const std::function<void()> &onClick);

    void handleEvent(const SDL_Event &event) override;
    void update(double delta) override;
    void render(SDL_Renderer *renderer, SDL_Texture *texture) override;

    static SDL_Texture* textureGenerator(SDL_Renderer *renderer, const std::string &path);
};