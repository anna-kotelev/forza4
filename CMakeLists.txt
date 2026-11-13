# =============================================================================
# Forza 4: file principale di CMake
#
# CMake non compila da solo: legge questo file e GENERA i file di progetto
# per il vero sistema di compilazione (qui: Visual Studio / MSBuild con MSVC).
# =============================================================================

# Versione minima di CMake richiesta (serve per i CMakePresets.json versione 6).
cmake_minimum_required(VERSION 3.25)

# Nome del progetto, versione e linguaggio usato.
project(Forza4 VERSION 0.1.0 LANGUAGES CXX)

# --- Standard del linguaggio --------------------------------------------------
# C++20; REQUIRED = errore se il compilatore non lo supporta;
# EXTENSIONS OFF = solo C++ standard, niente estensioni specifiche del compilatore.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Avvisi (warning) del compilatore ------------------------------------------
# Un "target" INTERFACE non produce file: trasporta solo impostazioni.
# Chi lo "collega" (target_link_libraries) eredita queste opzioni.
#   /W4          = livello alto di avvisi (MSVC)
#   /permissive- = rispetto rigoroso dello standard C++ (MSVC)
#   /utf-8       = i file sorgente sono in UTF-8 (lettere accentate nei commenti)
add_library(forza4_warnings INTERFACE)
target_compile_options(forza4_warnings INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive- /utf-8>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# --- Logica di gioco (il "backend") ---------------------------------------------
# Qui finiranno Board, Player, Game... cioè le regole di docs/REGOLE.md.
# Per ora è INTERFACE (solo la cartella degli header); diventerà una libreria
# STATIC quando aggiungeremo i primi file .cpp in src/core/.
add_library(forza4_core INTERFACE)
target_include_directories(forza4_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(forza4_core INTERFACE forza4_warnings)

# --- Programma eseguibile (forza4.exe) -----------------------------------------
add_executable(forza4 src/main.cpp)
target_link_libraries(forza4 PRIVATE forza4_core)

# --- Test automatici -----------------------------------------------------------
option(FORZA4_BUILD_TESTS "Compila anche i test automatici" ON)
if(FORZA4_BUILD_TESTS)
    enable_testing()          # attiva CTest, lo strumento di CMake che esegue i test
    add_subdirectory(tests)   # legge tests/CMakeLists.txt
endif()
