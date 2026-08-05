#pragma once
#include <raylib.h>
#include <string>
#include <vector>

class Widget
{
public:
    std::string name;
    std::vector<Widget*> m_children;
    bool Visible = true;
    bool Hover = false;
    Rectangle rect;
    int depth;
    Widget* parent = nullptr; 
    
    Widget(Widget* parent): 
        parent(parent)
    {
        depth = 0;
        if(parent)
        {
           depth = parent->depth + 1; 
        }
    }
    //===========================================================
    bool isAncestorOf(const Widget* widget) const;
    bool isDescendanceOf(const Widget* widget) const;
    //===================Event Functions=========================
    virtual bool OnMouseEnter() { return false; }
    virtual bool OnMouseLeave() { return false; }
    virtual bool OnMouseDown()  { return false; }
    virtual bool OnMouseUp()    { return false; }
    //===========================================================
     
    virtual void UpdateLayout() { }
    virtual void Draw() const   { }
    virtual ~Widget()           { }
};