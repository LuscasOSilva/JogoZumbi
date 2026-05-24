#include "GameObject.h"

// Construtor
GameObject::GameObject() {
    isDead = false;
    started = false; // Começa sempre como falso
    angleDeg = 0;
}

void GameObject::Start() {
    for (size_t i = 0; i < components.size(); i++) {
        components[i]->Start();
    }
    started = true; // Marca o GameObject como iniciado
}

GameObject::~GameObject() {
    // O vector limpa os unique_ptr automaticamente porque unique_ptr tem essa regra rigida
    components.clear();
}

void GameObject::Update(float dt) {
    for (auto& cpt : components) {
        cpt->Update(dt);
    }
}

void GameObject::Render() {
    for (auto& cpt : components) {
        cpt->Render();
    }
}

bool GameObject::IsDead() { return isDead; }

void GameObject::RequestDelete() { isDead = true; }

void GameObject::AddComponent(Component* cpt) {
    components.emplace_back(cpt);

    // Se o GameObject já tiver iniciado, arranca o novo componente de imediato
    if (started) {
        cpt->Start();
    }
}

void GameObject::RemoveComponent(Component* cpt) {
    for (int i = 0; i < components.size(); i++) {
        if (components[i].get() == cpt) {
            components.erase(components.begin() + i);
            break;
        }
    }
}

Component* GameObject::GetComponent(std::string type) {
    for (auto& cpt : components) {
        if (cpt->Is(type)) {
            return cpt.get();
        }
    }
    return nullptr;
}

void GameObject::NotifyCollision(GameObject& other) {
    for (int i = 0; i < components.size(); i++) {
        components[i]->NotifyCollision(other);
    }
}