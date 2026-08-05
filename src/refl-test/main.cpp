#include "rfl/fields.hpp"
#include "rfl/internal/get_field_names.hpp"
#include <rfl.hpp>
#define RFL_EXTEND(x) rfl::Flatten<x> base;

struct Parent {
    float i_am_parent;
};

struct Child 
{
    RFL_EXTEND(Parent)
};

int main(){

    Child::i
    auto names = rfl::internal::get_field_names<Child>();
}