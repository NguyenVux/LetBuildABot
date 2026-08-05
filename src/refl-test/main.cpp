#include "rfl/fields.hpp"
#include <rfl.hpp>
#include <iostream>

struct Parent {
    float i_am_parent;
};

struct Child 
{
    rfl::Flatten<Parent> parent;
};

int main(){
    auto names = rfl::fields<Child>();
}