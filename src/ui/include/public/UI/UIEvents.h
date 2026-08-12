#pragma once

#include "raylib.h"
namespace UI {
namespace Events {
class Widget;
struct MouseEvent {
    Widget* HittedWidget;
    Widget* CapturedWidget;
    Vector2 MouseDelta;
};




}
}