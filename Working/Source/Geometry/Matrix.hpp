#ifndef MATRIX_HPP
#define MATRIX_HPP

// Matrix class declaration
template<int M, int N, typename T=double>
class Matrix {
    std::array<T, M * N> data;

public:
    inline T& get(int m, int n) {
        return data[m * N + n];
    }

    inline const T& get(int m, int n) const {
        return data[m * N + n];
    }

    Matrix<M, N, T> operator+(const Matrix<M, N, T>& m) const {
        Matrix<M, N, T> result;
        for (int i = 0; i < M * N; i++) {
            result.data[i] = data[i] + m.data[i];
        }
        return result;
    }

    Matrix<M, N, T>& operator+=(const Matrix<M, N, T>& m) {
        for (int i = 0; i < M * N; i++) {
            data[i] += m.data[i];
        }
        return *this;
    }

    Matrix<M, N, T> operator-(const Matrix<M, N, T>& m) const {
        Matrix<M, N, T> result;
        for (int i = 0; i < M * N; i++) {
            result.data[i] = data[i] - m.data[i];
        }
        return result;
    }

    Matrix<M, N, T>& operator-=(const Matrix<M, N, T>& m) {
        for (int i = 0; i < M * N; i++) {
            data[i] -= m.data[i];
        }
        return *this;
    }

    Matrix<M, N, T> operator*(const Matrix<M, N, T>& m) const {
        Matrix<M, N, T> result;
        for (int i = 0; i < N; i+=M) {
            for (int j = 0; j < M; j++) {
                result.data[i + j] = 0;
                for (int k = 0; k < N; k++) {
                    result.data[i + j] += data[i + k] * m.data[k + j];
                }
            }
        }
        return result;
    }
};

#endif // MATRIX_HPP