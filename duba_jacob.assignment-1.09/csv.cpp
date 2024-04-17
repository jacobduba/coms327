#include "csv.h"
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

/**
 * Return int, unless data is empty. Then return max
 */
int stoi_emax(std::string s) {
        if (s == "") {
                return INT_MAX;
        }

        return std::stoi(s);
}

std::string itos(int s) {
        if (s == INT_MAX) {
                return "Empty";
        }

        return std::to_string(s);
}

std::ostream &operator<<(std::ostream &o, const pokemon_data &m) {
        return o << "pokemon_data[id: " << itos(m.id)
                 << ", identifier: " << m.identifier
                 << ", species_id: " << itos(m.species_id)
                 << ", height: " << itos(m.height)
                 << ", weight: " << itos(m.weight)
                 << ", base_experience: " << itos(m.base_experience)
                 << ", order: " << itos(m.order)
                 << ", is_default: " << itos(m.is_default) << "]";
}

std::istream &operator>>(std::istream &i, pokemon_data &m) {
        std::string id;
        std::string identifier;
        std::string species_id;
        std::string height;
        std::string weight;
        std::string base_experience;
        std::string order;
        std::string is_default;

        getline(i, id, ',');
        m.id = stoi_emax(id);
        getline(i, identifier, ',');
        m.identifier = identifier;
        getline(i, species_id, ',');
        m.species_id = stoi_emax(species_id);
        getline(i, height, ',');
        m.height = stoi_emax(height);
        getline(i, weight, ',');
        m.weight = stoi_emax(weight);
        getline(i, base_experience, ',');
        m.base_experience = stoi_emax(base_experience);
        getline(i, order, ',');
        m.order = stoi_emax(order);
        getline(i, is_default);
        m.is_default = stoi_emax(is_default);

        return i;
}

std::ostream &operator<<(std::ostream &o, const move_data &m) {
        return o << "move_data[id: " << itos(m.id)
                 << ", identifier: " << m.identifier
                 << ", generation_id: " << itos(m.generation_id)
                 << ", type_id: " << itos(m.type_id)
                 << ", power: " << itos(m.power) << ", pp: " << itos(m.pp)
                 << ", accuracy: " << itos(m.accuracy)
                 << ", priority: " << itos(m.priority)
                 << ", target_id: " << itos(m.target_id)
                 << ", damage_class_id: " << itos(m.damage_class_id)
                 << ", effect_id: " << itos(m.effect_id)
                 << ", effect_chance: " << itos(m.effect_chance)
                 << ", contest_type_id: " << itos(m.contest_type_id)
                 << ", contest_effect_id: " << itos(m.contest_effect_id)
                 << ", super_contest_effect_id: "
                 << itos(m.super_contest_effect_id) << "]";
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
        m.id = stoi_emax(id);
        getline(i, identifier, ',');
        m.identifier = identifier;
        getline(i, generation_id, ',');
        m.generation_id = stoi_emax(generation_id);
        getline(i, type_id, ',');
        m.type_id = stoi_emax(type_id);
        getline(i, power, ',');
        m.power = stoi_emax(power);
        getline(i, pp, ',');
        m.pp = stoi_emax(pp);
        getline(i, accuracy, ',');
        m.accuracy = stoi_emax(accuracy);
        getline(i, priority, ',');
        m.priority = stoi_emax(priority);
        getline(i, target_id, ',');
        m.target_id = stoi_emax(target_id);
        getline(i, damage_class_id, ',');
        m.damage_class_id = stoi_emax(damage_class_id);
        getline(i, effect_id, ',');
        m.effect_id = stoi_emax(effect_id);
        getline(i, effect_chance, ',');
        m.effect_chance = stoi_emax(effect_chance);
        getline(i, contest_type_id, ',');
        m.contest_type_id = stoi_emax(contest_type_id);
        getline(i, contest_effect_id, ',');
        m.contest_effect_id = stoi_emax(contest_effect_id);
        getline(i, super_contest_effect_id);
        m.super_contest_effect_id = stoi_emax(super_contest_effect_id);

        return i;
}

std::ostream &operator<<(std::ostream &o, const pokemon_move_data &m) {
        return o << "pokemon_move_data[pokemon_id: " << itos(m.pokemon_id)
                 << ", version_group_id: " << itos(m.version_group_id)
                 << ", move_id: " << itos(m.move_id)
                 << ", pokemon_move_method_id: "
                 << itos(m.pokemon_move_method_id)
                 << ", level: " << itos(m.level) << ", order: " << itos(m.order)
                 << "]";
}

std::istream &operator>>(std::istream &i, pokemon_move_data &m) {
        std::string pokemon_id;
        std::string version_group_id;
        std::string move_id;
        std::string pokemon_move_method_id;
        std::string level;
        std::string order;

        getline(i, pokemon_id, ',');
        m.pokemon_id = stoi_emax(pokemon_id);
        getline(i, version_group_id, ',');
        m.version_group_id = stoi_emax(version_group_id);
        getline(i, move_id, ',');
        m.move_id = stoi_emax(move_id);
        getline(i, pokemon_move_method_id, ',');
        m.pokemon_move_method_id = stoi_emax(pokemon_move_method_id);
        getline(i, level, ',');
        m.level = stoi_emax(level);
        getline(i, order);
        m.order = stoi_emax(order);

        return i;
}

std::ostream &operator<<(std::ostream &o, const pokemon_species_data &m) {
        return o << "pokemon_species_data[id: " << itos(m.id)
                 << ", identifier: " << m.identifier
                 << ", generation_id: " << itos(m.generation_id)
                 << ", evolves_from_species_id: "
                 << itos(m.evolves_from_species_id)
                 << ", evolution_chain_id: " << itos(m.evolution_chain_id)
                 << ", color_id: " << itos(m.color_id)
                 << ", shape_id: " << itos(m.shape_id)
                 << ", habitat_id: " << itos(m.habitat_id)
                 << ", gender_rate: " << itos(m.gender_rate)
                 << ", capture_rate: " << itos(m.capture_rate)
                 << ", base_happiness: " << itos(m.base_happiness)
                 << ", is_baby: " << itos(m.is_baby)
                 << ", hatch_counter: " << itos(m.hatch_counter)
                 << ", has_gender_differences: "
                 << itos(m.has_gender_differences)
                 << ", growth_rate_id: " << itos(m.growth_rate_id)
                 << ", forms_switchable: " << itos(m.forms_switchable)
                 << ", is_legendary: " << itos(m.is_legendary)
                 << ", is_mythical: " << itos(m.is_mythical)
                 << ", order: " << itos(m.order)
                 << ", conquest_order: " << itos(m.conquest_order) << "]";
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
        m.id = stoi_emax(id);
        getline(i, identifier, ',');
        m.identifier = identifier;
        getline(i, generation_id, ',');
        m.generation_id = stoi_emax(generation_id);
        getline(i, evolves_from_species_id, ',');
        m.evolves_from_species_id = stoi_emax(evolves_from_species_id);
        getline(i, evolution_chain_id, ',');
        m.evolution_chain_id = stoi_emax(evolution_chain_id);
        getline(i, color_id, ',');
        m.color_id = stoi_emax(color_id);
        getline(i, shape_id, ',');
        m.shape_id = stoi_emax(shape_id);
        getline(i, habitat_id, ',');
        m.habitat_id = stoi_emax(habitat_id);
        getline(i, gender_rate, ',');
        m.gender_rate = stoi_emax(gender_rate);
        getline(i, capture_rate, ',');
        m.capture_rate = stoi_emax(capture_rate);
        getline(i, base_happiness, ',');
        m.base_happiness = stoi_emax(base_happiness);
        getline(i, is_baby, ',');
        m.is_baby = stoi_emax(is_baby);
        getline(i, hatch_counter, ',');
        m.hatch_counter = stoi_emax(hatch_counter);
        getline(i, has_gender_differences, ',');
        m.has_gender_differences = stoi_emax(has_gender_differences);
        getline(i, growth_rate_id, ',');
        m.growth_rate_id = stoi_emax(growth_rate_id);
        getline(i, forms_switchable, ',');
        m.forms_switchable = stoi_emax(forms_switchable);
        getline(i, is_legendary, ',');
        m.is_legendary = stoi_emax(is_legendary);
        getline(i, is_mythical, ',');
        m.is_mythical = stoi_emax(is_mythical);
        getline(i, order, ',');
        m.order = stoi_emax(order);
        getline(i, conquest_order);
        m.conquest_order = stoi_emax(conquest_order);

        return i;
}

std::ostream &operator<<(std::ostream &o, const experience_data &m) {
        return o << "experience_data[growth_rate_id: " << itos(m.growth_rate_id)
                 << ", level: " << itos(m.level)
                 << ", experience: " << itos(m.experience) << "]";
}

std::istream &operator>>(std::istream &i, experience_data &m) {
        std::string growth_rate_id;
        std::string level;
        std::string experience;

        getline(i, growth_rate_id, ',');
        m.growth_rate_id = stoi_emax(growth_rate_id);
        getline(i, level, ',');
        m.level = stoi_emax(level);
        getline(i, experience);
        m.experience = stoi_emax(experience);

        return i;
}

std::ostream &operator<<(std::ostream &o, const type_names_data &m) {
        return o << "type_names_data[type_id: " << itos(m.type_id)
                 << ", local_language_id: " << itos(m.local_language_id)
                 << ", name: " << m.name << "]";
}

std::istream &operator>>(std::istream &i, type_names_data &m) {
        std::string type_id;
        std::string local_language_id;
        std::string name;

        getline(i, type_id, ',');
        m.type_id = stoi_emax(type_id);
        getline(i, local_language_id, ',');
        m.local_language_id = stoi_emax(local_language_id);
        getline(i, name);
        m.name = name;

        return i;
}

std::ostream &operator<<(std::ostream &o, const pokemon_stats_data &m) {
        return o << "pokemon_stats_data[pokemon_id: " << itos(m.pokemon_id)
                 << ", stat_id: " << itos(m.stat_id)
                 << ", base_stat: " << itos(m.base_stat)
                 << ", effort: " << itos(m.effort) << "]";
}

std::istream &operator>>(std::istream &i, pokemon_stats_data &m) {
        std::string pokemon_id;
        std::string stat_id;
        std::string base_stat;
        std::string effort;

        getline(i, pokemon_id, ',');
        m.pokemon_id = stoi_emax(pokemon_id);
        getline(i, stat_id, ',');
        m.stat_id = stoi_emax(stat_id);
        getline(i, base_stat, ',');
        m.base_stat = stoi_emax(base_stat);
        getline(i, effort);
        m.effort = stoi_emax(effort);

        return i;
}

std::ostream &operator<<(std::ostream &o, const stats_data &m) {
        return o << "stats_data[id: " << itos(m.id)
                 << ", damage_class_id: " << itos(m.damage_class_id)
                 << ", identifier: " << m.identifier
                 << ", is_battle_only: " << itos(m.is_battle_only)
                 << ", game_index: " << itos(m.game_index) << "]";
}

std::istream &operator>>(std::istream &i, stats_data &m) {
        std::string id;
        std::string damage_class_id;
        std::string identifier;
        std::string is_battle_only;
        std::string game_index;

        getline(i, id, ',');
        m.id = stoi_emax(id);
        getline(i, damage_class_id, ',');
        m.damage_class_id = stoi_emax(damage_class_id);
        getline(i, identifier, ',');
        m.identifier = identifier;
        getline(i, is_battle_only, ',');
        m.is_battle_only = stoi_emax(is_battle_only);
        getline(i, game_index);
        m.game_index = stoi_emax(game_index);

        return i;
}

std::ostream &operator<<(std::ostream &o, const pokemon_types_data &m) {
        return o << "pokemon_types_data[pokemon_id: " << itos(m.pokemon_id)
                 << ", type_id: " << itos(m.type_id)
                 << ", slot: " << itos(m.slot) << "]";
}

std::istream &operator>>(std::istream &i, pokemon_types_data &m) {
        std::string pokemon_id;
        std::string type_id;
        std::string slot;

        getline(i, pokemon_id, ',');
        m.pokemon_id = stoi_emax(pokemon_id);
        getline(i, type_id, ',');
        m.type_id = stoi_emax(type_id);
        getline(i, slot);
        m.slot = stoi_emax(slot);

        return i;
}
