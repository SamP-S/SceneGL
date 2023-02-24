#include <type_traits>

template <int N, typename T, typename Enabled=void>
struct vec;

template <int N, typename T>
struct vec<N, T, std::enable_if_t<N == 2>> {
    T m[2];
    struct {
        union { T x, r, s; };
        union { T y, g, t; };
    };
};

template <int N, typename T>
struct vec<N, T, std::enable_if_t<N != 2>> {
    T m[N];
};

int main(int argc, char *argv[]) {
	vec<2, float> v2;
    vec<3, float> v3;
    
    return 0;
}