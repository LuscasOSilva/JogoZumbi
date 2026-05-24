#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

// Forward declaration para evitar inclusão circular
class GameObject;

class Component {
public:
    // O construtor deve receber o GameObject ao qual pertence
    Component(GameObject& associated);
    virtual ~Component();

    virtual void Start(); // Nova etapa do ciclo de vida

    // Métodos virtuais puros: obrigatórios nas classes filhas
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual bool Is(std::string type) = 0;

    virtual void NotifyCollision(GameObject& other) {}

protected:
    GameObject& associated; // Referência ao dono do componente
};

#endif