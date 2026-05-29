#include "Text.h"
#include "Game.h"
#include "Camera.h"
#include "Resources.h"

Text::Text(GameObject& associated, std::string fontFile, int fontSize, TextStyle style, std::string text, SDL_Color color)
    : Component(associated), texture(nullptr), text(text), style(style), fontFile(fontFile), fontSize(fontSize), color(color) {
    RemakeTexture();
}

Text::~Text() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
}

void Text::Update(float dt) {
    // Podemos adicionar animações (como piscar) aqui se quisermos.
}

void Text::Render() {
    if (texture != nullptr) {
        SDL_Rect clipRect = {0, 0, (int)associated.box.w, (int)associated.box.h};
        
        // Posição final na tela levando em conta a Câmera
        SDL_Rect dstRect;
        dstRect.x = associated.box.x - Camera::pos.x;
        dstRect.y = associated.box.y - Camera::pos.y;
        dstRect.w = clipRect.w;
        dstRect.h = clipRect.h;

        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstRect, associated.angleDeg, nullptr, SDL_FLIP_NONE);
    }
}

bool Text::Is(std::string type) {
    return type == "Text";
}

void Text::SetText(std::string text) {
    this->text = text;
    RemakeTexture();
}

void Text::SetColor(SDL_Color color) {
    this->color = color;
    RemakeTexture();
}

void Text::SetStyle(TextStyle style) {
    this->style = style;
    RemakeTexture();
}

void Text::SetFontFile(std::string fontFile) {
    this->fontFile = fontFile;
    RemakeTexture();
}

void Text::SetFontSize(int fontSize) {
    this->fontSize = fontSize;
    RemakeTexture();
}

void Text::RemakeTexture() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    // Abre a fonte (no futuro, isso deverá ser feito via Resources::GetFont)
    font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr) {
        SDL_Log("Falha ao carregar a fonte %s: %s", fontFile.c_str(), TTF_GetError());
        return;
    }

    SDL_Surface* surface = nullptr;

    // Renderiza a superfície de acordo com o estilo escolhido
    if (style == SOLID) {
        surface = TTF_RenderText_Solid(font, text.c_str(), color);
    } else if (style == SHADED) {
        SDL_Color bgColor = {0, 0, 0, 255}; // Fundo preto para o estilo SHADED
        surface = TTF_RenderText_Shaded(font, text.c_str(), color, bgColor);
    } else if (style == BLENDED) {
        surface = TTF_RenderText_Blended(font, text.c_str(), color);
    }

    if (surface != nullptr) {
        // Converte a superfície em textura para desenhar na placa de vídeo
        texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surface);
        
        // Atualiza a box do associado com as medidas do texto
        associated.box.w = surface->w;
        associated.box.h = surface->h;
        
        SDL_FreeSurface(surface);
    } else {
        SDL_Log("Falha ao renderizar o texto: %s", TTF_GetError());
    }

    TTF_CloseFont(font);
}