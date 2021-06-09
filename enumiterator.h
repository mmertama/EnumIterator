#ifndef ENUMITERATOR_H
#define ENUMITERATOR_H

#include <iostream>
#include <string_view>
#include <functional>
#include <array>
#include <optional>

using namespace std::string_view_literals;

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

class EnumIterator {

    template <typename ENUM, ENUM VALUE, std::enable_if_t<std::is_enum<ENUM>::value, bool> = true>
    static constexpr std::string_view string() noexcept {
        return __PRETTY_FUNCTION__;
    }

    template<int index, char c, int current, typename S>
    static constexpr int pos(S f) noexcept {
        constexpr bool is_same = f()[current] == c;
        if constexpr (is_same) {
            if constexpr (index == 0)
                return current;
            else
                return pos<index - 1, c, current + 1, S>(f);
        } else {
            return pos<index, c, current + 1, S>(f);
        }
    }

    template <int index, char c, int at, typename ENUM, ENUM VALUE>
    static constexpr int find() noexcept {
        constexpr auto f = []()->std::string_view {
            constexpr auto s = string<ENUM, VALUE>();
            return s;
        };
        return pos<index, c, at, decltype(f)>(f);
    }

    template <typename ENUM, ENUM VALUE>
    static constexpr std::string_view name() noexcept {
#if defined(_MSC_VER)
        constexpr auto p1 = EnumIterator::find<1, ',', 0, ENUM, VALUE>();
        constexpr auto p2 = EnumIterator::find<2, ',', 0, ENUM, VALUE>();
        return std::string_view(EnumIterator::string<ENUM, VALUE>().data() + (p1 + 1), p2 - p1 - 1);
#elif defined(__clang__)
        constexpr auto p1 = EnumIterator::find<1, '=', 0, ENUM, VALUE>();
        constexpr auto p2 = EnumIterator::find<0, ']', p1, ENUM, VALUE>();
        return std::string_view(EnumIterator::string<ENUM, VALUE>().data() + (p1 + 2), p2 - p1 - 2);

#else
        constexpr auto p1 = EnumIterator::find<1, '=', 0, ENUM, VALUE>();
        constexpr auto p2 = EnumIterator::find<0, ';', p1, ENUM, VALUE>();
        return std::string_view(EnumIterator::string<ENUM, VALUE>().data() + (p1 + 2), p2 - p1 - 2);

#endif
    }

public:

    template <typename ENUM,
            auto RANGE_MIN = 0,
            auto RANGE_MAX = 63,
            int SZ = static_cast<int>(RANGE_MAX) - static_cast<int>(RANGE_MIN) + 1>
    class Values {
        template <int P>
        static void apply(std::array<std::string_view, SZ>& array) {
            if constexpr (P > 0) {
                constexpr auto pos = P - 1;
                array[pos] = name<ENUM, static_cast<ENUM>(pos + static_cast<int>(RANGE_MIN))>();
                apply<P - 1>(array);
            }
        }

        std::array<std::string_view, SZ> values = []() {
            std::array<std::string_view, SZ> array = {};
            apply<SZ>(array);
            return array;
        } ();

    static constexpr bool isName(const std::string_view& v) {
        return v.size() > 0 && std::isalpha(v[0]); // Invalid for clang is number, gcc is '(', as msvc
    }

     static constexpr std::optional<std::size_t> nameExt(const std::string_view& v) {
        constexpr char ENUM_SEP = ':';
        const auto p0 = v.find_first_of(ENUM_SEP);
        return (p0 != std::string_view::npos && p0 < v.size() + 1 && v[p0 + 1] == ENUM_SEP)
                ? std::make_optional(p0) : std::nullopt;
    }

    public:

        struct Iterator {
            using value_type = std::tuple<ENUM, std::string_view>;
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using pointer           = value_type;
            using reference         = value_type;

            reference operator*() const {return std::make_tuple(static_cast<ENUM>(m_index + RANGE_MIN), m_values[m_index]);}
            pointer operator->() {return operator*();}

            Iterator(const std::array<std::string_view, SZ>& val, int pos) : m_values(val), m_index(pos) {
                if(m_index < SZ - 1) { //expected to be valid on start
                    while(!isName(m_values[m_index]) && m_index < SZ - 1) {
                        ++m_index;
                    }
                }
            }

            Iterator& operator++() {
                do {
                    if(m_index >= SZ)
                        break;
                    ++m_index;
                } while(m_index < SZ && !isName(m_values[m_index]));
                return *this;
            }

            Iterator& operator--() {
                do {
                    if(m_index <= 0)
                        break;
                    --m_index;
                } while(m_index >= 0 && !isName(m_values[m_index]));
                return *this;
            }

            Iterator operator++(int) {
                auto tmp = *this;
                ++(*this);
                return tmp;
                }

            Iterator operator--(int) {
                auto tmp = *this;
                --(*this);
                return tmp;
                }

            friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_index == b.m_index; };

            friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_index != b.m_index; };

            const std::array<std::string_view, SZ>& m_values;
            int m_index;
        };

        /// begin
        Iterator begin() const {
            return Iterator(values, 0);
        }

        /// end
        Iterator end() const {
            return Iterator(values, SZ);
        }

        /// get a name from
        static std::optional<std::string_view> nameOf(ENUM e) {
            int index_of = 0;
            const auto names = EnumIterator::Values<ENUM, RANGE_MIN, RANGE_MAX>().values;
            for(const auto& v : names) {
                if(isName(v)) {
                    if(static_cast<int>(e) == index_of + static_cast<int>(RANGE_MIN))
                        return names[index_of];
                }
                ++index_of;
            }
            return std::nullopt;
        }

        /// get a value from
        static std::optional<ENUM> valueOf(std::string_view str) {
           int index_of = 0;
            const auto names = EnumIterator::Values<ENUM, RANGE_MIN, RANGE_MAX>().values;
            for(const auto& v : names) {
                if(isName(v)) {
                    const auto p0 = nameExt(v);
                    if((p0 && (*p0 < v.size() + 2 && v.substr(*p0 + 2) == str)) || str == v)
                        return static_cast<ENUM>(index_of + static_cast<int>(RANGE_MIN));
                }
                ++index_of;
            }
            return std::nullopt;
        }
    };
};



#endif // ENUMITERATOR_H
