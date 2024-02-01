#ifndef _MEADOW_COLLECTION_HPP_
#define _MEADOW_COLLECTION_HPP_

template <typename T>
class collection {

public:
    const int size;
    T* data;

    collection(int size) : size(size), data(new T[size]) {}

    collection(const collection& other) : size(other.size), data(new T[size]) {
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    collection(collection& other) : size(other.size), data(other.data) {
        other.size = 0;
        other.data = nullptr;
    }

    ~collection() {
        delete[] data;
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }
    

};

#endif