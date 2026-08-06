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
public:
    std::vector<Widget*> m_children;
    bool Visible = true;
    bool Hover = false;
    Rectangle rect;
    int depth;
    Widget* parent = nullptr; 
    Widget(Widget* parent): 
        parent(parent)
    {
        depth = parent?parent->depth+1:0;
    }
    //===========================================================
    std::string_view GetName() const    { return Name; }
    void SetName(std::string name)      { Name = name; }
    bool isAncestorOf(const Widget* widget) const;
    bool isDescendanceOf(const Widget* widget) const;
    //===================Event Functions=========================
    virtual bool OnMouseEnter() { return false; }
    virtual bool OnMouseLeave() { return false; }
    virtual bool OnMouseDown()  { return false; }
    virtual bool OnMouseUp()    { return false; }
    //===========================================================
    //===========================================================
    virtual void UpdateLayout() { }
    virtual void Draw() const   { }
    virtual ~Widget()           { }
};
}