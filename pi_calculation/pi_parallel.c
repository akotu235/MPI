#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

double random_coordinate(double radius) {
    return (2.0 * rand() / RAND_MAX - 1.0) * radius;
}

int main(int argc, char** argv) {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int total_samples = 1000000000;
    const double radius = 16.0;

    // Podział próbek
    int remainder = total_samples % size;
    int num_per_proc = total_samples / size;
    if (rank < remainder) {
        num_per_proc++;
    }

    int local_hits = 0;

    srand(time(NULL) + rank * 12345);

    for (int i = 0; i < num_per_proc; i++) {
        double x = random_coordinate(radius);
        double y = random_coordinate(radius);
        if (x*x + y*y <= radius*radius) {
            local_hits++;
        }
    }

    // Zbieranie wyników
    if (rank == 0) {
        int total_hits = local_hits;
        int temp_hits;
        for (int src = 1; src < size; src++) {
            MPI_Recv(&temp_hits, 1, MPI_INT, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_hits += temp_hits;
        }
        double pi = 4.0 * (double)total_hits / total_samples;
        printf("Approximated Pi: %f\n", pi);
    } else {
        MPI_Send(&local_hits, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    // Zakończenie pomiaru czasu
    if (rank == 0) {
        gettimeofday(&end, NULL);
        double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
        printf("Total execution time with %d processes: %f seconds\n", size, time_spent);
    }

    return 0;
}