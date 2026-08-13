#include "raylib.h"
#include <UI/Widget.h>

namespace UI
{
Widget::Widget(Widget* parent): 
    parent(parent)
{
    depth = parent?parent->depth+1:0;
}

bool Widget::isAncestorOf(const Widget* widget) const {
    return widget?widget->isDescendanceOf(this):false;
}


bool Widget::isDescendanceOf(const Widget* widget) const
{
    const Widget* currentParent = parent;
    while(currentParent != nullptr)
    {
       if(currentParent == widget) 
       {
            return true;
       }
       currentParent = currentParent->parent;
    }
    return false;
}


VContainerWidget::VContainerWidget(Widget* parent): Widget(parent) {

}

void VContainerWidget::UpdateLayout()  {
    rect.height = border * 2.0f;
    int realChildCount = 0;
    for (int i = 0; i < m_children.size(); i++)
    {
        Widget* child = m_children[i];
        if (child == nullptr)
        {
            TraceLog(LOG_WARNING, std::format("{}::UpdateLayout() child index {} null", GetName(), i).c_str());
            continue;
        }
        realChildCount++;
        child->rect.x = rect.x + border;
        child->rect.y = rect.y + border;
        if (i > 0)
        {
            Widget* prevChild = nullptr;
            int prev = i;
            do
            {
                prevChild = m_children[--prev];
            } while (prevChild == nullptr && prev > 0);
            if (prevChild != nullptr)
            {
                child->rect.y = prevChild->rect.y + prevChild->rect.height + spacing;
            }
        }
        rect.height += child->rect.height;
    }
    rect.height += (realChildCount - 1.0f) * spacing;
}

void VContainerWidget::Draw() const
{
    Color color = {89, 89, 89, 255};
    DrawRectangleRec(rect, color);
}


Block::Block(Widget* parent) : Widget(parent)
{
}

EventResult Block::OnMouseEnter(Events::MouseEvent event)
{
    EventResult result = {
	    .Consumed = false,
	    .Handled = true,
    };
    
    isHover = true;
    return result;
}

EventResult Block::OnMouseLeave(Events::MouseEvent event)
{
    EventResult result = {
	    .Consumed = false,
	    .Handled = true,
    };
    isHover = false;
    return result;
}

EventResult Block::OnMouseDown(Events::MouseEvent event)
{
    EventResult result = {
	    .Consumed = false,
	    .Handled = true,
    };
    isMouseDown = true;
    return result;
}

EventResult Block::OnMouseUp(Events::MouseEvent event)
{
    EventResult result = {
	    .Consumed = false,
	    .Handled = true,
    };
    isMouseDown = false;
    return result;
}

void Block::Draw() const
{
    Color _color = isHover ? RED : GREEN;
    _color = isMouseDown?YELLOW:_color;
    DrawRectangleRec(rect, _color);
}
} // namespace UI
