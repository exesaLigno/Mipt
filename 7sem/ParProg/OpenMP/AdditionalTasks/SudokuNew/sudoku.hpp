#include <cstdio>
#include <vector>
#include <omp.h>
#include <math.h>
#include <time.h>

#define DEBUG if(true)

long int clk();

class Cell
{
  private:
    long variants = 0;
    long mask = 0;
    int variantsCount = 0;

  public:
    Cell();
    ~Cell();

    void fillVariants(int variants_count);
    void setKnownValue(int knownValue);
    bool excludeVariant(int variant);
    bool excludeVariant(long variant);
    int getCellValue();
    long getCellValueMask();
};

class SudokuSolver
{
  private:
    int fieldSize = 0;
    int blockSize = 0;
    Cell** field = nullptr;
    bool fieldChanged = false;

    void setFieldSize(int size);
    void createField();

  public:
    SudokuSolver();
    ~SudokuSolver();

    void readTask(char* filename);
    void debugTask();
    void showField();
    void showFieldSimple();

    bool excludeVariants();

    long run();
};
