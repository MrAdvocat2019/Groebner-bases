# Groebner bases

Реализация вычисления базисов Грёбнера на C++20: арифметика многочленов от многих
переменных с выбираемыми в рантайме мономиальными порядками (Lex, GrLex, GrvLex),
редукция многочленов, алгоритм Бухбергера и приведённые базисы Грёбнера.

## Сборка

```bash
cmake --preset Debug      # либо Release для бенчмарков
cmake --build build/Debug
```

## Запуск тестов

```bash
cmake --build build/Debug --target Groebnerbases_test
./build/Debug/Groebnerbases_test
```

Для бенчмарка используйте пресет `Release`.
