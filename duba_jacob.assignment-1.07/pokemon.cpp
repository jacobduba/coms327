#include <filesystem>
#include <iostream>
#include <string>

const std::string CSV_DIR_1 = "/share/cs327/pokedex/pokedex/data/csv/";
const std::string CSV_DIR_2 =
    std::string(std::getenv("HOME")) + "/.poke327/pokedex/pokedex/data/csv/";

int read_pokemon() {
        if (std::filesystem::exists(CSV_DIR_1)) {
                std::cout << "yippie1" << std::endl;
        } else if (std::filesystem::exists(CSV_DIR_2)) {
                std::cout << "yippie2" << std::endl;
        }

        return 0;
}
