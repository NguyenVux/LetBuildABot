#include <UI/UIManager.h>
#include <UI/Widget.h>
#include <UI/Helper.h>
#include <memory>
#include <stack>

namespace UI
{
UIManager::UIManager():
    root(std::make_unique<Widget>(nullptr))
{
    
}

Widget* UIManager::Raycast()
{
    Vector2 mousePos = GetMousePosition();
    Widget* hitted = nullptr;
    std::stack<Widget*> widgets;
    widgets.push(root.get());
    while (!widgets.empty())
    {
        Widget* top = widgets.top();
        widgets.pop();
        if (!top)
        {
            continue;
        }
        if (!top->Visible)
        {
            continue;
        }
        bool isHit = CheckCollisionPointRec(mousePos, top->rect);
        if (isHit)
        {
            hitted = top;
        }

        for (auto it = top->m_children.rbegin(); it != top->m_children.rend(); it++)
        {
            widgets.push(*it);
        }
    }

    return hitted;
}
const Widget* UIManager::GetCommonNode(const Widget* node1, const Widget* node2)
{
    if (!node1 || !node2)
    {
        return nullptr;
    }

    if (node1->depth < node2->depth)
    {
        std::swap(node1, node2);
    }

    while (node1->depth > node2->depth)
    {
        node1 = node1->GetParent();
    }

    while (node1 != node2)
    {
        node1 = node1->GetParent();
        node2 = node2->GetParent();

        if (!node1 || !node2)
        {
            return nullptr;
        }
    }

    return node1;
}
void UIManager::Update()
{
    Widget* hitted = Raycast();
    Events::MouseEvent mouseEvent;
    mouseEvent.CapturedWidget = capturedWidget;

    bool isMouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    // Get common ancestor
    auto commonNode = GetCommonNode(hitted, lastHittedWidget);
    Widget* currentMouseLeave = lastHittedWidget;
    Widget* currentMouseEnter = hitted;

    while (currentMouseLeave != commonNode)
    {
        // TraceLog(LOG_INFO, std::format("Mouse Leave: {}", currentMouseLeave->GetName()).c_str());
        currentMouseLeave->OnMouseLeave(mouseEvent);
        currentMouseLeave = currentMouseLeave->GetParent();
    }

    while (currentMouseEnter != commonNode)
    {
        // TraceLog(LOG_INFO, std::format("Mouse Enter: {}", currentMouseEnter->GetName()).c_str());
        currentMouseEnter->OnMouseEnter(mouseEvent);
        currentMouseEnter = currentMouseEnter->GetParent();
    }

    if (isMouseDown)
    {
        if (!capturedWidget)
        {
            capturedWidget = hitted;
            for(Widget* wid : Helper::WidgetAncestorRange::From(capturedWidget))
            {
                EventResult result = wid->OnMouseDown(mouseEvent);
		if(result.Consumed)
		{
			break;
		}
            }
        }
    }
    else
    {
        if (capturedWidget)
        {
            for(Widget* wid : Helper::WidgetAncestorRange::From(capturedWidget))
            {
                EventResult result = wid->OnMouseUp(mouseEvent);
		if(result.Consumed)
		{
			break;
		}
            }
            capturedWidget = nullptr;
        }
    }

    lastHittedWidget = hitted;
}

void UIManager::Draw()
{
    std::stack<Widget*> widgetsToDraw;
    widgetsToDraw.push(root.get());
    while (!widgetsToDraw.empty())
    {
        const Widget* top = widgetsToDraw.top();
        widgetsToDraw.pop();
        if (!top)
        {
            continue;
        }
        if (!top->Visible)
        {
            continue;
        }
        top->Draw();
        for (auto it = top->m_children.rbegin(); it != top->m_children.rend(); it++)
        {
            Widget* child = *it;
            widgetsToDraw.push(child);
        }
    }
}

} // namespace UI
