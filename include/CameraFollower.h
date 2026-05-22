#ifndef CAMERAFOLLOWER_H
#define CAMERAFOLLOWER_H

#include "Component.h"

class CameraFollower : public Component {
public:
    CameraFollower(GameObject& associated);
    void Update(float dt) override;
    void Render() override;

    bool Is(std::string type) override;
};

#endif