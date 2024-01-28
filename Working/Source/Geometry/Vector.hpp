#ifndef VECTOR_H
#define VECTOR_H

#include <utility>
#include <stdexcept>
#include <cmath>

template <typename T>
class Vec2 {
    T data[2];

public:
    Vec2(T x, T y) : data{x, y} {};
    Vec2() = default;

    const T& operator[](int index) const { return data[index]; }
    T& operator[](int index) { return data[index]; }

    const T& x() const { return data[0]; }
    T& x() { return data[0]; }

    const T& y() const { return data[1]; }
    T& y() { return data[1]; }

    inline T& get(int i) {
        if (i < 0 || i > 1) {
            throw std::out_of_range("Index out of range");
        }
        return data[i];
    }

    Vec2<T> operator+(const Vec2<T>& v) const {
        return Vec2<T>(data[0] + v[0], data[1] + v[1]);
    }

    Vec2<T> operator-(const Vec2<T>& v) const {
        return Vec2<T>(data[0] - v[0], data[1] - v[1]);
    }

    inline T dot(const Vec2<T>& v) const {
        return data[0] * v[0] + data[1] * v[1];
    }

    inline T crossMagnitude(const Vec2<T>& v) const {
        return data[0] * v[1] - data[1] * v[0];
    }

    inline T magnitude() const {
        return std::sqrt(data[0] * data[0] + data[1] * data[1]);
    }
};

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;

template <typename T>
class Vec3 {
    T data[3];
public:

    Vec3(T x, T y, T z) : data{x, y, z} {};
    Vec3() = default;

    const T& operator[](int index) const { return data[index]; }
    T& operator[](int index) { return data[index]; }

    const T& x() const { return data[0]; }
    T& x() { return data[0]; }

    const T& y() const { return data[1]; }
    T& y() { return data[1]; }

    const T& z() const { return data[2]; }
    T& z() { return data[2]; }

    T& get(int i) {
        if (i < 0 || i > 2) {
            throw std::out_of_range("Index out of range");
        }
        return data[i];
    }

    Vec3<T> operator+(const Vec3<T>& v) const {
        return Vec3<T>(data[0] + v[0], data[1] + v[1], data[2] + v[2]);
    }

    Vec3<T> operator-(const Vec3<T>& v) const {
        return Vec3<T>(data[0] - v[0], data[1] - v[1], data[2] - v[2]);
    }

    inline T dot(const Vec3<T>& v) const {
        return data[0] * v[0] + data[1] * v[1] + data[2] * v[2];
    };

    Vec3<T> cross(const Vec3<T>& v) const {
        return Vec3<T>(
            data[1] * v[2] - data[2] * v[1],
            data[2] * v[0] - data[0] * v[2],
            data[0] * v[1] - data[1] * v[0]
        );
    }

    inline T magnitude() const {
        return std::sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
    }

};

typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;



#endif // VECTOR_H
