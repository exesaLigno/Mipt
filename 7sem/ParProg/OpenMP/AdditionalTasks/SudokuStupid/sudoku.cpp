#pragma once
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <omp.h>
#include <time.h>

long int clk()
{
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    long int time_ms = time.tv_sec * 1000000 + time.tv_nsec / 1000;
    return time_ms;
}

int FindNextEmptyCell(int** field, int field_size, int start_cell_num)
{
    int cell_num = start_cell_num + 1;
    while (cell_num < field_size * field_size and field[cell_num / field_size][cell_num % field_size] != 0)
        cell_num++;
    return cell_num < field_size * field_size ? cell_num : -1;
}

int FindFirstEmptyCell(int** field, int field_size)
{
    return FindNextEmptyCell(field, field_size, -1);
}

int** CopyField(int** field, int field_size)
{
    int** field_copy = new int*[field_size];
    for (int row = 0; row < field_size; row++)
    {
        field_copy[row] = new int[field_size];
        for (int col = 0; col < field_size; col++)
            field_copy[row][col] = field[row][col];
    }

    return field_copy;
}

bool IsCorrect(int** field, int field_size, int current_cell_num)
{
    int row = current_cell_num / field_size;
    int col = current_cell_num % field_size;

    for (int second_row = 0; second_row < field_size; second_row++)
    {
        if (second_row != row and field[row][col] == field[second_row][col])
            return false;
    }
    for (int second_col = 0; second_col < field_size; second_col++)
    {
        if (second_col != col and field[row][col] == field[row][second_col])
            return false;
    }

    int block_size = sqrt(field_size);

    int block_row = row / block_size;
    int block_col = col / block_size;

    for (int second_row = block_row * block_size; second_row < (block_row + 1) * block_size; second_row++)
    {
        for (int second_col = block_col * block_size; second_col < (block_col + 1) * block_size; second_col++)
        {
            if (second_row != row and second_col != col and field[row][col] == field[second_row][second_col])
                return false;
        }
    }

    return true; // Add function to validate substitution
}

bool SolveSudoku(int** field, int field_size, int current_cell_num)
{
    int row = current_cell_num / field_size;
    int col = current_cell_num % field_size;

    for (int variant = 1; variant <= field_size; variant++)
    {
        field[row][col] = variant;
        if (IsCorrect(field, field_size, current_cell_num))
        {
            int next_cell_num = FindNextEmptyCell(field, field_size, current_cell_num);

            if (next_cell_num == -1 or SolveSudoku(field, field_size, next_cell_num))
                return true; // if nested function found solution or field filled, returning true and keeping solution
        }
    }

    field[row][col] = 0;
    return false;
}

long SolveSudoku(int** field, int field_size, int*** solution, bool* single_solution, bool parallel_run)
{
    int current_cell_num = FindFirstEmptyCell(field, field_size);
    int row = current_cell_num / field_size;
    int col = current_cell_num % field_size;

    int** filled_field = nullptr;
    bool multiple_solutions = false;

    long start_time = clk();

    #pragma omp parallel for if(parallel_run) schedule(dynamic, 1)
    for (int variant = 1; variant <= field_size; variant++)
    {
        int** local_field = CopyField(field, field_size); // Creating field copy for thread

        local_field[row][col] = variant;
        if (IsCorrect(local_field, field_size, current_cell_num))
        {
            int next_cell_num = FindNextEmptyCell(local_field, field_size, current_cell_num);

            if (next_cell_num == -1 or SolveSudoku(local_field, field_size, next_cell_num))
            {
                #pragma omp critical
                {
                    if (filled_field != nullptr) multiple_solutions = true;
                    filled_field = local_field; // if nested function found solution or field filled, returning true and keeping solution
                }
            }
        }
    }

    *solution = filled_field;
    *single_solution = !multiple_solutions;

    long stop_time = clk();
    return stop_time - start_time;
}


void ShowFieldSimple(int** field, int field_size)
{
    for (int row = 0; row < field_size; row++)
    {
        for (int column = 0; column < field_size; column++)
        {
            int cell_value = field[row][column];
            if (cell_value == 0)
                printf("   ");
            else
                printf("%2d ", cell_value);
        }
        printf("\n");
    }
}

void ShowField(int** field, int field_size)
{
    bool wide_field = field_size / 10 >= 1;

    if (wide_field) // Need to rewrite showing for wide fields
    {
        ShowFieldSimple(field, field_size);
        return;
    }

    printf("╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n");

    for (int row = 0; row < field_size; row++)
    {
        printf("║");
        for (int column = 0; column < field_size; column++)
        {
            int cell_value = field[row][column];
            if (cell_value == 0)
                printf("   ");
            else 
                printf(" %d ", cell_value);

            printf("%s", (column + 1) % 3 == 0 ? "║" : "│");
        }
        printf("\n");

        if (row != 8 and (row + 1) % 3 == 0)
            printf("╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n");
        else if (row != 8)
            printf("╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n");
        else
            printf("╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n");
    }
}

void CreateField(int*** field, int* field_size)
{
    *field = new int*[*field_size];
    for (int i = 0; i < *field_size; i++)
    {
        (*field)[i] = new int[*field_size];
    }

}

void SetDebugTask(int*** field, int* field_size)
{
    *field_size = 9;
    CreateField(field, field_size);

    (*field)[0][2] = 7;
    (*field)[0][5] = 3;
    (*field)[0][6] = 4;
    (*field)[0][7] = 9;
    (*field)[0][8] = 6;
    
    (*field)[1][0] = 9;
    (*field)[1][1] = 3;
    (*field)[1][4] = 5;
    (*field)[1][7] = 1;
    (*field)[1][8] = 8;

    (*field)[2][0] = 8;
    (*field)[2][1] = 2;
    (*field)[2][5] = 9;
    (*field)[2][6] = 7;

    (*field)[3][2] = 5;
    (*field)[3][3] = 3;
    (*field)[3][4] = 8;
    (*field)[3][5] = 7;
    (*field)[3][8] = 2;

    (*field)[4][0] = 6;
    (*field)[4][4] = 9;

    (*field)[5][1] = 9;
    (*field)[5][3] = 5;
    (*field)[5][4] = 1;
    (*field)[5][5] = 6;

    (*field)[6][0] = 4;
    (*field)[6][1] = 5;
    (*field)[6][2] = 9;
    (*field)[6][5] = 1;
    (*field)[6][8] = 7;

    (*field)[7][0] = 3;
    (*field)[7][3] = 2;
    (*field)[7][4] = 7;

    (*field)[8][1] = 6;
    (*field)[8][2] = 2;
    (*field)[8][4] = 4;
    (*field)[8][7] = 3;
    (*field)[8][8] = 1;
}

void ReadTask(const char* filename, int*** field, int* field_size)
{
    if (filename == nullptr)
    {
        SetDebugTask(field, field_size);
        return;
    }

    FILE* task_file = fopen(filename, "r");
    fseek (task_file, 0, SEEK_END);
    long text_size = ftell (task_file);
    rewind (task_file);

    char* buffer = new char[text_size + 1];
    fread(buffer, sizeof(char), text_size, task_file);

    char* buffer_start = buffer;

    bool in_symbol = false;
    int cells_count = 0;

    while (*buffer != '\n')
    {
        if ((*buffer >= '0' and *buffer <= '9') or *buffer == '_')
        {
            if (not in_symbol)
            {
                cells_count++;
                in_symbol = true;
            }
        }

        else
            in_symbol = false;
        
        buffer++;
    }

    *field_size = cells_count;

    CreateField(field, field_size);

    buffer = buffer_start;
    in_symbol = false;
    bool in_number = false;
    int cell_number = 0;
    char number_buffer[3] = {0};
    int num_buf_counter = 0;

    while (*buffer != '\0')
    {
        if ((*buffer >= '0' and *buffer <= '9') or *buffer == '_')
        {
            if (not in_symbol)
            {
                cell_number++;
                in_symbol = true;
            }
        }

        else
        {
            in_symbol = false;
            in_number = false;

            if (num_buf_counter > 0)
            {
                number_buffer[num_buf_counter] = '\0';
                int row = (cell_number - 1) / *field_size;
                int col = (cell_number - 1) % *field_size;
                (*field)[row][col] = atoi(number_buffer);
            }

            num_buf_counter = 0;
        }

        if (*buffer >= '0' and *buffer <= '9')
        {
            number_buffer[num_buf_counter] = *buffer;
            num_buf_counter++;
        }
        
        buffer++;
    }
}

int EmptyCellsCount(int** field, int field_size)
{
    int count = 0;
    for (int row = 0; row < field_size; row++)
    {
        for (int col = 0; col < field_size; col++)
        {
            if (field[row][col] == 0) count++;
        }
    }
    return count;
}
