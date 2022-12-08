#include "sudoku.hpp"

long int clk()
{
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    long int time_ms = time.tv_sec * 1000000 + time.tv_nsec / 1000;
    return time_ms;
}

Cell::Cell() {}

Cell::~Cell() {}

void Cell::setKnownValue(int known_value)
{
    variants = 1 << (known_value - 1);
    variantsCount = 1;
}

void Cell::fillVariants(int variants_count)
{
    variants = (1 << variants_count) - 1;
    variantsCount = variants_count;
}

bool Cell::excludeVariant(long variant)
{
    bool changed = false;
    long new_variants = variants & ~variant;
    if (new_variants != variants)
    {
        variantsCount--;
        changed = true;
    }
    variants = new_variants;
    return changed;
}

bool Cell::excludeVariant(int variant)
{
    bool changed = false;
    long new_variants = variants & ~(1 << (variant - 1));
    if (new_variants != variants)
    {
        variantsCount--;
        changed = true;
    }
    variants = new_variants;
    return changed;
}

int Cell::getCellValue()
{
    if (variantsCount == 1)
    {
        long variants_copy = variants;
        int value = 1;
        while (variants_copy != 1)
        {
            variants_copy = variants_copy >> 1;
            value++;
        }

        return value;
    }

    return 0;
}

long Cell::getCellValueMask()
{
    if (variantsCount == 1)
        return variants;
    else
        return 0;
}



void SudokuSolver::setFieldSize(int size)
{
    fieldSize = size;
    blockSize = sqrt(size);
}

void SudokuSolver::createField()
{
    field = new Cell*[fieldSize];
    for (int row = 0; row < fieldSize; row++)
    {
        field[row] = new Cell[9];
        for (int column = 0; column < fieldSize; column++)
            field[row][column].fillVariants(fieldSize);
    }
}

void SudokuSolver::readTask(char* filename)
{
    if (filename == nullptr)
    {
        setFieldSize(9);
        createField();
        debugTask();
    }

    else
    {

    }
}

void SudokuSolver::debugTask()
{
    field[0][2].setKnownValue(7);
    field[0][5].setKnownValue(3);
    field[0][6].setKnownValue(4);
    field[0][7].setKnownValue(9);
    field[0][8].setKnownValue(6);
    
    field[1][0].setKnownValue(9);
    field[1][1].setKnownValue(3);
    field[1][4].setKnownValue(5);
    field[1][7].setKnownValue(1);
    field[1][8].setKnownValue(8);

    field[2][0].setKnownValue(8);
    field[2][1].setKnownValue(2);
    field[2][5].setKnownValue(9);
    field[2][6].setKnownValue(7);

    field[3][2].setKnownValue(5);
    field[3][3].setKnownValue(3);
    field[3][4].setKnownValue(8);
    field[3][5].setKnownValue(7);
    field[3][8].setKnownValue(2);

    field[4][0].setKnownValue(6);
    field[4][4].setKnownValue(9);

    field[5][1].setKnownValue(9);
    field[5][3].setKnownValue(5);
    field[5][4].setKnownValue(1);
    field[5][5].setKnownValue(6);

    field[6][0].setKnownValue(4);
    field[6][1].setKnownValue(5);
    field[6][2].setKnownValue(9);
    field[6][5].setKnownValue(1);
    field[6][8].setKnownValue(7);

    field[7][0].setKnownValue(3);
    field[7][3].setKnownValue(2);
    field[7][4].setKnownValue(7);

    field[8][1].setKnownValue(6);
    field[8][2].setKnownValue(2);
    field[8][4].setKnownValue(4);
    field[8][7].setKnownValue(3);
    field[8][8].setKnownValue(1);
}

void SudokuSolver::showFieldSimple()
{
    for (int row = 0; row < fieldSize; row++)
    {
        for (int column = 0; column < fieldSize; column++)
        {
            int cell_value = field[row][column].getCellValue();
            if (cell_value == 0)
                printf("  ");
            else
                printf("%d ", cell_value);
        }
        printf("\n");
    }
}

void SudokuSolver::showField()
{
    bool wide_field = fieldSize / 10 >= 1;

    if (wide_field) // Need to rewrite showing for wide fields
    {
        showFieldSimple();
        return;
    }

    printf("╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n");

    for (int row = 0; row < fieldSize; row++)
    {
        printf("║");
        for (int column = 0; column < fieldSize; column++)
        {
            int cell_value = field[row][column].getCellValue();
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

bool SudokuSolver::excludeVariants()
{
    bool something_changed = false;

    //#pragma omp parallel for schedule(dynamic, 1) reduction(||:something_changed)
    for (int row = 0; row < fieldSize; row++)
    {
        //printf("%d threads\n", omp_get_num_threads());
        //#pragma omp parallel for schedule(dynamic, 1) reduction(||:something_changed)
        for (int column = 0; column < fieldSize; column++)
        {
            for (int second_row = 0; second_row < fieldSize; second_row++)
            {
                if (second_row != row)
                    something_changed |= field[row][column].excludeVariant(field[second_row][column].getCellValueMask());
            }
            for (int second_column = 0; second_column < fieldSize; second_column++)
            {
                if (second_column != column)
                    something_changed |= field[row][column].excludeVariant(field[row][second_column].getCellValueMask());
            }

            int block_row = row / blockSize;
            int block_column = column / blockSize;

            for (int second_row = block_row * blockSize; second_row < (block_row + 1) * blockSize; second_row++)
            {
                for (int second_column = block_column * blockSize; second_column < (block_column + 1) * blockSize; second_column++)
                {
                    if (second_row != row and second_column != column)
                        something_changed |= field[row][column].excludeVariant(field[second_row][second_column].getCellValueMask());
                }
            }
        }
    }

    return something_changed;
}

long SudokuSolver::run()
{
    long start = clk();

    bool need_exclusion = true;
    int count = 0;

    while (need_exclusion)
    {
        need_exclusion = excludeVariants();
        count++;
    }

    long stop = clk();

    printf("%d repeats\n", count);

    return stop-start;
}


SudokuSolver::SudokuSolver() { }

SudokuSolver::~SudokuSolver()
{
    if (field != nullptr)
    {
        for (int i = 0; i < fieldSize; i++)
        {
            if (field[i] != nullptr)
            {
                delete[] field[i];
                field[i] = nullptr;
            }
        }
        field = nullptr;
    }
}
