#pragma once
#include <raylib.h>
#include <memory>
#include <UI/UIEvents.h>
namespace UI
{

class Widget;

class UIManager
{
private:
    std::unique_ptr<Widget> root = nullptr;
    Widget* capturedWidget = nullptr;
    Widget* lastHittedWidget = nullptr;
    bool prevMouseState = 0x0;

    Widget* Raycast();
    const Widget* GetCommonNode(const Widget* node1, const Widget* node2);
public:
    UIManager();
    Widget* GetRootWidget() { return root.get(); }
    void UpdateLayout();
    void Update();
    void Draw();
};
} // namespace UI
