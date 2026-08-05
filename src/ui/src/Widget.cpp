#include <UI/Widget.h>

namespace UI
{

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
}