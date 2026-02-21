#pragma once
#include "core/IButton.h"

class LightButton : public IButton {
public:
    void render() const override;
};
