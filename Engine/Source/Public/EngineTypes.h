#pragma once
#include "Debug/UDebug.h"

//System Libs 
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>

//redefine of the standard string 
typedef std::string UString;

//redefine all of the unsigned ints
typedef uint8_t UUi8;
typedef uint16_t UUi16;
typedef uint32_t UUi32;
typedef uint64_t UUi64;

// redefine the standard vector 
template <typename T>
using TArray = std::vector<T>;

//redefine smart pointers 
//redefine shared pointer 
template <typename T> 
using TShared = std::shared_ptr<T>;


//redefine unique pointer 
template <typename T>
using TUnique = std::unique_ptr<T>;

//redefine weak pointer 
template <typename T> 
using TWeak = std::weak_ptr<T>;

//redefine weak shared function
template <typename T, typename...Args>
TShared<T> TMakeShared(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);

}

//redefine weak unique function
template <typename T, typename...Args>
TUnique<T> TMakeUnique(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);

}