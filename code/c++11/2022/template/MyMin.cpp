// MyMin.cpp
#include "MyMin.h"

template<typename T>
const T& mymin(const T& a, const T& b) 
{
    return (a < b) ? a : b;
}