#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>
#include "Rect.h"
#include "Component.h"

class GameObject {
public:
    GameObject();
    ~GameObject();

    void Update(float dt);
    void Render();
    bool IsDead();
    void RequestDelete();
    void AddComponent(Component* cpt);
    void RemoveComponent(Component* cpt);
    
    // Método template fornecido pela disciplina 
    template <typename T>
    T* GetComponent() {
        for (auto cpt : components) {
            T* ptr = dynamic_cast<T*>(cpt);
            if (ptr != nullptr) return ptr;
        }
        return nullptr;
    }

    Rect box; // Posição e dimensões

private:
    std::vector<Component*> components; // Lista de componentes
    bool isDead; // Flag de destruição 
};

#endif