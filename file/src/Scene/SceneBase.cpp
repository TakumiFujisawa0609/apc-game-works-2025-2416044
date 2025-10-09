#include "SceneBase.h"

void SceneBase::SetScene(SCENE s) {
    myScene_ = nextScene_ = s;
}

SceneBase::SCENE SceneBase::GetMyScene() const {
    return myScene_;
}

SceneBase::SCENE SceneBase::GetNextScene() const {
    return nextScene_;
}
