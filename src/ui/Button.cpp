#include <SDL3/SDL.h>
#include "ui/Button.h"

Button::Button(float x, float y, float width, float height, const std::function<void()> &onClick) : rect{x, y, width, height}, state(ButtonState::Normal), onClick(onClick){};

void Button::handleEvent(const SDL_Event &event) {
    SDL_FPoint point;
    switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION:
            point = {event.motion.x, event.motion.y};
            if (SDL_PointInRectFloat(&point, &rect)) {
                if (state != ButtonState::Pressed) {
                    state = ButtonState::Hover;
                }
            }
            else state = ButtonState::Normal;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                point = {event.button.x, event.button.y};
                if (SDL_PointInRectFloat(&point, &rect)) {
                    state = ButtonState::Pressed;
                }
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                point = {event.button.x, event.button.y};
                bool inside = SDL_PointInRectFloat(&point, &rect);
                if (state == ButtonState::Pressed && inside) {
                    if (onClick) onClick();
                    state = ButtonState::Hover;
                }
                else {
                    state = ButtonState::Normal;
                }
            }
            break;
        default:
            state = ButtonState::Normal;
            break;
    }
}

void Button::update(float delta) {
    (void)delta;
}

void Button::render(SDL_Renderer *renderer) {
    switch (state) {
        case ButtonState::Normal:
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            break;
        case ButtonState::Hover:
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
            break;
        case ButtonState::Pressed:
            SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
            break;
    }

    SDL_RenderFillRect(renderer, &rect);
}