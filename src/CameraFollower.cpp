#include "CameraFollower.h"
#include "Camera.h"

CameraFollower::CameraFollower(GameObject& associated) : Component(associated) {}

void CameraFollower::Update(float dt) {
    // Mantém o objeto sempre na posição (0,0) da tela, independente da câmera
    associated.box.x = Camera::pos.x;
    associated.box.y = Camera::pos.y;
}

void CameraFollower::Render() {}

bool CameraFollower::Is(std::string type) { return type == "CameraFollower"; }
