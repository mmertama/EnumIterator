# EnumIterator
Simple iterate over enum class, get value as a string or enumeration by string.
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
    
EnumIterator::Values<States>::valueOf("symptom");
EnumIterator::Values<States>::nameOf(States::symptom));
            
 </pre> 
  
  
