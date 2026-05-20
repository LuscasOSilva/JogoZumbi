#include "Component.h"

// Inicializa a referência na lista de inicialização
Component::Component(GameObject& associated) : associated(associated) {}

Component::~Component() {}

void Component::Start() {
    // Por defeito, não faz nada. 
    // Pode ser reescrito pelas classes derivadas (ex: Gun, Bullet) se necessário.
}