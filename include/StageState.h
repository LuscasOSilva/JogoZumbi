#ifndef STAGESTATE_H
#define STAGESTATE_H

#include "State.h"
#include "Music.h"

// StageState herda de State
class StageState : public State {
public:
    StageState();
    ~StageState();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;

    void Start() override;
    void Pause() override;
    void Resume() override;

private:
    Music music;
};

#endif