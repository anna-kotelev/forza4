# =============================================================================
# Test automatici con Catch2 (libreria installata tramite vcpkg, vedi vcpkg.json)
# =============================================================================

# Cerca la libreria Catch2, versione 3. REQUIRED = errore se non la trova.
find_package(Catch2 3 CONFIG REQUIRED)

# Un eseguibile separato che contiene solo i test.
add_executable(forza4_tests
    test_smoke.cpp
)

# I test usano la logica di gioco (forza4_core) e Catch2.
# Catch2WithMain fornisce già la funzione main(): noi scriviamo solo i test.
target_link_libraries(forza4_tests PRIVATE forza4_core Catch2::Catch2WithMain)

# Registra ogni TEST_CASE presso CTest, così si possono eseguire con "ctest".
include(Catch)
catch_discover_tests(forza4_tests)
