

#include <string>
#include <iostream>

template<typename T>
struct base_type
{
    static std::string name()
    {
        return T::get_name();
    }
};

// Для поля не подойдет, т.к. внутри класс нельзя делать специализацию шаблона
// И не получится объявить с помощью этого внутри класса внутри класса

#define TYPE_WITH_NAME(type, name)     \
    struct type;                       \
    template<typename T>               \
    struct name_container;             \
    template<>                         \
    struct name_container<type>        \
    {                                  \
        static std::string get_name()  \
        {                              \
            return name;               \
        }                              \
    };                                 \
    struct type : base_type<name_container<type>> 

template<typename T, typename... Ts>
struct extends : T, extends<Ts...>
{};

template<typename T>
struct extends<T> : T
{};

#define EXTENDS(...) , extends< __VA_ARGS__ >

TYPE_WITH_NAME(Type, "Type") {}; TYPE_WITH_NAME(Type1, "Type1") {};

TYPE_WITH_NAME(Type2, "Type2") EXTENDS(Type, Type1)
{};

int main(int argc, char** argv)
{
    std::cout << Type::name() << "\n";
    std::cout << Type1::name() << "\n";
    std::cout << Type2::Type::name() << "\n";
    std::cout << Type2::Type1::name() << "\n";
}
