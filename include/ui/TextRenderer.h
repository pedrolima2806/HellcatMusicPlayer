#pragma once

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class TextRenderer {
private:
    SDL_Renderer *renderer;
    TTF_Font *font;

    public:
    TextRenderer(SDL_Renderer *renderer, TTF_Font *font);

    static std::string pathStrToText(const std::string &pathString);
    void renderText (const std::string &text, float x, float y);
};