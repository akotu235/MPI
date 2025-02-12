import matplotlib.pyplot as plt

# Wczytaj dane z pliku
with open("results.dat") as f:
    lines = f.readlines()[1:]

# Przetwórz dane
procs, times, speedups, efficiencies = [], [], [], []
for line in lines:
    p, t, s, e = map(float, line.split())
    procs.append(p)
    times.append(t)
    speedups.append(s)
    efficiencies.append(e)

# Wykres przyśpieszenia
plt.figure()
plt.plot(procs, speedups, marker='o', label='Przyśpieszenie (S)')
plt.xlabel('Liczba procesów (p)')
plt.ylabel('Przyśpieszenie (S)')
plt.title('Przyśpieszenie (S = T_seq / T_par)')
plt.grid()
plt.legend()
plt.savefig('speedup.png')

# Wykres efektywności
plt.figure()
plt.plot(procs, efficiencies, marker='o', label='Efektywność (E)')
plt.xlabel('Liczba procesów (p)')
plt.ylabel('Efektywność (E)')
plt.title('Efektywność (E = S / p)')
plt.grid()
plt.legend()
plt.savefig('efficiency.png')