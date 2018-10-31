#pragma once

#define ARGUMENT32(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, ...) a32
#define COUNT_ARGUMENTS(...) ARGUMENT32(__VA_ARGS__, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8 ,7, 6, 5, 4, 3, 2, 1, 0)

#define IDENTITY(x) x

#define APPLY(macro, ...) IDENTITY(macro(__VA_ARGS__))

#define MAP1(m, x)      m(x)
#define MAP2(m, x, ...) m(x) IDENTITY(MAP1(m, __VA_ARGS__))
#define MAP3(m, x, ...) m(x) IDENTITY(MAP2(m, __VA_ARGS__))
#define MAP4(m, x, ...) m(x) IDENTITY(MAP3(m, __VA_ARGS__))
#define MAP5(m, x, ...) m(x) IDENTITY(MAP4(m, __VA_ARGS__))
#define MAP6(m, x, ...) m(x) IDENTITY(MAP5(m, __VA_ARGS__))
#define MAP7(m, x, ...) m(x) IDENTITY(MAP6(m, __VA_ARGS__))
#define MAP8(m, x, ...) m(x) IDENTITY(MAP7(m, __VA_ARGS__))
#define MAP9(m, x, ...) m(x) IDENTITY(MAP8(m, __VA_ARGS__))
#define MAP10(m, x, ...) m(x) IDENTITY(MAP9(m, __VA_ARGS__))
#define MAP11(m, x, ...) m(x) IDENTITY(MAP10(m, __VA_ARGS__))
#define MAP12(m, x, ...) m(x) IDENTITY(MAP11(m, __VA_ARGS__))
#define MAP13(m, x, ...) m(x) IDENTITY(MAP12(m, __VA_ARGS__))
#define MAP14(m, x, ...) m(x) IDENTITY(MAP13(m, __VA_ARGS__))
#define MAP15(m, x, ...) m(x) IDENTITY(MAP14(m, __VA_ARGS__))
#define MAP16(m, x, ...) m(x) IDENTITY(MAP15(m, __VA_ARGS__))

#define CHOOSE_MAP_START(count) MAP##count

#define MAP(macro, ...) \
    IDENTITY( \
        APPLY(CHOOSE_MAP_START, COUNT_ARGUMENTS(__VA_ARGS__)) \
            (macro, __VA_ARGS__))

#define IGNORE_ASSIGN_SINGLE(expression) (ignore_assign)expression,
#define IGNORE_ASSIGN(...) IDENTITY(MAP(IGNORE_ASSIGN_SINGLE, __VA_ARGS__))

#define IGNORE_ASSIGN_CAST_SINGLE(expression) (underlying_type)((int64)((ignore_assign)expression)),
#define IGNORE_ASSIGN_CAST(...) IDENTITY(MAP(IGNORE_ASSIGN_CAST_SINGLE, __VA_ARGS__))

#define STRINGIZE_SINGLE(expression) string(#expression),
#define STRINGIZE(...) IDENTITY(MAP(STRINGIZE_SINGLE, __VA_ARGS__))

#define ENUM(name, u_type, ...) \
class name\
{\
public:\
    enum TYPE\
    {\
        __VA_ARGS__\
    };\
    using underlying_type = u_type;\
    using enum_type = TYPE;\
    static constexpr array<int64, COUNT_ARGUMENTS(__VA_ARGS__)> values_int64 =\
    {\
        IDENTITY(IGNORE_ASSIGN(__VA_ARGS__))\
    };\
    static constexpr array<underlying_type, COUNT_ARGUMENTS(__VA_ARGS__)> values =\
    {\
        IDENTITY(IGNORE_ASSIGN_CAST(__VA_ARGS__))\
    };\
    static constexpr array<string_view, COUNT_ARGUMENTS(__VA_ARGS__)> names =\
    {\
        IDENTITY(STRINGIZE(__VA_ARGS__))\
    };\
};


#define REGISTER_TEMPLATE1(template_)\
auto g_##template_##_static_template_info = reflection::get_instance().register_template<template_>(1);\
template<class T1>\
struct resolve_template<template_<T1>> : public true_type \
{\
    static type_index                   native_template_info()\
    {\
        return type_index(typeid(template_));\
    }\
    static const template_info*         template_info()\
    {\
        return reflection::get_instance().find_template<template_>();\
    }\
    static vector<type_index>           native_args_types()\
    {\
        return {\
            type_index(typeid(T1))\
        };\
    }\
    static vector<const type_info*>     args_types()\
    {\
        return {\
            reflection::get_instance().find_type<T1>() \
        };\
    }\
};

#define REGISTER_TEMPLATE2(template_)\
auto g_##template_##_static_template_info = reflection::get_instance().register_template<template_>(2);\
template<class T1, class T2>\
struct resolve_template<template_<T1, T2>> : public true_type \
{\
    static type_index                   native_template_info()\
    {\
        return type_index(typeid(template_));\
    }\
    static const template_info*         template_info()\
    {\
        return reflection::get_instance().find_template<template_>();\
    }\
    static vector<type_index>           native_args_types()\
    {\
        return {\
            type_index(typeid(T1)),\
            type_index(typeid(T2))\
        };\
    }\
    static vector<const type_info*>     args_types()\
    {\
        return {\
            reflection::get_instance().find_type<T1>(), \
            reflection::get_instance().find_type<T2>() \
        };\
    }\
};

#define REGISTER_TEMPLATE3(template_)\
auto g_##template_##_static_template_info = reflection::get_instance().register_template<template_>(3);\
template<class T1, class T2, class T3>\
struct resolve_template<template_<T1, T2, T3>> : public true_type \
{\
    static type_index                   native_template_info()\
    {\
        return type_index(typeid(template_));\
    }\
    static const template_info*         template_info()\
    {\
        return reflection::get_instance().find_template<template_>();\
    }\
    static vector<type_index>           native_args_types()\
    {\
        return {\
            type_index(typeid(T1)),\
            type_index(typeid(T2)),\
            type_index(typeid(T3))\
        };\
    }\
    static vector<const type_info*>     args_types()\
    {\
        return {\
            reflection::get_instance().find_type<T1>(), \
            reflection::get_instance().find_type<T2>(), \
            reflection::get_instance().find_type<T3>() \
        };\
    }\
};

#define REGISTER_TEMPLATE4(template_)\
auto g_##template_##_static_template_info = reflection::get_instance().register_template<template_>(4);\
template<class T1, class T2, class T3, class T4>\
struct resolve_template<template_<T1, T2, T3, T4>> : public true_type \
{\
    static type_index                   native_template_info()\
    {\
        return type_index(typeid(template_));\
    }\
    static const template_info*         template_info()\
    {\
        return reflection::get_instance().find_template<template_>();\
    }\
    static vector<type_index>           native_args_types()\
    {\
        return {\
            type_index(typeid(T1)),\
            type_index(typeid(T2)),\
            type_index(typeid(T3)),\
            type_index(typeid(T4))\
        };\
    }\
    static vector<const type_info*>     args_types()\
    {\
        return {\
            reflection::get_instance().find_type<T1>(), \
            reflection::get_instance().find_type<T2>(), \
            reflection::get_instance().find_type<T3>(), \
            reflection::get_instance().find_type<T4>() \
        };\
    }\
};