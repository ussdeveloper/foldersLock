cmake_minimum_required(VERSION 3.10)

project(folders-lock LANGUAGES CXX) # Zmieniono nazwę projektu

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(folders-lock main.cpp) # Zmieniono nazwę pliku wykonywalnego

set_target_properties(folders-lock PROPERTIES
    OUTPUT_NAME "folders-lock" # Zmieniono nazwę wyjściową
    # DEBUG_POSTFIX "_d"
)

# Opcjonalne: Instalacja
# install(TARGETS create_dirs DESTINATION bin) 

# Informacje dla użytkownika po konfiguracji CMake
message(STATUS "Konfiguracja CMake zakończona. Aby zbudować projekt, użyj:")
message(STATUS "  cmake --build . --config Release  (lub Debug)")
message(STATUS "Plik wykonywalny 'FoldersLock' zostanie utworzony w katalogu build.") # Zmieniono nazwę w komunikacie
