#include <string>
#include <vector>

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

std::istream &operator>>(std::istream &i, const pokemon_data &p);

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

std::istream &operator>>(std::istream &o, const move_data &m);

int load_moves(std::vector<move_data> &mv);

struct pokemon_move_data {
        int pokemon_id;
        int version_group_id;
        int move_id;
        int pokemon_move_method_id;
        int level;
        int order;
};

std::ostream &operator<<(std::ostream &o, const pokemon_move_data &pm);

std::istream &operator>>(std::istream &o, const pokemon_move_data &pm);

int load_moves(std::vector<pokemon_move_data> &pmv);
