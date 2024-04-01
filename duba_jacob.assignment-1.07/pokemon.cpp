#include "pokemon.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const std::string CSV_DIR_1 = "/share/cs327/pokedex/pokedex/data/csv/";
const std::string CSV_DIR_2 =
    std::string(std::getenv("HOME")) + "/.poke327/pokedex/pokedex/data/csv/";

/**
 * Get file path for csv file. Tries /share/cs327/ first, and then ~/.poke327.
 * If both do not exist, returns 1.
 */
int get_file_path(std::string *loc, std::string file) {
        if (std::filesystem::exists(CSV_DIR_1 + file)) {
                *loc = CSV_DIR_1 + file;
                return 0;
        }

        if (std::filesystem::exists(CSV_DIR_2 + file)) {
                *loc = CSV_DIR_2 + file;
                return 0;
        }

        return 1;
}

std::ostream &operator<<(std::ostream &o, const pokemon &p) {
        return o << "pokemon[id: " << p.id << ", identifier: " << p.identifier
                 << ", species_id: " << p.species_id << ", height: " << p.height
                 << ", weight: " << p.weight
                 << ", base_experience: " << p.base_experience
                 << ", order: " << p.order << ", is_default: " << p.is_default
                 << "]";
}

int load_pokemon(std::vector<pokemon> &poke_list) {
        std::string loc;

        if (get_file_path(&loc, "pokemon.csv")) {
                std::cout << "pokemon.csv does not exist" << std::endl;
                return 1;
        }

        std::ifstream file(loc.c_str());

        std::string firstline;
        getline(file, firstline);

        std::string id;
        std::string identifier;
        std::string species_id;
        std::string height;
        std::string weight;
        std::string base_experience;
        std::string order;
        std::string is_default;

        while (file.peek() != -1) {
                pokemon pokemon;

                getline(file, id, ',');
                pokemon.id = std::stoi(id);
                getline(file, identifier, ',');
                pokemon.identifier = identifier;
                getline(file, species_id, ',');
                pokemon.species_id = std::stoi(species_id);
                getline(file, height, ',');
                pokemon.height = std::stoi(height);
                getline(file, weight, ',');
                pokemon.weight = std::stoi(weight);
                getline(file, base_experience, ',');
                pokemon.base_experience = std::stoi(base_experience);
                getline(file, order, ',');
                pokemon.order = std::stoi(order);
                getline(file, is_default);
                pokemon.is_default = std::stoi(is_default);

                poke_list.push_back(pokemon);
        }

        return 0;
}
