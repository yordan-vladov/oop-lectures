# ObserverNews (CMake, C++17)

## Build
```bash
cmake -S . -B build
cmake --build build
```

## Run
```bash
./build/ObserverNews
```

Проектът демонстрира Observer pattern (push модел) с:
- `NewsAgency` като Subject
- `TVChannel`, `Radio`, `MobileApp`, `Website` като Observers
- `std::weak_ptr` за безопасно съхранение на наблюдатели
