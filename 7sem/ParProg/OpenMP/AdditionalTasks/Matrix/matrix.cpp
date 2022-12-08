#pragma once

#include <cstdio>
#include <cstdlib>
#include <omp.h>


class Matrix
{
  private:
    int** data = nullptr;
    int size = 0;

    int LongestNumber();
    int FieldLength();

  public:
    Matrix(int size);
    ~Matrix();

    void FillRandom();
    void Print();

    bool operator==(Matrix& other);
    bool operator!=(Matrix& other);

    Matrix* LinearMultiply(Matrix* other);    // default matrix multiplier
    Matrix* ParallelMultiply(Matrix* other);  // default, but parallel multiplier

    Matrix* FastMultiplyLinear(Matrix* other);
    Matrix* FastMultiply(Matrix* other);

    Matrix* SimdMultiply(Matrix* other);
};

Matrix::Matrix(int size)
{
    this -> size = size;
    this -> data = new int*[this -> size];

    for (int row = 0; row < this -> size; row++)
    {
        (this -> data)[row] = new int[this -> size];
        for (int col = 0; col < this -> size; col++)
            (this -> data)[row][col] = 0;
    }
}

Matrix::~Matrix()
{
    if (this -> data != nullptr)
    {
        for (int i = 0; i < size; i++)
        {
            if ((this -> data)[i] != nullptr)
                delete[] (this -> data)[i];
        }

        delete[] this -> data;
    }
}

void Matrix::FillRandom()
{
    for (int row = 0; row < this -> size; row++)
    {
        for (int col = 0; col < this -> size; col++)
        {
            (this -> data)[row][col] = rand() % 19 - 10;
        }
    }
}

void Matrix::Print()
{
    for (int row = 0; row < this -> size; row++)
    {
        printf("│ ");
        for (int col = 0; col < this -> size; col++)
        {
            printf("%5d ", (this -> data)[row][col]);
        }
        printf("│\n");
    }

    printf("\n");
}

Matrix* Matrix::LinearMultiply(Matrix* other)
{
    Matrix* new_matrix = new Matrix(this -> size);

    for (int row = 0; row < new_matrix -> size; row++)
    {
        for (int col = 0; col < new_matrix -> size; col++)
        {
            for (int term_num = 0; term_num < this -> size; term_num++)
                (new_matrix -> data)[row][col] += (this -> data)[row][term_num] * (other -> data)[term_num][col];
        }
    }

    return new_matrix;
}

Matrix* Matrix::FastMultiplyLinear(Matrix* other)
{
    Matrix* new_matrix = new Matrix(this -> size);
    
    int* rowFactor = new int[this -> size];
    int* columnFactor = new int[this -> size];

    int d = (this -> size) / 2;

    for (int row = 0; row < this -> size; row++)
    {
        rowFactor[row] = (this -> data)[row][0] * (this -> data)[row][1];
        for (int col = 1; col < d; col++)
            rowFactor[row] += (this -> data)[row][2 * col] * (this -> data)[row][2 * col + 1];
    } 

    for (int col = 0; col < this -> size; col++)
    {
        columnFactor[col] = (other -> data)[0][col] * (other -> data)[1][col];
        for (int row = 1; row < d; row++)
            columnFactor[col] += (other -> data)[2 * row][col] * (other -> data)[2 * row + 1][col];
    }

    for (int row = 0; row < new_matrix -> size; row++)
    {
        for (int col = 0; col < new_matrix -> size; col++)
        {
            (new_matrix -> data)[row][col] = -rowFactor[row] - columnFactor[col];

            for (int k = 0; k < d; k++)
            {
                (new_matrix -> data)[row][col] += ((this -> data)[row][2 * k] + (other -> data)[2 * k + 1][col]) * 
                    ((this -> data)[row][2 * k + 1] + (other -> data)[2 * k][col]);
            }
        }
    }

    if (2 * d != new_matrix -> size)
    {
        for (int row = 0; row < this -> size; row++)
        {
            for (int col = 0; col < other -> size; col++)
                (new_matrix -> data)[row][col] += (this -> data)[row][(this -> size) - 1] * 
                    (other -> data)[(other -> size) - 1][col];
        }
    }

    return new_matrix;
}

Matrix* Matrix::ParallelMultiply(Matrix* other)
{
    Matrix* new_matrix = new Matrix(this -> size);

    #pragma omp parallel for schedule(guided, 1)
    for (int row = 0; row < new_matrix -> size; row++)
    {
        #pragma omp parallel for schedule(guided, 1)
        for (int col = 0; col < new_matrix -> size; col++)
        {
            for (int term_num = 0; term_num < this -> size; term_num++)
                (new_matrix -> data)[row][col] += (this -> data)[row][term_num] * (other -> data)[term_num][col];
        }
    }

    return new_matrix;
}

Matrix* Matrix::FastMultiply(Matrix* other)
{
    Matrix* new_matrix = new Matrix(this -> size);

    int* rowFactor = new int[this -> size];
    int* columnFactor = new int[this -> size];

    int d = (this -> size) / 2;

    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp section
        {
            #pragma omp parallel for schedule(guided, 1) num_threads(4)
            for (int row = 0; row < this -> size; row++)
            {
                rowFactor[row] = (this -> data)[row][0] * (this -> data)[row][1];
                for (int col = 1; col < d; col++)
                    rowFactor[row] += (this -> data)[row][2 * col] * (this -> data)[row][2 * col + 1];
            } 
        }

        #pragma omp section
        {
            #pragma omp parallel for schedule(guided, 1) num_threads(4)
            for (int col = 0; col < this -> size; col++)
            {
                columnFactor[col] = (other -> data)[0][col] * (other -> data)[1][col];
                for (int row = 1; row < d; row++)
                    columnFactor[col] += (other -> data)[2 * row][col] * (other -> data)[2 * row + 1][col];
            }
        }
    }

    #pragma omp parallel for schedule(guided, 1)
    for (int row = 0; row < new_matrix -> size; row++)
    {
        #pragma omp parallel for schedule(guided, 1)
        for (int col = 0; col < new_matrix -> size; col++)
        {
            (new_matrix -> data)[row][col] = -rowFactor[row] - columnFactor[col];

            for (int k = 0; k < d; k++)
            {
                (new_matrix -> data)[row][col] += ((this -> data)[row][2 * k] + (other -> data)[2 * k + 1][col]) * 
                    ((this -> data)[row][2 * k + 1] + (other -> data)[2 * k][col]);
            }
        }
    }

    if (2 * d != new_matrix -> size)
    {
        #pragma omp parallel for schedule(guided, 1)
        for (int row = 0; row < this -> size; row++)
        {
            for (int col = 0; col < other -> size; col++)
                (new_matrix -> data)[row][col] += (this -> data)[row][(this -> size) - 1] * 
                    (other -> data)[(other -> size) - 1][col];
        }
    }

    delete[] rowFactor;
    delete[] columnFactor;

    return new_matrix;
}

Matrix* Matrix::SimdMultiply(Matrix* other)
{
    Matrix* new_matrix = new Matrix(this -> size);

    #pragma omp parallel for
    for (int row = 0; row < new_matrix -> size; row++)
    {
        #pragma omp parallel for
        for (int col = 0; col < new_matrix -> size; col++)
        {
            #pragma omp simd
            for (int term_num = 0; term_num < this -> size; term_num++)
                new_matrix -> data[row][col] += (this -> data)[row][term_num] * (other -> data)[term_num][col];
        }
    }

    return new_matrix;
}

bool Matrix::operator==(Matrix& other)
{
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            if (data[row][col] != other.data[row][col])
                return false;
        }
    }

    return true;
}

bool Matrix::operator!=(Matrix& other)
{
    return not (*this == other);
}
