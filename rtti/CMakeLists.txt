cmake_minimum_required(VERSION 3.8)
project(rtti)

set(rtti_Sources
	#hash
	include/hash/hash.hpp
	include/hash/detail/fowler_noll_vo_hash.hpp
	include/hash/detail/hash_common.hpp
	include/hash/detail/jenkins_hash.hpp
	include/hash/detail/pjw_hash.hpp
	
	#reflection
	include/reflection/rtti.hpp
	include/reflection/common.hpp
	include/reflection/common_reflection_impl.inl
	include/reflection/reflection.hpp
	include/reflection/reflection.cpp
	include/reflection/reflection.inl
	include/reflection/reflection_defs.hpp
	
	include/reflection/type/type_info.hpp
	include/reflection/type/type_info.cpp
	include/reflection/type/type_info.inl
	
	include/reflection/type/constructor/constructor_info.hpp
	include/reflection/type/constructor/constructor_info.inl
	
	include/reflection/template_type/template_type_info.hpp
	include/reflection/template_type/template_type_info.cpp
	include/reflection/template_type/template_type_info.inl
	
	include/reflection/template/template_info.hpp
	include/reflection/template/template_info.cpp
	include/reflection/template/template_info.inl
	
	include/reflection/function/function_info.hpp
	include/reflection/function/function_info.inl
	
	include/reflection/enum/enum_info.hpp
	include/reflection/enum/enum_info.cpp
	include/reflection/enum/enum_info.inl
	
	include/reflection/class/class_info.hpp
	include/reflection/class/class_info.cpp
	include/reflection/class/class_info.inl
	
	include/reflection/class/method/class_method_info.hpp
	include/reflection/class/method/class_method_info.inl
	
	include/reflection/class/field/class_field_info.hpp
	include/reflection/class/field/class_field_info.inl
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${rtti_Sources})

add_library(rtti STATIC ${rtti_Sources})

target_include_directories(rtti PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(rtti PROPERTIES CXX_STANDARD 17)
set_target_properties(rtti PROPERTIES LINKER_LANGUAGE CXX)

