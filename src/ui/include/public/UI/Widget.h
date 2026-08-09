#pragma once
#include <rfl.hpp>
#include <raylib.h>
#include <vector>
#include <UI/Properties.h>

namespace UI{

class Widget
{
protected:
    std::string Name;
    Widget* parent = nullptr; 
public:
    std::vector<Widget*> m_children;
    bool Visible = true;
    bool Hover = false;
    Rectangle rect;
    int depth;

    Widget(Widget* parent);
    //===========================================================
    std::string_view GetName() const    { return Name; }
    Widget* GetParent()                 { return parent; }
    const Widget* GetParent() const     { return parent; }
    void SetName(std::string name)      { Name = name; }
    bool isAncestorOf(const Widget* widget) const;
    bool isDescendanceOf(const Widget* widget) const;
    //===================Event Functions=========================
    virtual bool OnMouseEnter() { return false; }
    virtual bool OnMouseLeave() { return false; }
    virtual bool OnMouseDown()  { return false; }
    virtual bool OnMouseUp()    { return false; }
    //======================Life Cycle===========================
    virtual void UpdateLayout() { }
    virtual void Draw() const   { }
    virtual ~Widget()           { }
};

class VContainerWidget : public Widget
{
public:
    float spacing = 5.0f; 
    float border = 5.0f;
    Properties::EdgeInsets Padding;

    VContainerWidget(Widget* parent);
    virtual void UpdateLayout() override;
    virtual void Draw() const override;
};


class Block : public Widget
{
private:
    Texture2D Normal;
    Texture2D Hover;
    Texture2D Down;
public:
    Color color = GREEN;
    bool isHover = false;
    Block(Widget* parent);
    //===================Event Functions=========================
    virtual bool OnMouseEnter() override;
    virtual bool OnMouseLeave() override;
    virtual bool OnMouseDown() override;
    virtual bool OnMouseUp() override;
    //===========================================================
    virtual void Draw() const override;
};

} // namespace UI