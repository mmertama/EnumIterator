# EnumIterator
Simple iterate over enum class, get value as a string or enumeration by string.

* C++17
* GCC, Clang. MinGW, MSVC19

<pre>
class EnumIterator {
  public:
  class Values<template <typename ENUM,
            auto RANGE_MIN = 0,
            auto RANGE_MAX = 63> {
            public:
            Iterator EnumIterator::Values::begin() const;
            Iterator EnumIterator::Values::end() const;
            static std::optional<std::string_view> nameOf(ENUM e);
            static std::optional<ENUM> valueOf(std::string_view str);
    };
  };
</pre>

* __ENUM__ - name of the ENUM class
* __RANGE_MIN__ - less or equal of the smallest enum value, default 0.
* __RANGE_MAX__ - more or equal of the largest enum value, default 63.

## Examples
  
  <pre>
    
enum class States {
    symptom,
    healed,
    zucker = 3
};  
  
for(const auto& [v, s] : EnumIterator::Values<States>{}) {
    std::cout << static_cast<int>(v) << " --> " << s << std::endl;
}

// outputs
> 0 --> States::symptom
> 1 --> States::healed
> 3 --> States::zucker
    
EnumIterator::Values<States>::valueOf("symptom"); // States::symptom
EnumIterator::Values<States>::nameOf(States::symptom)); // "symptom"

enum class Genophenes {
    treee = 92,
    hulien = 21,
    doomsday = 103
};

//here we have to set ranges due bigger values, you can also use enum values here.
 for(const auto& [v, s] : EnumIterator::Values<Genophenes, 20, 110>{}) {
        std::cout << static_cast<int>(v) << " --> " << s << std::endl;
    }

//outputs
> 21 --> Genophenes::hulien
> 92 --> Genophenes::treee
> 103 --> Genophenes::doomsday
            
 </pre> 
  
  MIT License, (c) Markus Mertama 2021
  
