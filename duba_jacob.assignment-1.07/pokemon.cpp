#include "pokemon.h"
#include <climits>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * Get file path for csv file. Tries /share/cs327/ first, and then ~/.poke327.
 * If both do not exist, returns 1.
 */
int get_file_path(std::string *loc, std::string file) {
        const std::string CSV_DIR_1 = "/share/cs327/pokedex/pokedex/data/csv/";
        const std::string CSV_DIR_2 = std::string(std::getenv("HOME")) +
                                      "/.poke327/pokedex/pokedex/data/csv/";

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

int stoi_else_intmax(std::string s) {
        if (s == "") {
                return INT_MAX;
        }

        return std::stoi(s);
}

std::ostream &operator<<(std::ostream &o, const pokemon_data &p) {
        return o << "pokemon[id: " << p.id << ", identifier: " << p.identifier
                 << ", species_id: " << p.species_id << ", height: " << p.height
                 << ", weight: " << p.weight
                 << ", base_experience: " << p.base_experience
                 << ", order: " << p.order << ", is_default: " << p.is_default
                 << "]";
}

std::istream &operator>>(std::istream &i, pokemon_data &p) {
        std::string id;
        std::string identifier;
        std::string species_id;
        std::string height;
        std::string weight;
        std::string base_experience;
        std::string order;
        std::string is_default;

        getline(i, id, ',');
        p.id = stoi_else_intmax(id);
        getline(i, identifier, ',');
        p.identifier = identifier;
        getline(i, species_id, ',');
        p.species_id = stoi_else_intmax(species_id);
        getline(i, height, ',');
        p.height = stoi_else_intmax(height);
        getline(i, weight, ',');
        p.weight = stoi_else_intmax(weight);
        getline(i, base_experience, ',');
        p.base_experience = stoi_else_intmax(base_experience);
        getline(i, order, ',');
        p.order = stoi_else_intmax(order);
        getline(i, is_default);
        p.is_default = stoi_else_intmax(is_default);

        return i;
}

int load_pokemon(std::vector<pokemon_data> &poke_list) {
        std::string loc;

        if (get_file_path(&loc, "pokemon.csv")) {
                std::cout << "pokemon.csv does not exist" << std::endl;
                return 1;
        }

        std::ifstream file(loc.c_str());

        std::string firstline;
        getline(file, firstline);

        while (file.peek() != -1) {
                pokemon_data pokemon;

                file >> pokemon;

                poke_list.push_back(pokemon);
        }

        return 0;
}

std::ostream &operator<<(std::ostream &o, const move_data &m) {
        return o << "move[id: " << m.id << ", identifier: " << m.identifier
                 << ", generation_id: " << m.generation_id
                 << ", type_id: " << m.type_id << ", power: " << m.power
                 << ", pp: " << m.pp << ", accuracy: " << m.accuracy
                 << ", priority: " << m.priority
                 << ", target_id: " << m.target_id
                 << ", damage_class_id: " << m.damage_class_id
                 << ", effect_id: " << m.effect_id
                 << ", effect_chance: " << m.effect_chance
                 << ", contest_type_id: " << m.contest_type_id
                 << ", contest_effect_id: " << m.contest_effect_id
                 << ", super_contest_effect_id: " << m.super_contest_effect_id
                 << "]";
}

std::istream &operator>>(std::istream &i, move_data &m) {
        std::string id;
        std::string identifier;
        std::string generation_id;
        std::string type_id;
        std::string power;
        std::string pp;
        std::string accuracy;
        std::string priority;
        std::string target_id;
        std::string damage_class_id;
        std::string effect_id;
        std::string effect_chance;
        std::string contest_type_id;
        std::string contest_effect_id;
        std::string super_contest_effect_id;

        getline(i, id, ',');
        m.id = stoi_else_intmax(id);
        getline(i, identifier, ',');
        m.identifier = identifier;
        getline(i, generation_id, ',');
        m.generation_id = stoi_else_intmax(generation_id);
        getline(i, type_id, ',');
        m.type_id = stoi_else_intmax(type_id);
        getline(i, power, ',');
        m.power = stoi_else_intmax(power);
        getline(i, pp, ',');
        m.pp = stoi_else_intmax(pp);
        getline(i, accuracy, ',');
        m.accuracy = stoi_else_intmax(accuracy);
        getline(i, priority, ',');
        m.priority = stoi_else_intmax(priority);
        getline(i, target_id, ',');
        m.target_id = stoi_else_intmax(target_id);
        getline(i, damage_class_id, ',');
        m.damage_class_id = stoi_else_intmax(damage_class_id);
        getline(i, effect_id, ',');
        m.effect_id = stoi_else_intmax(effect_id);
        getline(i, effect_chance, ',');
        m.effect_chance = stoi_else_intmax(effect_chance);
        getline(i, contest_type_id, ',');
        m.contest_type_id = stoi_else_intmax(contest_type_id);
        getline(i, contest_effect_id, ',');
        m.contest_effect_id = stoi_else_intmax(contest_effect_id);
        getline(i, super_contest_effect_id);
        m.super_contest_effect_id = stoi_else_intmax(super_contest_effect_id);

        std::cout << m << std::endl;

        return i;
}

int load_moves(std::vector<move_data> &move_list) {
        std::string loc;

        if (get_file_path(&loc, "moves.csv")) {
                std::cout << "moves.csv does not exist" << std::endl;
                return 1;
        }

        std::ifstream file(loc.c_str());

        std::string firstline;
        getline(file, firstline);

        while (file.peek() != -1) {
                move_data move;

                file >> move;

                move_list.push_back(move);
        }

        return 0;
}
