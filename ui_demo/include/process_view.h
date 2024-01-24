#pragma once

#include "view_base.h"

class ProcessView : public IView {
private:
public:
    bool Render() override;
};

bool ProcessView::Render() {
    return false;
}
