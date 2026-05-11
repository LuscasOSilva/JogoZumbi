#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"
#include "Vec2.h"

class Camera {
public:
    // Move a câmera com base nas teclas ou no foco
    static void Update(float dt);
    // Faz a câmera focar em um objeto específico
    static void Follow(GameObject* newFocus);
    // Para de seguir o objeto
    static void Unfollow();

    static Vec2 pos;
    static Vec2 speed;

private:
    static GameObject* focus;
};

#endif