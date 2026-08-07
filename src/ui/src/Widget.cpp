#include <UI/Widget.h>

namespace UI
{
Widget::Widget(Widget* parent): 
    parent(parent)
{
    depth = parent?parent->depth+1:0;
}

bool Widget::isAncestorOf(const Widget* widget) const {
    return widget->isDescendanceOf(this);
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

bool Block::OnMouseEnter()
{
    isHover = true;
    // TraceLog(LOG_INFO, "Mouse enter %s", name.c_str());
    return false;
}

bool Block::OnMouseLeave()
{
    isHover = false;
    // TraceLog(LOG_INFO, "Mouse leave %s", name.c_str());
    return false;
}

bool Block::OnMouseDown()
{
    TraceLog(LOG_INFO, std::format("Mouse down: {}", GetName()).c_str());
    return true;
}

bool Block::OnMouseUp()
{
    TraceLog(LOG_INFO, std::format("Mouse up: {}", GetName()).c_str());
    return true;
}

void Block::Draw() const
{
    Color _color = isHover ? RED : GREEN;
    DrawRectangleRec(rect, _color);
}
} // namespace UI