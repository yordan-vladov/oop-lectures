#pragma once
#include "core/IButton.h"

class MacButton : public IButton {
public:
    void render() const override;
};
