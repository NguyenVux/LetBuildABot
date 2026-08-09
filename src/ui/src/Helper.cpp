#include <UI/Helper.h>
#include <UI/Widget.h>


namespace UI {
namespace Helper{


WidgetAncestorRange::Iterator::Iterator(Widget* widget) noexcept : current(widget)
{
}

Widget* WidgetAncestorRange::Iterator::operator*() const noexcept
{
    return current;
}

WidgetAncestorRange::Iterator& WidgetAncestorRange::Iterator::operator++() noexcept
{
    current = current->GetParent();
    return *this;
}

bool WidgetAncestorRange::Iterator::operator==(const Iterator& other) const noexcept
{
    return current == other.current;
}


}
}
		