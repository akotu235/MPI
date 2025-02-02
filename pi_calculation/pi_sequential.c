#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

double random_coordinate(double radius) {
    return (2.0 * rand() / RAND_MAX - 1.0) * radius;
}

int main() {
    const int total_samples = 100000000;
    const double radius = 16.0;
    int hits = 0;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    srand(time(NULL));

    for (int i = 0; i < total_samples; i++) {
        double x = random_coordinate(radius);
        double y = random_coordinate(radius);
        if (x*x + y*y <= radius*radius) {
            hits++;
        }
    }

    double pi = 4.0 * (double)hits / total_samples;

    gettimeofday(&end, NULL);
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printf("Approximated Pi: %f\n", pi);
    printf("Time: %f seconds\n", time_spent);

    return 0;
}