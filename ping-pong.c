#include "mpi.h"  /* wlacza plik naglowkowy potrzebny dla biblioteki MPI */
#include <stdio.h> /* potrzebny dla funkcji 'fprintf()' */
#include <unistd.h> /* potrzebny dla funkcji 'sleep()' */

int main(int argc, char **argv) {
	const int LIMIT = 20;
	int id, size;

	/* inicjalizuje srodowisko MPI */
	MPI_Init(&argc, &argv);

	/* pobiera identyfikator biezacego procesu */
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	/* pobieranie liczby procesów*/
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* zapewnienie ilosci procesow = 2*/
	if(size != 2) {
		printf("Program nalezy uruchomic z dwoma procesami \n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	int count = 0;
	int partner_id = (id +1 ) % 2;

	/* proces 0 serwuje*/
	if(id == 0) {
		count++;
		printf("proces %d serwuje, ilosc odbic: %d \n", id, count);
		MPI_Send(&count, 1, MPI_INT, partner_id, 0, MPI_COMM_WORLD);
	}

	while(count < LIMIT) {
		MPI_Recv(&count, 1, MPI_INT, partner_id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		sleep(1);
		if(count < LIMIT){
			count++;
			printf("proces %d odbija, ilosc odbic: %d \n", id, count);
			MPI_Send(&count, 1, MPI_INT, partner_id, 0, MPI_COMM_WORLD);
		}
	}

	/* konczy dzialanie srodowiska MPI */
	MPI_Finalize();
}