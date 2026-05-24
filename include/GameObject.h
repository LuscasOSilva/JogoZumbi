#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>
#include <memory>
#include "Rect.h"
#include "Component.h"

class GameObject {
public:
    GameObject();
    ~GameObject();

    void Start(); // Inicia o GameObject e os seus componentes
    void Update(float dt);
    void Render();
    bool IsDead();
    void RequestDelete();
    void AddComponent(Component* cpt);
    void RemoveComponent(Component* cpt);

    // Método de busca por String
    Component* GetComponent(std::string type);

    // Método template fornecido pela disciplina e corrigido no T5
    template <typename T>
    T* GetComponent() {
        // Corrigido: Usar auto& para não copiar o unique_ptr, e usar .get()
        for (auto& cpt : components) {
            T* ptr = dynamic_cast<T*>(cpt.get());
            if (ptr != nullptr) return ptr;
        }
        return nullptr;
    }

    Rect box; // Posição e dimensões
    bool started; // Variável para controlar o estado de arranque
    double angleDeg; // Novo: Ângulo de rotação em graus

    void NotifyCollision(GameObject& other);

private:
    std::vector<std::unique_ptr<Component>> components; // Lista de componentes
    bool isDead; // Flag de destruição 
};

#endif