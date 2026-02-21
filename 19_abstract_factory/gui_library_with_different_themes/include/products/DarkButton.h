#pragma once
#include "core/IButton.h"

class DarkButton : public IButton {
public:
    void render() const override;
};
