// Test "di fumo" (smoke test): non verifica nessuna regola del gioco.
// Serve solo a dimostrare che la catena "compila → collega Catch2 → esegui i test"
// funziona. Quando avremo la classe Board, aggiungeremo test_board.cpp.

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Lo scheletro del progetto compila ed esegue i test", "[smoke]")
{
    REQUIRE(1 + 1 == 2);
}
