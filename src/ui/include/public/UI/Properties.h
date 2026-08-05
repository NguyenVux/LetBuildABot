#pragma once
#include <string>


namespace UI {
namespace Properties{

struct EdgeInsets {
	float top		= 0.0f;
	float bottom	= 0.0f;
	float left		= 0.0f;
	float right		= 0.0f;
};

struct Core {
    std::string Name;
};

struct LayoutProperties{
	float spacing;
	EdgeInsets Padding;
};

}
}