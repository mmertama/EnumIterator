#include <iostream>
#include <string>

#include "enumiterator.h"

enum class States {
symptom,
healed,
zucker = 3
};

enum class Genophenes {
    treee = 92,
    hulien = 21,
    doomsday = 103
};


enum class F99 : unsigned {
    bing, bang, bong
};

int main() {




    for(const auto& [v, s] : EnumIterator::Values<States>{}) {
        std::cout << static_cast<int>(v) << " --> " << s << std::endl;
    }

    EnumIterator::Values<States> v{};


    for(auto it = std::make_reverse_iterator(v.end()) ; it != std::make_reverse_iterator(v.begin()); ++it) {
        std::cout << static_cast<int>(std::get<0>(*it)) << " --> " << std::get<1>(*it) << std::endl;
    }

    for(const auto& [v, s] : EnumIterator::Values<F99>{}) {
        std::cout << static_cast<int>(v) << " --> " << s << std::endl;
    }

    for(const auto& [v, s] : EnumIterator::Values<Genophenes, 20, 110>{}) {
        std::cout << static_cast<int>(v) << " --> " << s << std::endl;
    }

    const auto prout = [](auto v0, auto n0) {
        std::cout << "Hello " << (v0 ? std::to_string(static_cast<int>(*v0)) : "N/A") << std::endl;
        std::cout << "Hello " << (n0? *n0 : "N/A") << std::endl;
    };

    prout(
            EnumIterator::Values<States>::valueOf("symptom"),
            EnumIterator::Values<States>::nameOf(States::symptom));

    prout(
            EnumIterator::Values<States>::valueOf("healed"),
            EnumIterator::Values<States>::nameOf(States::healed));

    prout(
            EnumIterator::Values<States>::valueOf("zucker"),
            EnumIterator::Values<States>::nameOf(States::zucker));

    prout(
            EnumIterator::Values<States>::valueOf("pizzaboy"),
            EnumIterator::Values<States>::nameOf(static_cast<States>(12)));

    prout(
            EnumIterator::Values<Genophenes, 20, 110>::valueOf("treee"),
            EnumIterator::Values<Genophenes, 20, 110>::nameOf(Genophenes::treee));

    prout(
            EnumIterator::Values<Genophenes, Genophenes::hulien, Genophenes::treee>::valueOf("hulien"),
            EnumIterator::Values<Genophenes, Genophenes::hulien, Genophenes::treee>::nameOf(Genophenes::hulien));

    prout(
            EnumIterator::Values<Genophenes, Genophenes::hulien, Genophenes::doomsday>::valueOf("doomsday"),
            EnumIterator::Values<Genophenes, Genophenes::hulien, Genophenes::doomsday>::nameOf(Genophenes::doomsday));

    return 0;
}
