// Standardowe biblioteki wejścia/wyjścia i obsługi stringów
#include <iostream>
#include <string>
// Biblioteka systemu plików (wymaga C++17)
#include <filesystem>
// Do konwersji string na int (atoi)
#include <cstdlib>

// Użycie przestrzeni nazw std::filesystem dla wygody
namespace fs = std::filesystem;

/**
 * @brief Rekurencyjnie tworzy strukturę katalogów.
 *
 * Dla danej ścieżki i głębokości, funkcja tworzy 10 podkatalogów (0-9)
 * i wywołuje samą siebie dla każdego z nich, aż do osiągnięcia maksymalnej głębokości.
 *
 * @param currentPath Aktualna ścieżka, w której tworzone są podkatalogi.
 * @param currentDepth Aktualna głębokość rekurencji.
 * @param maxDepth Maksymalna docelowa głębokość struktury katalogów.
 */
void createDirectoriesRecursively(const fs::path& currentPath, int currentDepth, int maxDepth) {
    // Warunek bazowy rekurencji: jeśli osiągnięto maksymalną głębokość, zakończ.
    if (currentDepth >= maxDepth) {
        return;
    }

    // Pętla tworząca podkatalogi o nazwach 0, 1, ..., 9
    for (int i = 0; i < 10; ++i) {
        // Tworzenie pełnej ścieżki do nowego podkatalogu
        fs::path newDir = currentPath / std::to_string(i);
        bool can_recurse = false; // Flaga określająca, czy można kontynuować rekurencję dla newDir

        try {
            // Próba utworzenia katalogu.
            // fs::create_directory zwraca true, jeśli katalog został pomyślnie utworzony.
            // Zwraca false, jeśli katalog już istniał (i jest katalogiem).
            // Rzuca wyjątek fs::filesystem_error w przypadku innych błędów (np. brak uprawnień,
            // newDir jest istniejącym plikiem).
            if (fs::create_directory(newDir)) {
                // Katalog został nowo utworzony
                // std::cout << "Utworzono katalog: " << newDir.string() << std::endl; // Opcjonalne: do debugowania
                can_recurse = true;
            } else {
                // Katalog już istniał (i jest katalogiem, inaczej zostałby rzucony wyjątek)
                // std::cout << "Katalog już istnieje (lub nie można było utworzyć, ale nie było błędu krytycznego): " << newDir.string() << std::endl; // Opcjonalne
                can_recurse = true; // Zakładamy, że jeśli nie było błędu, a katalog istnieje, to jest to katalog
            }
        } catch (const fs::filesystem_error& e) {
            // Przechwytywanie błędów systemu plików, np. newDir jest istniejącym plikiem lub brak uprawnień.
            std::cerr << "Błąd podczas tworzenia lub dostępu do katalogu " << newDir.string() << ": " << e.what() << std::endl;
            // can_recurse pozostaje false, więc nie próbujemy rekurencji dla problematycznej ścieżki.
        }

        // Jeśli tworzenie/dostęp do katalogu się powiodło, kontynuuj rekurencję
        if (can_recurse) {
            createDirectoriesRecursively(newDir, currentDepth + 1, maxDepth);
        }
    }
}

/**
 * @brief Główna funkcja programu.
 *
 * Parsuje argumenty wiersza poleceń, waliduje je i inicjuje proces tworzenia katalogów.
 * Argumenty:
 *  1. Nazwa katalogu root lub ścieżka do niego (obowiązkowy).
 *  2. Głębokość struktury katalogów (opcjonalny, domyślnie 4).
 */
int main(int argc, char* argv[]) {
    // Nazwa katalogu głównego lub pełna ścieżka
    std::string rootDirName;
    // Domyślna głębokość zagnieżdżenia katalogów
    int depth = 4;

    // Sprawdzenie, czy podano przynajmniej jeden argument (nazwę/ścieżkę katalogu root)
    if (argc < 2) {
        std::cerr << "Użycie: " << argv[0] << " <nazwa_katalogu_root_lub_sciezka> [glebokosc]" << std::endl;
        std::cerr << "  <nazwa_katalogu_root_lub_sciezka>: Obowiązkowe. Nazwa lub ścieżka do katalogu głównego." << std::endl;
        std::cerr << "  [glebokosc]: Opcjonalne. Liczba poziomów podkatalogów. Domyślnie " << depth << "." << std::endl;
        std::cerr << "\nAby skompilować ten program (FoldersLock):" << std::endl; // Dodano nazwę projektu
        std::cerr << "  Użyj CMake (zalecane, plik CMakeLists.txt jest dostarczony):" << std::endl;
        std::cerr << "    mkdir build && cd build" << std::endl;
        std::cerr << "    cmake .." << std::endl;
        std::cerr << "    cmake --build ." << std::endl;
        std::cerr << "  Lub bezpośrednio kompilatorem C++17 (np. g++):" << std::endl;
        std::cerr << "    g++ -std=c++17 main.cpp -o FoldersLock" << std::endl; // Zmieniono nazwę pliku wykonywalnego
        std::cerr << "Przykładowe użycie FoldersLock:" << std::endl; // Dodano nazwę projektu
        std::cerr << "  .\\FoldersLock.exe moj_katalog_danych" << std::endl; // Zmieniono nazwę pliku wykonywalnego
        std::cerr << "  .\\FoldersLock.exe C:\\\\sciezka\\\\do\\\\katalogu 3" << std::endl; // Zmieniono nazwę pliku wykonywalnego
        return 1; // Zakończ program z kodem błędu
    }

    // Pierwszy argument to nazwa/ścieżka katalogu root
    rootDirName = argv[1];

    // Jeśli podano drugi argument (głębokość)
    if (argc > 2) {
        depth = std::atoi(argv[2]); // Konwersja argumentu na liczbę całkowitą
        // Walidacja głębokości
        if (depth <= 0) {
            std::cerr << "Głębokość musi być dodatnią liczbą całkowitą. Używam domyślnej głębokości: " << 4 << std::endl;
            depth = 4; // Przywrócenie wartości domyślnej
        }
    }

    // Utworzenie obiektu ścieżki dla katalogu root
    fs::path rootPath = rootDirName;

    try {
        // Próba utworzenia katalogu root
        // fs::create_directory zwraca true, jeśli katalog został utworzony,
        // false, jeśli już istniał (i jest katalogiem).
        // Rzuca wyjątek w przypadku innych błędów.
        if (fs::create_directory(rootPath)) {
            std::cout << "Utworzono katalog główny: " << rootPath.string() << std::endl;
        } else {
            // Jeśli create_directory zwróciło false, katalog już istniał (lub nie można było utworzyć, ale nie było błędu krytycznego)
            // Sprawdzamy, czy na pewno istnieje i jest katalogiem, zanim przejdziemy dalej.
            if (fs::exists(rootPath) && fs::is_directory(rootPath)) {
                 std::cout << "Katalog główny '" << rootPath.string() << "' już istnieje. Przechodzę do tworzenia podkatalogów." << std::endl;
            } else if (fs::exists(rootPath)) {
                std::cerr << "Błąd: Ścieżka '" << rootPath.string() << "' istnieje, ale nie jest katalogiem." << std::endl;
                return 1; // Zakończ z błędem
            } else {
                // Ten przypadek jest mało prawdopodobny, jeśli create_directory zwróciło false bez wyjątku,
                // ale dla pewności.
                std::cerr << "Nie można utworzyć katalogu głównego: '" << rootPath.string() << "', a także nie istnieje." << std::endl;
                return 1; // Zakończ z błędem
            }
        }
        
        // Upewnij się, że katalog root istnieje i jest katalogiem przed rekurencją
        if (fs::exists(rootPath) && fs::is_directory(rootPath)) {
            createDirectoriesRecursively(rootPath, 0, depth);
            std::cout << "Struktura katalogów została pomyślnie utworzona w '" << rootPath.string() << "' z głębokością " << depth << "." << std::endl;
        } else if (!fs::exists(rootPath)) { // Jeśli rootPath nadal nie istnieje (np. create_directory zawiodło cicho)
             std::cerr << "Nie udało się utworzyć katalogu głównego: " << rootPath.string() << std::endl;
             return 1; // Zakończ z błędem
        }
        // Jeśli rootPath istnieje, ale nie jest katalogiem, błąd został już obsłużony wyżej.

    } catch (const fs::filesystem_error& e) {
        // Przechwytywanie błędów systemu plików podczas operacji na katalogu root
        std::cerr << "Błąd systemu plików: " << e.what() << std::endl;
        return 1; // Zakończ program z kodem błędu
    }

    return 0; // Zakończ program pomyślnie
}