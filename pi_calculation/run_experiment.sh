#!/bin/bash

# Kompilacja programów
gcc pi_sequential.c -o ../out/pi_sequential -lm
mpicc pi_parallel.c -o ../out/pi_parallel -lm

# Sekwencyjny pomiar czasu
T_SEQ=$(../out/pi_sequential | awk '/Time/ {print $2}')
echo "Czas sekwencyjny: $T_SEQ s"

# Lista procesów do testowania
PROCS=(1 2 5 10 20)

# Plik wynikowy
DATA_FILE="results.dat"
rm -f $DATA_FILE
echo "# Procesy Czas Przyśpieszenie Efektywność" > $DATA_FILE

# Pomiary dla wersji równoległej
for p in "${PROCS[@]}"; do
    echo "Uruchamiam z $p procesami..."
    TIME=$(mpiexec -n $p ../out/pi_parallel | awk '/Total execution time/ {print $7}')

    # Oblicz przyśpieszenie i efektywność
    SPEEDUP=$(echo "$T_SEQ / $TIME" | bc -l)
    EFFICIENCY=$(echo "$SPEEDUP / $p" | bc -l)

    # Zapisz dane
    printf "%d %f %f %f\n" $p $TIME $SPEEDUP $EFFICIENCY >> $DATA_FILE
done

echo "Eksperyment zakończony. Wyniki w pliku results.dat"

## Generowanie wykresów
#python plot_results.py
#echo "Eksperyment zakończony. Wyniki w plikach speedup.png i efficiency.png"