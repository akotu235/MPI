# Weryfikacja projektowania algorytmów równoległych

## Partycjonowanie (Partitioning)

Etap partycjonowania w rozwiązaniu problemu powinien dostarczyć jedną lub więcej możliwych dekompozycji problemu. Przed przystąpieniem do oceny wymagań komunikacyjnych korzystamy z poniższej listy kontrolnej celem upewnienia się, że rozwiązanie nie ma oczywistych wad. Ogólnie rzecz biorąc, na wszystkie te pytania należy odpowiedzieć twierdząco.

### Weryfikacja (Partitioning design checklist)

1. Czy mamy do czynienia z liczbą zadań (tasków) przekraczającą liczbę procesorów o co najmniej rząd wielkości?

2. Czy podział nie powoduje nadmiaru obliczeń i nadmiaru przechowywania danych? Jeśli nie, algorytm nie będzie dobrze skalowalny.

3. Czy zadania są podobnych rozmiarów? Jeśli nie procesory będą nierównomiernie obciążone.

4. Czy ilość zadań rośnie wraz ze wzrostem rozmiaru problemu? Jeśli nie może nie być możliwe rozwiązanie dużych problemów, nawet jeśli będziemy dysponowali dużą ilością procesorów.

5. Czy rozważono inne możliwości?

Odpowiedzi na te pytania mogą sugerować, że pomimo starannego przemyślenia na tym i kolejnych etapach projektowania rozwiązania, mamy nie do końca dobre podejście do rozwiązania. Powinniśmy  umieć odpowiednio zastosować techniki oceny wydajności (np. oszacowanie kosztów wykonania), aby określić, czy projekt spełnia nasze cele wydajnościowe pomimo widocznych niedociągnięć. Możemy również przeanalizować specyfikację problemu. W zastosowaniach naukowych i inżynierskich, gdzie problem do rozwiązania może obejmować symulację złożonego procesu fizycznego, aproksymacje i techniki obliczeń numerycznych użyte do opracowania symulacji mogą silnie wpłynąć na łatwość implementacji równoległej. W niektórych przypadkach, rozwiązania sekwencyjne i równoległe tego samego problemu mogą wykorzystywać zupełnie odmienne od siebie techniki rozwiązywania.

## Komunikacja (Communication)

Po opracowaniu dekompozycji (partycjonowania) i struktury komunikacji dla algorytmu, powinniśmy ocenić rozwiązanie, korzystając z poniższej listy kontrolnych pytań. Są to wytyczne mające na celu identyfikację nieskalowanych cech rozwiązania, a nie twardych i sztywnych reguł. Powinniśmy zdawać sobie sprawę z tego, kiedy projekt je narusza i dlaczego.

### Weryfikacja (Communication design checklist)

1. Czy wszystkie taski mają w przybliżeniu taką samą ilość wymienianych komunikatów?

2. Czy każdy task komunikuje się tylko z małą ilością sąsiadów?

3. Czy operacje komunikacji mogą się wykonywać współbieżnie? Jeśli nie, algorytm jest nieefektywny i nieskalowalny.

4. Czy obliczenia związane z poszczególnymi zadaniami mogą się wykonywać współbieżnie?

## Aglomeracja (Agglomeration)

Po analizie decyzji dotyczących partycjonowania i komunikacji, opracowanych na pierwszych dwóch etapach, kolej jest na aglomerację zadań (tasków) i operacji komunikacyjnych. Możliwe jest zapotrzebowanie na aglomerację zadań, jeśli np. analiza wymagań komunikacyjnych wskazuje, że oryginalne partycjonowanie utworzyło zadania, które nie mogą być wykonywane jednocześnie. Alternatywnie, mogliśmy zastosować aglomerację do zwiększenia ziarnistości obliczeń i komunikacji i/lub zmniejszenia kosztów inżynierii oprogramowania, nawet jeśli możliwości współbieżnego wykonywania są ograniczone. Na tym etapie oceniamy nasz projekt pod kątem poniższej listy kontrolnej. Kilka z tych pytań kładzie nacisk na ilościową analizę wydajności, która staje się ważniejsza, gdy przechodzimy od abstrakcji do konkretu.

### Weryfikacja (Agglomeration design checklist)

1. Czy aglomeracja zredukowała koszty komunikacyjne zwiększając lokalność?

2. Jeśli w ramach aglomeracji zreplikowano obliczenia, czy sprawdzono, że korzyści z tej replikacji przewyższają jej koszty, dla różnych rozmiarów problemów i liczby procesorów?

3. Jeśli w ramach aglomeracji zreplikowano dane, czy sprawdzono, że nie ma zagrożenia  dla skalowalności algorytmu poprzez ograniczenie zakresu rozmiarów problemów lub liczby procesorów, które mogą uczestniczyć w obliczeniach?

4. Jeśli aglomeracja sprawiła replikację obliczeń, czy zweryfikowano wzrost korzyści płynących przy wzroście rozmiaru problemu i ilości procesorów?

5. Czy aglomeracja stworzyła zadania (taski) o podobnym koszcie komunikacyjnym obliczeniowym?

6. Czy ilość zadań wciąż jest skalowalna ze wzrostem rozmiaru problemu? Jeśli nie algorytm nie będzie nadawał się do rozwiązywania większych problemów na większych maszynach.

7. Czy aglomeracja wyeliminowała możliwość współbieżnego wykonania?

8. Czy koszty modyfikacji algorytmu sekwencyjnego są wysokie?

## Odwzorowanie (Mapping)

Zakończyliśmy projektowanie algorytmów równoległych, określając, w jaki sposób zadania (taski) zdefiniowane na poprzednich etapach projektowania są mapowane na procesory (procesy). Nasze decyzje dotyczące mapowania mają na celu zrównoważenie sprzecznych wymagań dotyczących sprawiedliwej dystrybucji obciążenia i niskich kosztów komunikacji. Jeśli jest to możliwe, używamy statycznego schematu mapowania, który przydziela każde zadanie do jednego procesora. Jednak gdy liczba lub wielkość zadań jest zmienna lub nie jest znana do czasu uruchomienia, możemy użyć schematu dynamicznego równoważenia obciążenia lub przeformułować problem, aby do planowania obliczeń można było wykorzystać scheduler do planowania zadań. Poniższa lista kontrolna może służyć jako podstawa do oceny procedury mapowania.

### Weryfikacja (Mapping design checklist)

1. Rozważając zastosowanie metody (techniki) Single Program Multiple Data (SPMD) dla problemu obliczeniowego który ma być rozwiązany, czy rozważano również algorytm oparty na dynamicznym tworzeniu i usuwaniu zadań (tasków)? To drugie podejście może dać prostszy algorytm, jednak wydajność może stać się problematyczna.

2. Czy rozważając projekt oparty na dynamicznym tworzeniu i usuwaniu zadań, rozważano również algorytm realizowany metodą SPMD? Technika SPMD zapewnia większą kontrolę nad planowaniem komunikacji i obliczeń, ale może być bardziej złożony.

3. Jeśli korzysta się ze scentralizowanego schematu równoważenia obciążenia, czy sprawdzono, że menedżer (master) nie stanie się wąskim gardłem? Można zmniejszyć koszty komunikacji w tych schematach, przekazując masterowi wskaźniki do zadań (tasków), a nie same taski.

4. Jeśli korzysta się z dynamicznego schematu równoważenia obciążenia, czy oszacowano względne koszty różnych strategii? Pamiętajmy, aby w analizie uwzględnić koszty wdrożenia. Probabilistyczne lub cykliczne schematy mapowania są proste i należy je zawsze brać pod uwagę, ponieważ pozwalają uniknąć konieczności powtarzania operacji równoważenia obciążenia.

5. Jeśli korzysta się z metod probabilistycznych lub cyklicznych, czy jest wystarczająco dużo zadań (tasków), aby zapewnić rozsądne równoważenie obciążenia? Zazwyczaj wymagane jest co najmniej dziesięć razy więcej tasków niż procesorów.
