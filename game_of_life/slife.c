#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  int K,N;
  int **tab, **tab2, **tmpwsk;
  int i, j, jed=0, nb_count;                        //ilosc sasiadow
  int pokolenie, il_pok;
  double pp; //prawdopodobienstwo jedynki

//odczytanie parametrow wywolania programu
  if (argc != 4) {
    fprintf(stderr,
         "Blad! Zle wywolanie. \n uzycie: %s <rozmiar: 1..n> <prawdopodobienstwo: 0..1> <pokolen:1..n>\n", argv[0]);
    exit(1);

  }
  else {
    N = K = atoi(argv[1]);
    printf("K: %d, ", K);
    pp = atof(argv[2]);
    printf("prawdopodobienstwo: %.2f\n", pp);
    il_pok=atoi(argv[3]);
    if ( pp > 1 || pp < 0) {
      fprintf(stderr, "Blad! Prawdopodobienstwo poza zakresem!!\n");
      exit(1);
    }
  }

//alokacja pamieci na tablice
  tab = (int **) calloc(K+2, sizeof(int *));
  tab2 = (int **) calloc(K+2, sizeof(int *));
  for (i = 0; i < K+2; i++) {
    tab[i] = (int *) calloc(N, sizeof(int));
    tab2[i] = (int *) calloc(N, sizeof(int));
  }


//wypelnianie pola poczatkowymi wartosciami
  srandom(time(NULL));
  for (i=1; i<=K; i++)
    for (j=0; j<N; j++) {
      tab[i][j] = (random()%100 < pp*100) ? 1 : 0;
      jed+=tab[i][j];
  }

  fprintf(stderr, "Wylosowano jedynek: %d\n", jed);
  for (pokolenie=1; pokolenie<=il_pok; pokolenie++) {
    jed=0;

//==============================

//TUTAJ PRZESYLANIE ZAKLADEK!!!!!
//ZADANIE DLA PANSTWA!!!!!!

//==============

//wypisywanie pola po komunikacji
  /*  for (i=0; i<=K+1; i++) {
      if (i==0 || i==K+1) printf("!");
      printf("PW%3d: ",i);
      for (j=0; j<N; j++) {
        printf("%d",tab[i][j]);
      }
      printf("\n");
    }
*/

//obliczenie nastepnego pokolenia!
    //kopiowanie tablicy
    //tab2[i][j]=tab[i][j];
    tmpwsk = tab2; tab2 = tab; tab = tmpwsk;

    for (i=1; i<=K; i++)
      for (j=0; j<N; j++) {
        //ilosc sasiadow
        nb_count =
          tab2[i-1][(j-1+N)%N]+tab2[i-1][j]+tab2[i-1][(j+1)%N]
          +tab2[i][(j-1+N)%N]+           tab2[i][(j+1)%N]
          +tab2[i+1][(j-1+N)%N]+tab2[i+1][j]+tab2[i+1][(j+1)%N];

        if (nb_count==3) {
          tab[i][j]=1;
        }
        else {
          if (nb_count==2)
            tab[i][j]=tab2[i][j];
          else
            tab[i][j]=0;
        }
	    jed+=tab[i][j];
      }

    printf("W pokoleniu: %d jedynek: %d\n", pokolenie+1, jed);
  }
// zwolnienie pamieci
  for (i = 0; i < K+2; i++) {
    free(tab[i]); free(tab2[i]);
  }
  free(tab);
  free(tab2);
  return 0;
}
