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
        return o << "pokemon_data[id: " << p.id
                 << ", identifier: " << p.identifier
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

std::ostream &operator<<(std::ostream &o, const move_data &m) {
        return o << "move_data[id: " << m.id << ", identifier: " << m.identifier
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

        return i;
}

std::ostream &operator<<(std::ostream &o, const pokemon_move_data &m) {
        return o << "pokemon_move_data[pokemon_id: " << m.pokemon_id
                 << ", version_group_id: " << m.version_group_id
                 << ", move_id: " << m.move_id
                 << ", pokemon_move_method_id: " << m.pokemon_move_method_id
                 << ", level: " << m.level << ", order: " << m.order << "]";
}

std::istream &operator>>(std::istream &i, pokemon_move_data &m) {
        std::string pokemon_id;
        std::string version_group_id;
        std::string move_id;
        std::string pokemon_move_method_id;
        std::string level;
        std::string order;

        getline(i, pokemon_id, ',');
        m.pokemon_id = stoi_else_intmax(pokemon_id);
        getline(i, version_group_id, ',');
        m.version_group_id = stoi_else_intmax(version_group_id);
        getline(i, move_id, ',');
        m.move_id = stoi_else_intmax(move_id);
        getline(i, pokemon_move_method_id, ',');
        m.pokemon_move_method_id = stoi_else_intmax(pokemon_move_method_id);
        getline(i, level, ',');
        m.level = stoi_else_intmax(level);
        getline(i, order);
        m.order = stoi_else_intmax(order);

        return i;
}

std::ostream &operator<<(std::ostream &o, const pokemon_species_data &m) {
        return o << "pokemon_species_data[id: " << m.id
                 << ", identifier: " << m.identifier
                 << ", generation_id: " << m.generation_id
                 << ", evolves_from_species_id: " << m.evolves_from_species_id
                 << ", evolution_chain_id: " << m.evolution_chain_id
                 << ", color_id: " << m.color_id << ", shape_id: " << m.shape_id
                 << ", habitat_id: " << m.habitat_id
                 << ", gender_rate: " << m.gender_rate
                 << ", capture_rate: " << m.capture_rate
                 << ", base_happiness: " << m.base_happiness
                 << ", is_baby: " << m.is_baby
                 << ", hatch_counter: " << m.hatch_counter
                 << ", has_gender_differences: " << m.has_gender_differences
                 << ", growth_rate_id: " << m.growth_rate_id
                 << ", forms_switchable: " << m.forms_switchable
                 << ", is_legendary: " << m.is_legendary
                 << ", is_mythical: " << m.is_mythical << ", order: " << m.order
                 << ", conquest_order: " << m.conquest_order << "]";
}

std::istream &operator>>(std::istream &i, pokemon_species_data &m) {
        std::string id;
        std::string identifier;
        std::string generation_id;
        std::string evolves_from_species_id;
        std::string evolution_chain_id;
        std::string color_id;
        std::string shape_id;
        std::string growth_rate_id;
        std::string level;
        std::string experience;
        std::string habitat_id;
        std::string gender_rate;
        std::string capture_rate;
        std::string base_happiness;
        std::string is_baby;
        std::string hatch_counter;
        std::string has_gender_differences;
        std::string forms_switchable;
        std::string is_legendary;
        std::string is_mythical;
        std::string order;
        std::string conquest_order;

        getline(i, id, ',');
        m.id = stoi_else_intmax(id);
        getline(i, identifier, ',');
        m.identifier = identifier;
        getline(i, generation_id, ',');
        m.generation_id = stoi_else_intmax(generation_id);
        getline(i, evolves_from_species_id, ',');
        m.evolves_from_species_id = stoi_else_intmax(evolves_from_species_id);
        getline(i, evolution_chain_id, ',');
        m.evolution_chain_id = stoi_else_intmax(evolution_chain_id);
        getline(i, color_id, ',');
        m.color_id = stoi_else_intmax(color_id);
        getline(i, shape_id, ',');
        m.shape_id = stoi_else_intmax(shape_id);
        getline(i, habitat_id, ',');
        m.habitat_id = stoi_else_intmax(habitat_id);
        getline(i, gender_rate, ',');
        m.gender_rate = stoi_else_intmax(gender_rate);
        getline(i, capture_rate, ',');
        m.capture_rate = stoi_else_intmax(capture_rate);
        getline(i, base_happiness, ',');
        m.base_happiness = stoi_else_intmax(base_happiness);
        getline(i, is_baby, ',');
        m.is_baby = stoi_else_intmax(is_baby);
        getline(i, hatch_counter, ',');
        m.hatch_counter = stoi_else_intmax(hatch_counter);
        getline(i, has_gender_differences, ',');
        m.has_gender_differences = stoi_else_intmax(has_gender_differences);
        getline(i, growth_rate_id, ',');
        m.growth_rate_id = stoi_else_intmax(growth_rate_id);
        getline(i, forms_switchable, ',');
        m.forms_switchable = stoi_else_intmax(forms_switchable);
        getline(i, is_legendary, ',');
        m.is_legendary = stoi_else_intmax(is_legendary);
        getline(i, is_mythical, ',');
        m.is_mythical = stoi_else_intmax(is_mythical);
        getline(i, order, ',');
        m.order = stoi_else_intmax(order);
        getline(i, conquest_order);
        m.conquest_order = stoi_else_intmax(conquest_order);

        return i;
}

std::ostream &operator<<(std::ostream &o, const experience_data &m) {
        return o << "experience_data[growth_rate_id: " << m.growth_rate_id
                 << ", level: " << m.level << ", experience: " << m.experience
                 << "]";
}

std::istream &operator>>(std::istream &i, experience_data &m) {
        std::string growth_rate_id;
        std::string level;
        std::string experience;

        getline(i, growth_rate_id, ',');
        m.growth_rate_id = stoi_else_intmax(growth_rate_id);
        getline(i, level, ',');
        m.level = stoi_else_intmax(level);
        getline(i, experience);
        m.experience = stoi_else_intmax(experience);

        return i;
}

std::ostream &operator<<(std::ostream &o, const type_names_data &m) {
        return o << "type_names_data[type_id: " << m.type_id
                 << ", local_language_id: " << m.local_language_id
                 << ", name: " << m.name << "]";
}

std::istream &operator>>(std::istream &i, type_names_data &m) {
        std::string type_id;
        std::string local_language_id;
        std::string name;

        getline(i, type_id, ',');
        m.type_id = stoi_else_intmax(type_id);
        getline(i, local_language_id, ',');
        m.local_language_id = stoi_else_intmax(local_language_id);
        getline(i, name);
        m.name = name;

        return i;
}
