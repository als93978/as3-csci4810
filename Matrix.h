#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

template<typename T>
class Matrix {
public:
    Matrix(size_t rows, size_t cols) {
        this->rows = rows;
        this->cols = cols;
        data.reserve(rows * cols);
    }
    
    T& operator()(size_t i, size_t j) {
        return data[i * cols + j];
    }
    
    T operator()(size_t i, size_t j) const {
        return data[i * cols + j];
    }
    
    Matrix operator*(Matrix &m) const {
        Matrix<T> result(rows, m.getCols());

        // if time allows, error checking (e.g., invalid sizes)
    
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < m.getCols(); j++) {
                T currentSum = 0;
            
                for(int k = 0; k < m.getRows(); k++) {
                    currentSum += (*this)(i, k) * m(k, j);
                }

                result(i, j) = currentSum;
            }
        }

        return result;
    }
    
    size_t getRows() {
        return rows;
    }
    
    size_t getCols() {
        return cols;
    }

private:
    size_t rows;
    size_t cols;
    std::vector<T> data;
};

#endif
