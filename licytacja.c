#include "mpi.h"  /* wlacza plik naglowkowy potrzebny dla biblioteki MPI */
#include <stdio.h> /* potrzebny dla funkcji 'fprintf()' */
#include <stdlib.h>
#include <time.h> /* potrzebny dla funcji 'time()' */
#include <unistd.h> /* potrzebny dla funkcji 'sleep()' */

int main(int argc, char **argv) {
	int myid, namelen, i, size;
	char computer_name[512]; /* nazwa komputera na ktorym */
	int a[2];

	MPI_Status status;

	MPI_Init(&argc, &argv);  /* inicjalizuje srodowisko MPI */

	/* pobiera identyfikator biezacego procesu */
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	/* Pobiera nazwe komputera na ktorym proces sie wykonuje */
	MPI_Get_processor_name(computer_name, &namelen);

	/* pobieranie liczby procesów*/
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	srandom(time(NULL) + myid * 20);
	int maxBid = random() % 20 + 1;
	int currentBid = 0;
	int nextId = (myid == size - 1) ? 0 : myid + 1;
	int prevId = (myid == 0) ? size - 1 : myid - 1;

	/* proces 0 zaczyna licytacje*/
	if (myid == 0) {
		a[0] = 1;
		a[1] = myid;
		MPI_Send(a, 2, MPI_INT, nextId, 12, MPI_COMM_WORLD);
		fprintf(stderr, "Proces %d na %s[max = %d] mowi: %d\n", myid, computer_name, maxBid, a[0]);
	}

	while(1) {
		MPI_Recv(a, 2, MPI_INT, prevId, 12, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		if(a[0] == currentBid) {
			if(a[1] == myid) {
				fprintf(stderr, "Proces %d na %s[max = %d] wygral lictacje: %d\n", myid, computer_name, maxBid, a[0]);
			}
			MPI_Send(a, 2, MPI_INT, nextId, 12, MPI_COMM_WORLD);
			break;
		} else {
			if(a[0] < maxBid) {
				a[0] += 1;
				a[1] = myid;
				fprintf(stderr, "Proces %d na %s[max = %d] mowi: %d\n", myid, computer_name, maxBid, a[0]);
				usleep(100000);
			}
		}
		currentBid = a[0];
		MPI_Send(a, 2, MPI_INT, nextId, 12, MPI_COMM_WORLD);
	}

	/* konczy dzialanie srodowiska MPI */
	MPI_Finalize();

	return 0;
}