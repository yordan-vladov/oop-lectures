#pragma once
#include "core/IButton.h"

class WindowsButton : public IButton {
public:
    void render() const override;
};
