#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include <vector>
#include <unistd.h>
#include <time.h>

int task_random(int iter, int thread);
int task_stable(int iter, int thread);
int task_increasing_1(int iter, int thread);
int task_increasing_2(int iter, int thread);
void print_results(std::vector<int> results[], int threads_count);
void clear_results(std::vector<int> results[], int threads_count);

int main(int argc, char **argv)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);

    int iterations = 65;
    int threads = 4;

    int (*task)(int, int) = task_increasing_2;

    if (argc == 3)
    {
        iterations = atoi(argv[1]);
        threads = atoi(argv[2]);
    }

    std::vector<int> results[threads];

    clear_results(results, threads);
    printf("Running %d iterations on %d threads without schedule\n", iterations, threads);
    #pragma omp parallel for num_threads(threads)
    for (int i = 0; i < iterations; i++)
    {
        task(i, omp_get_thread_num());
        results[omp_get_thread_num()].push_back(i);
    }
    print_results(results, threads);

    clear_results(results, threads);
    printf("Running %d iterations on %d threads with schedule(static, 1)\n", iterations, threads);
    #pragma omp parallel for schedule(static, 1) num_threads(threads)
    for (int i = 0; i < iterations; i++)
    {
        task(i, omp_get_thread_num());
        results[omp_get_thread_num()].push_back(i);
    }
    print_results(results, threads);

    clear_results(results, threads);
    printf("Running %d iterations on %d threads with schedule(static, 4)\n", iterations, threads);
    #pragma omp parallel for schedule(static, 4) num_threads(threads)
    for (int i = 0; i < iterations; i++)
    {
        task(i, omp_get_thread_num());
        results[omp_get_thread_num()].push_back(i);
    }
    print_results(results, threads);

    clear_results(results, threads);
    printf("Running %d iterations on %d threads with schedule(dynamic, 1)\n", iterations, threads);
    #pragma omp parallel for schedule(dynamic, 1) num_threads(threads)
    for (int i = 0; i < iterations; i++)
    {
        task(i, omp_get_thread_num());
        results[omp_get_thread_num()].push_back(i);
    }
    print_results(results, threads);

    clear_results(results, threads);
    printf("Running %d iterations on %d threads with schedule(dynamic, 4)\n", iterations, threads);
    #pragma omp parallel for schedule(dynamic, 4) num_threads(threads)
    for (int i = 0; i < iterations; i++)
    {
        task(i, omp_get_thread_num());
        results[omp_get_thread_num()].push_back(i);
    }
    print_results(results, threads);

    clear_results(results, threads);
    printf("Running %d iterations on %d threads with schedule(guided, 1)\n", iterations, threads);
    #pragma omp parallel for schedule(guided, 1) num_threads(threads)
    for (int i = 0; i < iterations; i++)
    {
        task(i, omp_get_thread_num());
        results[omp_get_thread_num()].push_back(i);
    }
    print_results(results, threads);

    clear_results(results, threads);
    printf("Running %d iterations on %d threads with schedule(guided, 4)\n", iterations, threads);
    #pragma omp parallel for schedule(guided, 4) num_threads(threads)
    for (int i = 0; i < iterations; i++)
    {
        task(i, omp_get_thread_num());
        results[omp_get_thread_num()].push_back(i);
    }
    print_results(results, threads);

    return 0;
}


int task_random(int iter, int thread)
{
    int wait_time = rand() % 100;
    usleep(wait_time * 1000);
    return wait_time;
}


int task_stable(int iter, int thread)
{
    usleep(100 * 1000);
    return 100;
}


int task_increasing_1(int iter, int thread)
{
    usleep(iter * 2000);
    return iter * 2;
}


int task_increasing_2(int iter, int thread)
{
    usleep((thread + 1) * 30 * 1000);
    return (thread + 1) * 30;
}


void print_results(std::vector<int> results[], int threads_count)
{
    bool colorize = false;

    const char *bold = "\x1b[1m", *normal = "\x1b[0m", *green = "\x1b[1;32m";

    if (not colorize)
    {
        bold = "";
        normal = "";
        green = "";
    }

    int maximum_vector_len = 0;
    for (int i = 0; i < threads_count; i++)
    {
        if (results[i].size() > maximum_vector_len)
            maximum_vector_len = results[i].size();
    }

    printf("┏━━━━━━━━━━");
    for (int i = 0; i < maximum_vector_len; i++)
        printf("%s━━━━━━━", i == 0 ? "┯" : "━");
    printf("┓\n");

    printf("┃ %sThread №%s │", bold, normal);
    printf("%*s%*s┃\n", maximum_vector_len * 4 - 1, "Iterations", maximum_vector_len * 4, " ");

    for (int i = 0; i < threads_count; i++)
    {
        printf("┣━━━━━━━━━━");
        for (int k = 0; k < maximum_vector_len; k++)
            printf("%s━━━━━━━", k == 0 or i != 0 ? "┿" : "┯");
        printf("┫\n");

        printf("┃ %sThread %d%s ", green, i, normal);
        for (int k = 0; k < maximum_vector_len; k++)
        {
            if (k < results[i].size())
                printf("│ %5d ", results[i][k]);
            else
                printf("│       ");
        }

        printf("┃\n");
    }

    printf("┗━━━━━━━━━━");
    for (int i = 0; i < maximum_vector_len; i++)
        printf("┷━━━━━━━");
    printf("┛\n\n");
}


void clear_results(std::vector<int> results[], int threads_count)
{
    for (int i = 0; i < threads_count; i++)
        results[i].clear();
}
