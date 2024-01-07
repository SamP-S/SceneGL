#pragma once

#include <iostream>
#include <cmath>
#include <assert.h>
#include "assertfunc.hpp"

namespace LA {

    // enum qualifier {
    //     highp,
    //     mediump,
    //     lowp,
    //     defaultp = highp
    // };

    typedef int length_t;
    // typedef qualifier precision;

    template<length_t L, typename T> struct vec;
    template<length_t R, length_t C, typename T> struct mat;


    /// ------------------------------------------------- ///

    template<int N, typename T, typename Enabled = void>
    struct vec_data;

    template<int N, typename T>
    struct vec_data<N, T, std::enable_if_t<N == 2>> {
        union {
            T m[2] = {0, 0};
            struct {
                union { T x, r, s; };
                union { T y, g, t; };
            };
        };
    };

    template<int N, typename T>
    struct vec_data<N, T, std::enable_if_t<N == 3>> {
        union {
            T m[3] = {0, 0, 0};
            struct {
                union { T x, r, s; };
                union { T y, g, t; };
                union { T z, b, p; };
            };
        };
    };

    template<int N, typename T>
    struct vec_data<N, T, std::enable_if_t<N == 4>> {
        union {
            T m[4] = {0, 0, 0, 0};
            struct {
                union { T x, r, s; };
                union { T y, g, t; };
                union { T z, b, p; };
                union { T w, a, q; };
            };
        };
    };

    template<int N, typename T>
    struct vec_data<N, T, std::enable_if_t<(N != 2 && N != 3 && N != 4)>> {
        T m[N];
    };

    template<int N, typename T>
    struct vec : vec_data<N, T> {

        public:
        static int length() { return N; }
        static int size() { return N; }

        // ---  Nullary Functions --- //
        int len() const { return N; }
        int height() const { return N; }
        int width() const { return 1; }

        // --- Constructors --- //
        vec(T scalar = 0) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) = scalar;
            }
        }

        vec(const vec<N,T>& v) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) = v[i];
            }
        }

        vec(const T arr[N]) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) = arr[N];
            }
        }

        vec(std::initializer_list<T> arr) {
            assert(arr.size() <= N);
            std::copy(arr.begin(), arr.end(), this->m);
        }

        // --- Unary Selector Functions --- //

        // Index Operator
        T & operator[](int i) {
            if (i > this->length() || i < 0) {
                throw std::out_of_range("index out of bounds");
            }
            return this->m[i];
        }

		T const& operator[](int i) const {
            if (i > this->length() || i < 0) {
                throw std::out_of_range("index out of bounds");
            }
            return this->m[i];
        }


        // --- Unary Arithmetic Operator Functions --- //

        vec<N, T> & operator=(const vec<N, T>& v) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) = v[i];
            }
            return *this;
        }

        // Addition and assign
        template<typename U>
        vec<N, T> & operator+=(U scalar) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) += scalar;
            }
            return *this;
        }
        template<typename U>
        vec<N, T> & operator+=(const vec<N, U>& v) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) += v.operator[](i);
            }
            return *this;
        }

        // Subtract and assign
        template<typename U>
        vec<N, T> & operator-=(U scalar) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) -= scalar;
            }
            return *this;
        }
        template<typename U>
        vec<N, T> & operator-=(const vec<N, U>& v) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) -= v.operator[](i);
            }
            return *this;
        }

        // Scalar multiplication
        template<typename U>
        vec<N, T> & operator*=(U scalar) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) *= scalar;
            }
            return *this;
        }
        // Component-wise multiplication
        template<typename U>
        vec<N, T> & operator*=(const vec<N, U>& v) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) *= v[i];
            }
            return *this;
        }

        // Scalar division
        template<typename U>
        vec<N, T> & operator/=(U scalar) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) /= scalar;
            }
            return *this;
        }
        // Component-wise division
        template<typename U>
        vec<N, T> & operator/=(const vec<N, U>& v) {
            for (int i = 0; i < this->length(); i++) {
                if (v[i] == 0) {
                    throw "ERROR: Divide by 0";
                }
                this->operator[](i) /= v[i];
            }
            return *this;
        }

        // Pre-Fix Increment
        vec<N, T> & operator++() {
            for (int i = 0; i < this->length(); i++) {
                ++this->operator[](i);
            }
            return *this;
        }
        // Pre-Fix Decrement
        vec<N, T> & operator--() {
            for (int i = 0; i < this->length(); i++) {
                --this->operator[](i);
            }
            return *this;
        }
        // Post-Fix Increment
        vec<N, T> operator++(int) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i)++;
            }
            return *this;
        }
        
        // Post-Fix Decrement
        vec<N, T> operator--(int) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i)--;
            }
            return *this;
        }

    };

    // --- Binary Arithmetic Operator Functions --- //

    // Addition
    template<int N, typename T, typename U>
    vec<N, T> operator+(vec<N, T> const& v, U scalar) {
        vec<N, T> total;
        for (int i = 0; i < v.len(); i++) {
            total[i] = v[i] + scalar;
        }
        return total;
    }
    template<int N, typename T, typename U>
    vec<N, T> operator+(U scalar, vec<N, T> const& v) {
        vec<N, T> total;
        for (int i = 0; i < v.len(); i++) {
            total[i] = scalar + v[i];
        }
        return total;
    }
    template<int N, typename T, typename U>
    vec<N, T> operator+(vec<N, T> const& v1, vec<N, U> const& v2) {
        vec<N, T> total;
        for (int i = 0; i < v1.len(); i++) {
            total[i] = v1[i] + v2[i];
        }
        return total;
    }

    // Subtraction
    template<int N, typename T, typename U>
    vec<N, T> operator-(vec<N, T> const& v, U scalar) {
        vec<N, T> total;
        for (int i = 0; i < v.len(); i++) {
            total[i] = v[i] - scalar;
        }
        return total;
    }
    template<int N, typename T, typename U>
    vec<N, T> operator-(U scalar, vec<N, T> const& v) {
        vec<N, T> total;
        for (int i = 0; i < v.len(); i++) {
            total[i] = scalar - v[i];
        }
        return total;
    }
    template<int N, typename T, typename U>
    vec<N, T> operator-(vec<N, T> const& v1, vec<N, U> const& v2) {
        vec<N, T> total;
        for (int i = 0; i < v1.len(); i++) {
            total[i] = v1[i] - v2[i];
        }
        return total;
    }

    // Scalar multiplication
    template<int N, typename T, typename U>
    vec<N, T> operator*(vec<N, T> const& v, U scalar) {
        vec<N, T> total;
        for (int i = 0; i < v.len(); i++) {
            total[i] = v[i] * scalar;
        }
        return total;
    }
    template<int N, typename T, typename U>
    vec<N, T> operator*(U scalar, vec<N, T> const& v) {
        vec<N, T> total;
        for (int i = 0; i < v.len(); i++) {
            total[i] = scalar * v[i];
        }
        return total;
    }
    // Component-wise multiplication
    template<int N, typename T, typename U>
    vec<N, T> operator*(vec<N, T> const& v1, vec<N, U> const& v2); 

    // Scalar division
    template<int N, typename T, typename U>
    vec<N, T> operator/(vec<N, T> const& v, U scalar) {
        if (scalar == 0) {
            throw "ERROR: Divide by 0";
        }
        vec<N, T> total;
        for (int i = 0; i < v.len(); i++) {
            total[i] = v[i] / scalar;
        }
        return total;
    }
    template<int N, typename T, typename U>
    vec<N, T> operator/(U scalar, vec<N, T> const& v) {
        vec<N, T> total;
        for (int i = 0; i < v.len(); i++) {
            if (v[i] == 0) {
                throw "ERROR: Divide by 0";
            }   
            total[i] = scalar / v[i];
        }
        return total;
    }
    // Component-wise division
    template<int N, typename T, typename U>
    vec<N, T> operator/(vec<N, T> const& v1, vec<N, U> const& v2) {
        vec<N, T> total;
        for (int i = 0; i < v1.len(); i++) {
            if (v2[i] == 0) {
                throw "ERROR: Divide by 0";
            }   
            total[i] = v1[i] / v2[i];
        }
        return total;
    }

    template<int N, typename T, typename U>
	vec<N, T> operator%(vec<N, T> const& v, U scalar) {
        vec<N, T> total;
        for (int i = 0; i < v.len(); i++) {
            total[i] = v[i] % scalar;
        }
        return total;
    }

	template<int N, typename T, typename U>
	vec<N, T> operator%(U scalar, vec<N, T> const& v) {
        vec<N, T> total;
        for (int i = 0; i < v.len(); i++) {
            total[i] = scalar % v[i];
        }
        return total;
    }

	template<int N, typename T, typename U>
	vec<N, T> operator%(vec<N, T> const& v1, vec<N, U> const& v2) {
        vec<N, T> total;
        for (int i = 0; i < v1.len(); i++) {
            total[i] = v1[i] % v2[i];
        }
        return total;
    }


	template<int N, typename T, typename U>
	vec<N, T> operator^(vec<N, T> const& v, U scalar) {
        vec<N, T> total;
        for (int i = 0; i < v.len(); i++) {
            total[i] = v[i] ^ scalar;
        }
        return total;
    }

	template<int N, typename T, typename U>
	vec<N, T> operator^(U scalar, vec<N, T> const& v) {
        vec<N, T> total;
        for (int i = 0; i < v.len(); i++) {
            total[i] = scalar ^ v[i];
        }
        return total;
    }
    
	template<int N, typename T, typename U>
	vec<N, T> operator^(vec<N, T> const& v1, vec<N, U> const& v2) {
        vec<N, T> total;
        for (int i = 0; i < v1.len(); i++) {
            total[i] = v1[i] ^ v2[i];
        }
        return total;
    }


    // Equivalence Comparators
    template<int N, typename T, int M, typename U>
	bool operator==(vec<N, T> const& v1, vec<M, U> const& v2) {
        if (v1.len() != v2.len()) {
            return false;
        }
        for (int i = 0; i < v1.len(); i++) {
            if (v1[i] != v2[i]) {
                return false;
            }
        }
        return true;
    }

	template<int N, typename T, int M, typename U>
	bool operator!=(vec<N, T> const& v1, vec<M, U> const& v2) {
        if (v1 == v2) {
            return false;
        } else {
            return true;
        }
    }

    /// ---------------------------------------------------- ///

    template<int N, int M, typename T>
	struct mat {	
		typedef mat<N, M, T> type;
		typedef mat<M, N, T> transpose_type;
		typedef T value_type;
        typedef vec<N, T> col_type;
		typedef vec<M, T> row_type;

	private:
		col_type data[M];

	public:

		static length_t length() { return M; }
        static length_t size() { return M; }

        // ---  Nullary Functions --- //

        length_t len() const { return M; }
        length_t width() const { return M; }
        length_t height() const { return N; }
        length_t columns() const { return M; }
        length_t rows() const { return N; }
        
		col_type & operator[](int i) {
            if (i > this->width() || i < 0) {
                throw std::out_of_range("index out of bounds");
            }
            return this->data[i];
        }
		col_type const& operator[](int i) const {
            if (i > this->width() || i < 0) {
                throw std::out_of_range("index out of bounds");
            }
            return this->data[i];
        }

		// -- Constructors --

        // Identity Matrix
		mat() {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i)[i] = 1;
            }
        }

        mat(T scalar) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) = col_type(scalar);
            }
        }
        
        mat(const mat<N,M,T>& m) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) = m[i];
            }
        }


        mat(const T arr[N]) {
            for (int i = 0; i < this->length(); i++) {
                this->operator[](i) = arr[N];
            }
        }

        mat(std::initializer_list<col_type> arr) {
            assert(arr.size() <= N);
            std::copy(arr.begin(), arr.end(), this->data);
        }

        mat(std::initializer_list<std::initializer_list<T>> arg) {
            M_Assert((int)arg.size() == this->height(), "ERROR: Invalid input (width)");
            int i = 0;
            int j = 0;
            for (std::initializer_list<T> list : arg) {
                M_Assert((int)list.size() == this->width(), "ERROR: Invalid input (height)");
                i = 0;
                for (T t : list) {
                    this->operator[](i)[j] = t;
                    i += 1;
                }
                j += 1;
            }
        }


		// template<int I, int J, typename U>
		// template<typename std::enable_if<(N <= I && J <= J), bool> = true>
		// mat(mat<I, J, U> const& m);

		mat<N, M, T> & operator=(mat<N, M, T> const& m)  {
            for (int i = 0; i < this->width(); i++) {
                this->data[i] = m[i];
            }
            return *this;
        }

		template<typename U>
		mat<N, M, T> & operator+=(U scalar)  {
            for (int i = 0; i < this->width(); i++) {
                this->data[i] += scalar;
            }
            return *this;
        }
		template<typename U>
		mat<N, M, T> & operator+=(mat<N, M, U> const& m)  {
            for (int i = 0; i < this->width(); i++) {
                this->data[i] += m[i];
            }
            return *this;
        }

		template<typename U>
		mat<N, M, T> & operator-=(U scalar)  {
            for (int i = 0; i < this->width(); i++) {
                this->data[i] -= scalar;
            }
            return *this;
        }
		template<typename U>
		mat<N, M, T> & operator-=(mat<N, M, U> const& m) {
            for (int i = 0; i < this->width(); i++) {
                this->data[i] -= m[i];
            }
            return *this;
        }

		template<typename U>
		mat<N, M, T> & operator*=(U scalar) {
            for (int i = 0; i < this->width(); i++) {
                this->data[i] *= scalar;
            }
            return *this;
        }
		template<typename U>
		mat<N, M, T> & operator*=(mat<N, M, U> const& m) {
            float f = 0.0f;
            for (int i = 0; i < this->width(); i++) {
                for (int j = 0; j < this->height(); j++) {
                    f = 0.0f;
                    for (int k = 0; k < this->width(); k++) {
                        f += this->operator[  ](k)[j] * m[i][k];
                    }
                    this->operator[](j)[i] = f;
                } 
            }
            return *this;
        }

		template<typename U>
		mat<N, M, T> & operator/=(U scalar) {
            for (int i = 0; i < this->width(); i++) {
                this->operator[](i) /= scalar;
            }
            return *this;
        }
		template<typename U>
		mat<N, M, T> & operator/=(mat<N, M, U> const& m) {
            return *this *= inverse(m);
        }

	
		mat<N, M, T> & operator++() {
            for (int i = 0; i < this->width(); i++) {
                ++this->data[i];
            }
            return *this;
        }
		mat<N, M, T> & operator--()
         {
            for (int i = 0; i < this->width(); i++) {
                --this->data[i];
            }
            return *this;
        }

		mat<N, M, T> operator++(int) {
            for (int i = 0; i < this->width(); i++) {
                this->data[i]++;
            }
            return *this;
        }
		mat<N, M, T> operator--(int) {
            for (int i = 0; i < this->width(); i++) {
                this->data[i]--;
            }
            return *this;
        }

	};

    // --- Unary Operator --- ///

    template<int N, int M, typename T>
    mat<N, M, T> transpose(mat<M, N, T> const& m) {
        mat<N, M, T> inv;
        for (int i = 0; i < m.width(); i++) {
            for (int j = 0; j < m.height(); j++) {
                inv[j][i] = m[i][j];
            }
        }
        return inv;
    }

    template<int N, typename T>
    mat<N-1, N-1, T> sub_matrix(mat<N, N, T> a, int i, int j) {
        if (i < 0 || i >= N || j < 0 || j >= N) {
            return mat<N-1, N-1, T>();
        }
        int row = 0;
        int col = 0;
        mat<N-1, N-1, T> m = mat<N-1, N-1, T>();
        for (int q = 0; q < N; q++) {
            if (q != j) {
                for (int p = 0; p < N; p++) {
                    if (p != i) {
                        m[row][col] = a[q][p];
                        col++;
                    }
                } 
                row++;
                col = 0;
            }
        }
        return m;
    }

    // https://www.epsilonify.com/mathematics/linear-algebra/find-the-inverse-of-nxn-matrix-by-using-minors-cofactors-and-adjugate/
    // https://www.epsilonify.com/mathematics/linear-algebra/how-to-find-the-minors-of-a-nxn-matrix/

    template <typename T>
    double determinant(mat<1,1,T> a) {
        return a[0][0];
    }

    template <typename T>
    double determinant(mat<2,2,T> a) {
        return a[0][0] * a[1][1] - a[1][0] * a[0][1];
    }

    template<int N, typename T>
    double determinant(mat<N, N, T> m) {
        static_assert(N >= 3, "Matrix size must 3+");
        double det = 0.0;
        int sign = 1;
        for (int k = 0; k < N; k++) {
            det += sign * m[0][k] * determinant(sub_matrix(m, k, 0));
            sign = -sign;
        }
        return det;
    }

    template<int N, typename T>
    mat<N, N, T> minors(mat<N, N, T> m) {
        mat<N, N, T> n = mat<N, N, T>();
        for (int j = 0; j < N; j++) {
            for (int i = 0; i < N; i++) {
                n[j][i] = determinant(sub_matrix(m, i, j));
            }
        }
        return n;
    }

    template<int N, typename T>
    mat<N, N, T> cofactors(mat<N, N, T> m) {
        mat<N, N, T> min = minors(m);
        // std::cout << "minors" << std::endl;
        // print(min);
        mat<N, N, T> n = mat<N, N, T>();
        for (int j = 0; j < N; j++){
            for (int i = 0; i < N; i++) {
                n[j][i] = ((i + j) % 2 == 0 ? 1 : -1) * min[j][i];
            }
        }
        return n;
    }

    template<int N, typename T>
    mat<N, N, T> inverse(mat<N, N, T> m) {
        // mat<N, N, T> adj = adjucate(m);
        mat<N, N, T> cof = cofactors(m);
        mat<N, N, T> tran = transpose(cof);
        // std::cout << "adj" << std::endl;
        // print(adj);
        double det = determinant(m);
        // std::cout << "det = " << det << std::endl;
        return  (1 / det) * tran;  
    }
	
	template<int N, int M, typename T, typename U>
	mat<N, M, T> operator+(mat<N, M, T> const& m, U scalar) {
        mat<N, M, T> total;
        for (int i = 0; i < m.width(); i++) {
            total[i] = m[i] + scalar;
        }
        return total;
    }
	template<int N, int M, typename T, typename U>
	mat<N, M, T> operator+(U scalar, mat<N, M, T> const& m) {
        mat<N, M, T> total;
        for (int i = 0; i < m.width(); i++) {
            total[i] = scalar + m[i];
        }
        return total;
    }
	template<int N, int M, typename T, typename U>
	mat<N, M, T> operator+(mat<N, M, T> const& m1, mat<N, M, U> const& m2) {
        mat<N, M, T> total;
        for (int i = 0; i < m1.width(); i++) {
            total[i] = m1[i] + m2[i];
        }
        return total;
    }

	template<int N, int M, typename T, typename U>
	mat<N, M, T> operator-(mat<N, M, T> const& m, U scalar) {
        mat<N, M, T> total;
        for (int i = 0; i < m.width(); i++) {
            total[i] = m[i] - scalar;
        }
        return total;
    }
	template<int N, int M, typename T, typename U>
	mat<N, M, T> operator-(U scalar, mat<N, M, T> const& m) {
        mat<N, M, T> total;
        for (int i = 0; i < m.width(); i++) {
            total[i] = scalar - m[i];
        }
        return total;
    }
	template<int N, int M, typename T, typename U>
	mat<N, M, T> operator-(mat<N, M, T> const& m1, mat<N, M, U> const& m2) {
        mat<N, M, T> total;
        for (int i = 0; i < m1.width(); i++) {
            total[i] = m1[i] -  m2[i];
        }
        return total;
    }

	template<int N, int M, typename T, typename U>
	mat<N, M, T> operator*(mat<N, M, T> const& m, U scalar) {
        mat<N, M, T> total;
        for (int i = 0; i < m.width(); i++) {
            total[i] = m[i] * scalar;
        }
        return total;
    }

	template<int N, int M, typename T, typename U>
	mat<N, M, T> operator*(U scalar, mat<N, M, T> const& m) {
        mat<N, M, T> total;
        for (int i = 0; i < m.width(); i++) {
            total[i] = scalar * m[i];
        }
        return total;
    }

	template<int N, int M, typename T, typename U>
	typename mat<N, M, T>::col_type operator*(mat<N, M, T> const& m, typename mat<N, M, U>::col_type const& v) {
        typename mat<N, M, T>::col_type total;
        for (int i = 0; i < total.length(); i++) {
            T t = 0;
            for (int k = 0; k < total.length(); k++) {
                t += v[k] * m[k][i];
            }
            total[i] = t;
        }
        return total;
    }

	template<int N, int M, typename T, typename U>
	typename mat<N, M, T>::row_type operator*(typename mat<N, M, U>::row_type const& v, mat<N, M, T> const& m) {
        typename mat<N, M, T>::row_type total = mat<N, M, T>::row_type();
        for (int i = 0; i < total.length(); i++) {
            T t = 0;
            for (int k = 0; k < total.length(); k++) {
                t +=  m[i][k] * v[k];
            }
            total[i] = t;
        }
        return total;
    }

	template<int N, int M, int O, typename T, typename U>
	mat<N, O, T> operator*(mat<N, M, T> const& m1, mat<M, O, U> const& m2) {
        mat<N, O, T> total;
        for (int i = 0; i < m2.width(); i++) {
            for (int j = 0; j < m1.height(); j++) {
                T t = 0;
                for (int k = 0; k < m1.width(); k++) {
                    t += m1[k][j] * m2[i][k];
                }
                total[i][j] = t;
            }
        }
        return total;
    }

	template<int N, int M, typename T, typename U>
	mat<N, M, T> operator/(mat<N, M, T> const& m, U scalar) {
        mat<N, M, T> total;
        for (int i = 0; i < m.width(); i++) {
            total[i] = m[i] / scalar;
        }
        return total;
    }

	template<int N, int M, typename T, typename U>
	mat<N, M, T> operator/(U scalar, mat<N, M, T> const& m) {
        mat<N, M, T> total;
        for (int i = 0; i < m.width(); i++) {
            total[i] = scalar / m[i];
        }
        return total;
    }

	template<int N, int M, typename T, typename U>
	typename mat<N, M, T>::col_type operator/(mat<N, M, T> const& m, typename mat<N, M, U>::col_type const& v) {
        mat<M, M, T> inv = inverse(m);
        if (m == inv) {
            throw "ERROR: cannot divide using non-singular matrix";
            return v;
        }
        typename mat<N, M, T>::col_type total = mat<N, M, T>::col_type();
        for (int i = 0; i < total.length(); i++) {
            T t = 0;
            for (int k = 0; k < total.length(); k++) {
                t += v[k] / inv[i][k];
            }
            total[i] = t;
        }
        return total;
    }

	template<int N, int M, typename T, typename U>
	typename mat<N, M, T>::row_type operator/(typename mat<N, M, T>::row_type const& v, mat<N, M, U> const& m) {
        mat<M, M, T> inv = inverse(m);
        if (m == inv) {
            throw "ERROR: cannot divide using non-singular matrix";
            return v;
        }
        typename mat<N, M, T>::row_type total = mat<N, M, T>::row_type();
        for (int i = 0; i < total.length(); i++) {
            T t = 0;
            for (int k = 0; k < total.length(); k++) {
                t +=  inv[k][i] / v[k];
            }
            total[i] = t;
        }
        return total;
    }

	template<int N, int M, typename T, typename U>
	mat<N, M, T> operator/(mat<N, M, T> const& m1, mat<M, M, U> const& m2) {
        mat<M, M, T> inv = inverse(m2);
        if (m2 == inv) {
            throw "ERROR: cannot divide using non-singular matrix";
            return m1;
        }
        return m1 * inv;
    }

	// -- Boolean operators --

	template<int N, int M, typename T, typename U>
	bool operator==(mat<N, M, T> const& m1, mat<N, M, U> const& m2) {
        for (int i = 0; i < m1.width(); i++) {
            if (m1[i] != m2[i]) {
                return false;
            }
        }
        return true;
    }

	template<int N, int M, typename T, typename U>
	bool operator!=(mat<N, M, T> const& m1, mat<N, M, U> const& m2) {
        if (m1 == m2) {
            return false;
        } else {
            return true;
        }
    }

    // print
    template<int N, typename T>
    std::ostream& operator<<(std::ostream& os, vec<N, T>& obj) {
        for (int i = 0; i < obj.length() - 1; i++) {
            os << obj[i] << ", ";
        }
        return os << obj[obj.length() - 1] << std::endl;
    }

    template<int N, int M, typename T>
    std::ostream& operator<<(std::ostream& os, mat<N, M, T>& obj) {
        for (int i = 0; i < obj.length(); i++) {
            os << obj[i];
        }
        return os << std::endl;
    }

    template<int N, typename T>
    void print(vec<N, T> const& v) {
        for (int i = 0; i < v.length(); i++) {
            std::cout << v[i] << " ";
        }
        std::cout << std::endl;
    }

    template<int N, int M, typename T>
    void print(mat<N, M, T> const& m) {
        for (int j = 0; j < m.height(); j++) {
            print(m[j]);
        }
    }


    using vec2 = vec<2, float>;
    using vec3 = vec<3, float>;
    using vec4 = vec<4, float>;
    using mat2 = mat<2, 2, float>;
    using mat3 = mat<3, 3, float>;
    using mat4 = mat<4, 4, float>;
    template<int N>
    using vecf = vec<N, float>;
    template<int N>
    using vecd = vec<N, double>;
    template<int N, int M>
    using matf = mat<N, M, float>;
    template<int N, int M>
    using matd = mat<N, M, double>;
    
}