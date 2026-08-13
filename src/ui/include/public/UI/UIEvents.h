#pragma once

#include "raylib.h"
namespace UI {
class Widget;


namespace Events {




struct MouseEvent {
    Widget* HittedWidget = nullptr;
    Widget* CapturedWidget = nullptr;
    Vector2 MouseDelta;
};




}
}
