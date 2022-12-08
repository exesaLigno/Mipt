#include <cstdio>
#include <omp.h>

int NESTING_LEVEL = 3;

void print_thread_info()
{
    if (omp_get_num_threads() > 1)
    {
        int nesting = omp_get_level();
        const char* indent = nesting == 1 ? "" : nesting == 2 ? "\t" : "\t\t";

        char nesting_path[50] = {0};

        int line_indent = 0;
        for (int level = 0; level < nesting; level++)
        {
            line_indent += sprintf(nesting_path + line_indent, "%d/%d -> ", 
                              omp_get_ancestor_thread_num(level), omp_get_team_size(level));
        }

        printf("%s %d I am thread %d/%d (%sme)\n", 
                indent, nesting, omp_get_thread_num(), omp_get_num_threads(), nesting_path);
    }
}

int main(int argc, char** argv)
{
    printf("Allowed %d levels of parallelizm\n", omp_get_supported_active_levels());

    omp_set_nested(true);

    #pragma omp parallel num_threads(4) if(NESTING_LEVEL >= 1)
    {
        print_thread_info();

        #pragma omp parallel num_threads(3) if(NESTING_LEVEL >= 2)
        {
            print_thread_info();

            #pragma omp parallel num_threads(2) if(NESTING_LEVEL >= 3)
            {
                print_thread_info();
            }
        }
    }

    return 0;
}
