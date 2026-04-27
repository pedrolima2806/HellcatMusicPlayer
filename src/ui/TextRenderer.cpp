#include <filesystem>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "ui/TextRenderer.h"

namespace fs = std::filesystem;

TextRenderer::TextRenderer(SDL_Renderer *renderer, TTF_Font *font) : renderer(renderer), font(font) {};

std::string TextRenderer::pathStrToText(const std::string &pathString) {
    const fs::path textPath(pathString);
    return textPath.filename().string();
};

void TextRenderer::renderText(const std::string &text, float x, float y) {
    constexpr SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), text.size(), white);
    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    const SDL_FRect dst = {x, y, static_cast<float>(surface->w), static_cast<float>(surface->h)};
    SDL_RenderTexture(renderer, texture, nullptr, &dst);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
};