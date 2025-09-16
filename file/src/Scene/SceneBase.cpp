#include "SceneBase.h"

SceneBase::SCENE SceneBase::GetMyScene() const {
    return myScene_;
}

SceneBase::SCENE SceneBase::GetNextScene() const {
    return nextScene_;
}
