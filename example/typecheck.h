#include <limits>

template<typename T>
bool isFloat(T var) {
    return std::numeric_limits<T>::is_iec559;  
}

template<typename T>
bool isInteger(T var) {
    return std::numeric_limits<T>::is_integer;  
}

template<typename T>
bool isSigned(T var) {
    return std::numeric_limits<T>::is_signed;
}

template<typename T>
bool isUnsigned(T var) {
    return !std::numeric_limits<T>::is_signed;  
}