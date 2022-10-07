# Клауза `copyin(variable)`

## Принцип действия

Говоря простым языком, данная клауза предназначена для копирования
**нового** значения глобальной переменной, объявленной, как 
`threadprivate` во все треды параллельной области. 

Так, если объявить глобальную переменную как `threadprivate`, а 
затем поменять ее значение в основном треде, то при входе в 
параллельную область во всех тредах, кроме основного, она 
будет инициализирована начальным значением, однако в основном 
треде значение переменной сохраниться.

Если в новых тредах необходимо инициализировать эту переменную 
тем же числом, которое было присвоено в основном треде, необходимо
использовать клаузу `copyin()`.

Демонстрация поведения такой переменной приведена в файле 
[copyin_demonstration.cpp](copyin_demonstration.cpp)