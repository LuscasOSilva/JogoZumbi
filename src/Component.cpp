#include "Component.h"

// Inicializa a referência na lista de inicialização
Component::Component(GameObject& associated) : associated(associated) {}

Component::~Component() {}