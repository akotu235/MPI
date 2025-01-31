# MPI

Repozytorium zawiera przykładowe programy wykorzystujące MPI (Message Passing Interface) do równoległego przetwarzania.

## Instalacja MPICH

MPICH to implementacja standardu MPI. Instrukcje instalacji można znaleźć na oficjalnej stronie projektu: <https://www.mpich.org>

Aby sprawdzić poprawność instalacji:
```sh
mpiexec --version
```

## Kompilacja

Aby skompilować program MPI, użyj `mpicc`:

```sh
mpicc program.c -o program
```

## Uruchamianie

Program można uruchomić na określonej liczbie procesów przy użyciu `mpiexec` lub `mpirun`:

```sh
mpiexec -n <liczba_procesów> ./program
```

Przykład uruchomienia na 4 procesach:
```sh
mpiexec -n 4 ./program
```

