#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int get_file_path(std::string *loc, std::string file);

template <class T> int load_file(std::vector<T> &list, std::string file_name) {
        std::string loc;

        if (get_file_path(&loc, file_name)) {
                std::cout << file_name + " does not exist" << std::endl;
                return 1;
        }

        std::ifstream file(loc);

        std::string firstline;
        getline(file, firstline);

        while (file.peek() != -1) {
                T t;

                file >> t;

                list.push_back(t);
        }

        return 0;
}

struct pokemon_data {
        int id;
        std::string identifier;
        int species_id;
        int height;
        int weight;
        int base_experience;
        int order;
        int is_default;
};

std::ostream &operator<<(std::ostream &o, const pokemon_data &p);

std::istream &operator>>(std::istream &i, pokemon_data &p);

struct move_data {
        int id;
        std::string identifier;
        int generation_id;
        int type_id;
        int power;
        int pp;
        int accuracy;
        int priority;
        int target_id;
        int damage_class_id;
        int effect_id;
        int effect_chance;
        int contest_type_id;
        int contest_effect_id;
        int super_contest_effect_id;
};

std::ostream &operator<<(std::ostream &o, const move_data &m);

std::istream &operator>>(std::istream &o, move_data &m);

struct pokemon_move_data {
        int pokemon_id;
        int version_group_id;
        int move_id;
        int pokemon_move_method_id;
        int level;
        int order;
};

std::ostream &operator<<(std::ostream &o, const pokemon_move_data &m);

std::istream &operator>>(std::istream &o, pokemon_move_data &m);

struct pokemon_species_data {
        int id;
        std::string identifier;
        int generation_id;
        int evolves_from_species_id;
        int evolution_chain_id;
        int color_id;
        int shape_id;
        int habitat_id;
        int gender_rate;
        int capture_rate;
        int base_happiness;
        int is_baby;
        int hatch_counter;
        int has_gender_differences;
        int growth_rate_id;
        int forms_switchable;
        int is_legendary;
        int is_mythical;
        int order;
        int conquest_order;
};

std::ostream &operator<<(std::ostream &o, const pokemon_species_data &m);

std::istream &operator>>(std::istream &o, pokemon_species_data &m);

struct experience_data {
        int growth_rate_id;
        int level;
        int experience;
};

std::ostream &operator<<(std::ostream &o, const experience_data &m);

std::istream &operator>>(std::istream &o, experience_data &m);

struct type_names_data {
        int type_id;
        int local_language_id;
        std::string name;
};

std::ostream &operator<<(std::ostream &o, const type_names_data &m);

std::istream &operator>>(std::istream &o, type_names_data &m);

struct pokemon_stats_data {
        int pokemon_id;
        int stat_id;
        int base_stat;
        int effort;
};

std::ostream &operator<<(std::ostream &o, const pokemon_stats_data &m);

std::istream &operator>>(std::istream &o, pokemon_stats_data &m);

struct stats_data {
        int id;
        int damage_class_id;
        std::string identifier;
        int is_battle_only;
        int game_index;
};

std::ostream &operator<<(std::ostream &o, const stats_data &m);

std::istream &operator>>(std::istream &o, stats_data &m);

struct pokemon_types_data {
        int pokemon_id;
        int type_id;
        int slot;
};

std::ostream &operator<<(std::ostream &o, const pokemon_types_data &m);

std::istream &operator>>(std::istream &o, pokemon_types_data &m);

class csv_data {
      public:
        std::vector<pokemon_data> pokemon_list;
        std::vector<move_data> move_list;
        std::vector<pokemon_move_data> pokemon_move_list;
        std::vector<pokemon_species_data> pokemon_species_list;
        std::vector<experience_data> experience_list;
        std::vector<type_names_data> type_names_list;
        std::vector<pokemon_stats_data> pokemon_stats_list;
        std::vector<stats_data> stats_list;
        std::vector<pokemon_types_data> pokemon_types_list;
        csv_data() {
                load_file<pokemon_data>(pokemon_list, "pokemon.csv");
                load_file<move_data>(move_list, "moves.csv");
                load_file<pokemon_move_data>(pokemon_move_list,
                                             "pokemon_moves.csv");
                load_file<pokemon_species_data>(pokemon_species_list,
                                                "pokemon_species.csv");
                load_file<experience_data>(experience_list, "experience.csv");
                load_file<type_names_data>(type_names_list, "type_names.csv");
                load_file<pokemon_stats_data>(pokemon_stats_list,
                                              "pokemon_stats.csv");
                load_file<stats_data>(stats_list, "stats.csv");
                load_file<pokemon_types_data>(pokemon_types_list,
                                              "pokemon_types.csv");
        }
};
