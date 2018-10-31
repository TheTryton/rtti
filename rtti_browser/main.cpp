#include "include/rtti_browser.hpp"
#include <QtWidgets>
#include "include/reflection/rtti.hpp"

using namespace REFLECTION;

template<class T>
struct name_resolver<T*>
{
    static inline string get_name()
    {
        return name_resolver<T>::get_name() + "*";
    }
};

template<class T>
struct name_resolver<const T*>
{
    static inline string get_name()
    {
        return "const " + name_resolver<T>::get_name() + "*";
    }
};

template<class E, class T, class A>
struct name_resolver<basic_string<E, T, A>>
{
    static inline string get_name()
    {
        return "string<" + name_resolver<E>::get_name() + ", " + name_resolver<T>::get_name() + ", " + name_resolver<A>::get_name() + ">";
    }
};

void register_fundamental_types()
{
#define R_F(T)\
reflection::get_instance().register_type<T>()

    R_F(void);
    R_F(bool);

    R_F(char);
    R_F(unsigned char);
    R_F(signed char);

    R_F(wchar_t);
    R_F(char16_t);
    R_F(char32_t);

    R_F(short int);
    R_F(unsigned short int);

    R_F(int);
    R_F(unsigned int);

    R_F(long int);
    R_F(unsigned long int);

    R_F(long long int);
    R_F(unsigned long long int);

    R_F(float);
    R_F(double);
    R_F(long double);

    //R_F(int8_t);
    //R_F(int16_t);
    //R_F(int32_t);
    //R_F(int64_t);

    //R_F(uint8_t);
    //R_F(uint16_t);
    //R_F(uint32_t);
    //R_F(uint64_t);
#undef R_F
}

void register_additional_types()
{
#define R_F(T)\
reflection::get_instance().register_type<T>()
    R_F(string);
    R_F(string);

    R_F(wstring);
    R_F(wstring);

    R_F(u16string);
    R_F(u16string);

    R_F(u32string);
    R_F(u32string);
#undef R_F
}

class A1
{
public:
    A1() {};
    A1(void* a) {};
    A1(int* a) {};
    A1(const char* a) {};
    A1(string* a) {};
public:
    int r1(const A1& other)
    {
        return int();
    }
    string r2(const A1* other)
    {
        return string();
    };
    wstring r3(A1* other)
    {
        return wstring();
    }
};

class A2
{
    A2() = delete;
};

class B : public A1, public A2
{

};

#include <atomic>
#include <iostream>

using namespace std;

atomic<size_t> g_allocated_memory;
atomic<size_t> g_deallocated_memory;

void* operator new(size_t sz) {
    g_allocated_memory += sz;
    return std::malloc(sz);
}

void operator delete(void* ptr, size_t sz) noexcept
{
    g_deallocated_memory += sz;
    std::free(ptr);
}

void* operator new[](size_t sz) {
    g_allocated_memory += sz;
    return std::malloc(sz);
}

void operator delete[](void* ptr, size_t sz) noexcept
{
    g_deallocated_memory += sz;
    std::free(ptr);
}

void print_alloc_dealloc()
{
    cout << "allocated: " << g_allocated_memory << "bytes" << endl;
    cout << "deallocated: " << g_deallocated_memory << "bytes" << endl;
}

int main(int argc, char *argv[])
{
    qDebug() << has_add_operator<int, int>::value << endl;
    //g_allocated_memory = 0;
    //g_deallocated_memory = 0;
    register_fundamental_types();
    register_additional_types();
    //reflection::get_instance().register_type<QString>();
    auto class_info = (class_info_impl<A1>*)reflection::get_instance().register_class<A1>();
    class_info->register_constructor<void*>("void*");
    class_info->register_constructor<int*>("int*");
    class_info->register_constructor<const char*>("char*");
    class_info->register_constructor<string*>("string*");
    class_info->register_method(&A1::r1, "r1");
    class_info->register_method(&A1::r2, "r2");
    class_info->register_method(&A1::r3, "r3");
    //reflection::get_instance().register_class<A2>();
    //reflection::get_instance().register_class<B, A1, A2>();
    //reflection::get_instance().register_type<type_index>();
    print_alloc_dealloc();

    QApplication a(argc, argv);
    rtti_browser w;
    w.show();
    return a.exec();
}
