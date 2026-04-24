#include "GameObject.h"

GameObject::GameObject() : isDead(false) {}

GameObject::~GameObject() {
    // Percorre de trás para frente para deletar com segurança
    for (int i = components.size() - 1; i >= 0; i--) {
        delete components[i];
    }
    components.clear();
}

void GameObject::Update(float dt) {
    for (auto cpt : components) {
        cpt->Update(dt);
    }
}

void GameObject::Render() {
    for (auto cpt : components) {
        cpt->Render();
    }
}

bool GameObject::IsDead() { return isDead; }

void GameObject::RequestDelete() { isDead = true; }

void GameObject::AddComponent(Component* cpt) {
    components.push_back(cpt);
}

void GameObject::RemoveComponent(Component* cpt) {
    for (int i = 0; i < components.size(); i++) {
        if (components[i] == cpt) {
            components.erase(components.begin() + i);
            break;
        }
    }
}