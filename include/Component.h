#ifndef COMPONENT_H
#define COMPONENT_H

// Forward declaration para evitar inclusão circular
class GameObject;

class Component {
public:
    // O construtor deve receber o GameObject ao qual pertence
    Component(GameObject& associated);
    virtual ~Component();

    // Métodos virtuais puros: obrigatórios nas classes filhas
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;

protected:
    GameObject& associated; // Referência ao dono do componente
};

#endif