#pragma once
namespace UI {
	
class Widget;



namespace Helper{

class WidgetAncestorRange
{
public:
	class Iterator
	{
	public:
		explicit Iterator(Widget* widget) noexcept;
		Widget* operator*() const noexcept;
		Iterator& operator++() noexcept;
		bool operator==(const Iterator& other) const noexcept;
    private:
		Widget* current = nullptr;
	};

	explicit WidgetAncestorRange(Widget* first) noexcept
		: first(first)
	{
	}
	
	static WidgetAncestorRange From(Widget* from)	{ return WidgetAncestorRange(from);}
	

	Iterator begin() const noexcept
	{
		return Iterator(first);
	}

	Iterator end() const noexcept
	{
		return Iterator(last);
	}

private:
	Widget* first = nullptr;
	Widget* last = nullptr;
};
}
}