#include "InputManager.h"

InputManager& InputManager::GetInstance() {
    static InputManager instance;
    return instance;
}

InputManager::InputManager() {
    // Inicialize arrays com false/0 e variáveis com 0
    updateCounter = 0;
    quitRequested = false;
    mouseX = 0; mouseY = 0;
    for(int i=0; i<6; i++) { mouseState[i] = false; mouseUpdate[i] = 0; }
}

// Destrutor
InputManager::~InputManager() {
    // Pode ficar vazio
}

void InputManager::Update() {
    SDL_Event event;
    // Atualiza a posição atual do mouse e o estado dos botões neste frame
    SDL_GetMouseState(&mouseX, &mouseY);
    quitRequested = false;
    updateCounter++; // Incrementa o contador de frames para KeyPress/MousePress

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) quitRequested = true;

        if (event.type == SDL_KEYDOWN) {
            if (event.key.repeat) continue; // Ignora repetição do SO
            keyState[event.key.keysym.sym] = true;
            keyUpdate[event.key.keysym.sym] = updateCounter;
        }
        
        if (event.type == SDL_KEYUP) {
            keyState[event.key.keysym.sym] = false;
            keyUpdate[event.key.keysym.sym] = updateCounter;
        }

        // Lógica de Mouse
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            mouseState[event.button.button] = true;
            mouseUpdate[event.button.button] = updateCounter;
        }
        if (event.type == SDL_MOUSEBUTTONUP) {
            mouseState[event.button.button] = false;
            mouseUpdate[event.button.button] = updateCounter;
        }
    }
}

// Métodos de consulta de Teclado
bool InputManager::KeyPress(int key) {
    return (keyState[key] && (keyUpdate[key] == updateCounter));
}
bool InputManager::KeyRelease(int key) {
    return (!keyState[key] && (keyUpdate[key] == updateCounter));
}
bool InputManager::IsKeyDown(int key) {
    return keyState[key];
}

// Métodos de consulta de Mouse
bool InputManager::MousePress(int button) {
    return (mouseState[button] && (mouseUpdate[button] == updateCounter));
}
bool InputManager::MouseRelease(int button) {
    return (!mouseState[button] && (mouseUpdate[button] == updateCounter));
}
bool InputManager::IsMouseDown(int button) {
    return mouseState[button];
}

int InputManager::GetMouseX() { return mouseX; }
int InputManager::GetMouseY() { return mouseY; }
bool InputManager::QuitRequested() { return quitRequested; }