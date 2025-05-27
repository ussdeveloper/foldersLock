cmake_minimum_required(VERSION 3.10)

project(FoldersLock LANGUAGES CXX) # Zmieniono nazwę projektu

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(FoldersLock main.cpp) # Zmieniono nazwę pliku wykonywalnego

set_target_properties(FoldersLock PROPERTIES
    OUTPUT_NAME "FoldersLock" # Zmieniono nazwę wyjściową
    # DEBUG_POSTFIX "_d"
)

# Opcjonalne: Instalacja
# install(TARGETS create_dirs DESTINATION bin) 

# Informacje dla użytkownika po konfiguracji CMake
message(STATUS "Konfiguracja CMake zakończona. Aby zbudować projekt, użyj:")
message(STATUS "  cmake --build . --config Release  (lub Debug)")
message(STATUS "Plik wykonywalny 'FoldersLock' zostanie utworzony w katalogu build.") # Zmieniono nazwę w komunikacie
