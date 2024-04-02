#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int get_file_path(std::string *loc, std::string file);

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

int load_pokemon(std::vector<pokemon_data> &pv);

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

template <class T> int load_file(std::vector<T> &list, std::string file_name) {
        std::string loc;

        if (get_file_path(&loc, file_name)) {
                std::cout << file_name + " does not exist" << std::endl;
                return 1;
        }

        std::ifstream file(loc.c_str());

        std::string firstline;
        getline(file, firstline);

        while (file.peek() != -1) {
                T t;

                file >> t;

                list.push_back(t);
        }

        return 0;
}
