### [Laboratorium 8](https://artemis.wszib.edu.pl/~funika/pwir/MPI/lab7/)
# Temat: PCAM – weryfikacja rozwiązań dotyczących zrównoleglenia algorytmów
### **Andrzej Kotulski**
#### 02.02.2025r.

---



## 1. Co było do zrobienia

Celem laboratorium było przeprowadzenie analizy i weryfikacji równoległego rozwiązania problemu na podstawie [metodologii PCAM](https://www.mcs.anl.gov/~itf/dbpp/text/book.html) (*Partitioning, Communication, Agglomeration, Mapping*) zaproponowanej przez Iana Fostera. W ramach ćwiczenia należało przeanalizować własne rozwiązanie równoległe wybranego problemu. Przedmiotem analizy był [program implementujący równoległą wersję "Gry Życia"](https://github.com/akotu235/MPI/blob/master/game_of_life/plife.c) Johna Conwaya.

## 2. Podejście do rozwiązania problemu

Szczegóły implementacji dostępne są pod adresem: <https://github.com/akotu235/MPI/blob/master/report/report2.md>

W celu zrównoleglenia algorytmu "Gry Życia" zastosowano bibliotekę MPI, dzieląc planszę na segmenty przypisane do różnych procesów. Każdy proces otrzymuje fragment siatki, na której wykonuje obliczenia. Po każdej iteracji generacji procesy wymieniają między sobą krawędziowe wiersze, zapewniając poprawność symulacji.

Podział został zrealizowany w sposób jednowymiarowy (wiersze). Proces $0$ pełni dodatkową funkcję zarządzającą, inicjalizując planszę i rozsyłając dane. Synchronizacja pomiędzy procesami odbywa się za pomocą operacji `MPI_Sendrecv`, które umożliwiają wymianę brzegowych wierszy pomiędzy sąsiadującymi procesami.

## 3. Weryfikacja rozwiązania

### Partycjonowanie (Partitioning)
1. **Czy mamy do czynienia z liczbą zadań (tasków) przekraczającą liczbę procesorów o co najmniej rząd wielkości?**

   ❌
   ***Nie**. Liczba zadań odpowiada liczbie wierszy przydzielonych do procesorów. Jeśli rozmiar siatki (`N`) jest duży, a liczba procesorów (`psize`) mała, liczba zadań (wierszy) może przekraczać `psize` o rząd wielkości. Jednak w przypadku małego `N` lub dużego `psize` warunek nie jest spełniony.*

2. **Czy podział nie powoduje nadmiaru obliczeń i nadmiaru przechowywania danych? Jeśli nie, algorytm nie będzie dobrze skalowalny.**

   ✅
   ***Tak**. Podział na wiersze nie generuje nadmiaru obliczeń ani danych poza konieczną komunikacją brzegów.*

3. **Czy zadania są podobnych rozmiarów? Jeśli nie procesory będą nierównomiernie obciążone.**

   ❌
   ***Nie**. Gdy `N` nie jest podzielne przez `psize`, procesory o niższych ID mają więcej wierszy, co prowadzi do nierównego obciążenia.*

4. **Czy ilość zadań rośnie wraz ze wzrostem rozmiaru problemu? Jeśli nie może nie być możliwe rozwiązanie dużych problemów, nawet jeśli będziemy dysponowali dużą ilością procesorów.**

   ✅
   ***Tak**. Liczba zadań (wierszy) rośnie liniowo z rozmiarem problemu (`N`).*

5. **Czy rozważono inne możliwości?**

   ❌
   ***Nie**. Wykorzystano tylko podział 1D (wiersze). Nie rozważano innych metod, np. partycjonowania 2D.*

### Komunikacja (Communication)
1. **Czy wszystkie taski mają w przybliżeniu taką samą ilość wymienianych komunikatów?**

   ✅
   ***Tak**. Każdy procesor wymienia stałą liczbę komunikatów (2 wiersze z dwoma sąsiadami).*

2. **Czy każdy task komunikuje się tylko z małą ilością sąsiadów?**

   ✅
   ***Tak**. Każdy procesor komunikuje się tylko z bezpośrednimi sąsiadami (prev i next).*

3. **Czy operacje komunikacji mogą się wykonywać współbieżnie? Jeśli nie, algorytm jest nieefektywny i nieskalowalny.**

   ✅
   ***Tak**. Wykorzystano `MPI_Sendrecv`, które umożliwia współbieżną komunikację.*

4. **Czy obliczenia związane z poszczególnymi zadaniami mogą się wykonywać współbieżnie?**

   ✅
   ***Tak**. Obliczenia dla lokalnych wierszy są wykonywane niezależnie po synchronizacji brzegów.*

### Aglomeracja (Agglomeration)
1. **Czy aglomeracja zredukowała koszty komunikacyjne zwiększając lokalność?**

   ✅
   ***Tak**. Aglomeracja wierszy redukuje liczbę komunikatów poprzez grupowanie obliczeń w większe bloki.*

2. **Jeśli w ramach aglomeracji zreplikowano obliczenia, czy sprawdzono, że korzyści z tej replikacji przewyższają jej koszty, dla różnych rozmiarów problemów i liczby procesorów?**

   ❎
   ***Nie dotyczy**. Brak replikacji obliczeń.*

3. **Jeśli w ramach aglomeracji zreplikowano dane, czy sprawdzono, że nie ma zagrożenia  dla skalowalności algorytmu poprzez ograniczenie zakresu rozmiarów problemów lub liczby procesorów, które mogą uczestniczyć w obliczeniach?**

   ❎
   ***Nie dotyczy**. Brak replikacji danych poza koniecznymi brzegami.*

4. **Jeśli aglomeracja sprawiła replikację obliczeń, czy zweryfikowano wzrost korzyści płynących przy wzroście rozmiaru problemu i ilości procesorów?**

   ❎
   ***Nie dotyczy**. Brak replikacji obliczeń.*

5. **Czy aglomeracja stworzyła zadania (taski) o podobnym koszcie komunikacyjnym obliczeniowym?**

   ✅
   ***Częściowo**. Koszt obliczeniowy jest podobny, ale nierówny podział wierszy może prowadzić do różnic.*

6. **Czy ilość zadań wciąż jest skalowalna ze wzrostem rozmiaru problemu? Jeśli nie algorytm nie będzie nadawał się do rozwiązywania większych problemów na większych maszynach.**

   ✅
   ***Tak**. Liczba zadań (wierszy) skaluje się z rozmiarem problemu (`N`).*

7. **Czy aglomeracja wyeliminowała możliwość współbieżnego wykonania?**

   ✅
   ***Nie**. Współbieżność jest zachowana.*

8. **Czy koszty modyfikacji algorytmu sekwencyjnego są wysokie?**

   ✅
   ***Tak**. Modyfikacje względem wersji sekwencyjnej (np. komunikacja brzegów) są umiarkowanie kosztowne.*

### Odwzorowanie (Mapping)

1. **Rozważając zastosowanie metody (techniki) Single Program Multiple Data (SPMD) dla problemu obliczeniowego który ma być rozwiązany, czy rozważano również algorytm oparty na dynamicznym tworzeniu i usuwaniu zadań (tasków)? To drugie podejście może dać prostszy algorytm, jednak wydajność może stać się problematyczna.**

   ❌
   ***Nie**. Wykorzystano wyłącznie podejście SPMD. Nie rozważano alternatyw z dynamicznym tworzeniem zadań.*

2. **Czy rozważając projekt oparty na dynamicznym tworzeniu i usuwaniu zadań, rozważano również algorytm realizowany metodą SPMD? Technika SPMD zapewnia większą kontrolę nad planowaniem komunikacji i obliczeń, ale może być bardziej złożony.**

   ❌
   ***Nie**. Brak analizy porównawczej z podejściem SPMD.*

3. **Jeśli korzysta się ze scentralizowanego schematu równoważenia obciążenia, czy sprawdzono, że menedżer (master) nie stanie się wąskim gardłem? Można zmniejszyć koszty komunikacji w tych schematach, przekazując masterowi wskaźniki do zadań (tasków), a nie same taski.**

   ❌
   ***Nie**. Proces $0$ pełni rolę centralnego menedżera podczas zbierania danych do wypisywania, co może stać się wąskim gardłem dla dużych `N`.*

4. **Jeśli korzysta się z dynamicznego schematu równoważenia obciążenia, czy oszacowano względne koszty różnych strategii? Pamiętajmy, aby w analizie uwzględnić koszty wdrożenia. Probabilistyczne lub cykliczne schematy mapowania są proste i należy je zawsze brać pod uwagę, ponieważ pozwalają uniknąć konieczności powtarzania operacji równoważenia obciążenia.**

   ❌
   ***Nie**. Użyto statycznego podziału zadań. Nie oszacowano kosztów dynamicznego równoważenia obciążenia.*

5. **Jeśli korzysta się z metod probabilistycznych lub cyklicznych, czy jest wystarczająco dużo zadań (tasków), aby zapewnić rozsądne równoważenie obciążenia? Zazwyczaj wymagane jest co najmniej dziesięć razy więcej tasków niż procesorów.**

   ✅
   ***Tak**. Dla dużych `N` liczba wierszy na procesor jest wystarczająca (np.  `N` $= 1000$, `psize` $= 4$ daje $250$ wierszy/procesor).*

## 4. Wnioski

W przeprowadzonej analizie zauważono kilka aspektów mogących wpływać na wydajność i skalowalność rozwiązania:

* Główną wadą implementacji jest nierównomierny podział obciążenia w przypadku, gdy `N` nie jest podzielne przez `psize`. Można by to poprawić, stosując np. dynamiczne równoważenie obciążenia lub podział 2D.

* Algorytm korzysta ze statycznego podziału pracy, co jest proste do implementacji, ale może powodować problemy w przypadku dużej liczby procesorów i nierównomiernego podziału danych.

* Komunikacja między procesami została zaimplementowana optymalnie przy pomocy `MPI_Sendrecv`, co pozwala na jednoczesną wymianę danych.

* Proces $0$ pełni rolę centralnego menedżera wyników, co może stanowić wąskie gardło dla dużych instancji problemu.

Potencjalnym usprawnieniem rozwiązania mogłoby być zastosowanie bardziej zaawansowanego modelu podziału obliczeń (np. podział 2D zamiast 1D) oraz dynamiczne równoważenie obciążenia w zależności od rozkładu obliczeń.

Implementacja spełnia podstawowe wymagania i pozwala na skuteczne zrównoleglenie "Gry Życia", chociaż istnieją możliwości jej optymalizacji, zwłaszcza pod kątem równomiernego podziału obciążenia i eliminacji potencjalnego wąskiego gardła w postaci centralnego procesu zbierającego wyniki.

## 5. Bibliografia

1. [Materiały z laboratorium](https://artemis.wszib.edu.pl/~funika/pwir/MPI/lab7/)

2. Ian Foster, Designing and Building Parallel Programs: <https://www.mcs.anl.gov/~itf/dbpp/text/book.html>

## 6. Załączniki

1. **Repozytorium kodu źródłowego** – Pełny kod projektu. Dostępne online: <https://github.com/akotu235/MPI>

2. **Wersja online sprawozdania** – Bieżąca wersja dokumentu. Dostępne online: <https://github.com/akotu235/MPI/blob/master/report/report4.md>

3. **Sprawozdanie z laboratorium dotyczącego zrównoleglenia "Gry Życie"**. Dostępne online: <https://github.com/akotu235/MPI/blob/master/report/report2.md>