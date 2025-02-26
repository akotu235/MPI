#include "mpi.h"  /* wlacza plik naglowkowy potrzebny dla biblioteki MPI */
#include <stdio.h> /* potrzebny dla funkcji 'fprintf()' */
#include <unistd.h> /* potrzebny dla funkcji 'sleep()' */

int main(int argc, char **argv) {
	int myid, namelen;
	char computer_name[512]; /* nazwa komputera na ktorym wykonuje sie proces */

	MPI_Init(&argc, &argv);  /* inicjalizuje srodowisko MPI */

	/* pobiera identyfikator biezacego procesu */
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	/* Pobiera nazwe komputera na ktorym proces sie wykonuje */
 	MPI_Get_processor_name(computer_name, &namelen);

	for (int i = 0; i < 100; i++) {
		printf("Hello from process %d on %s\n", myid, computer_name);
		sleep(1);
	}
	/* konczy dzialanie srodowiska MPI */
	MPI_Finalize();

  return 0;
}