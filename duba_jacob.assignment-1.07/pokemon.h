#include <string>
#include <vector>

struct pokemon {
        int id;
        std::string identifier;
        int species_id;
        int height;
        int weight;
        int base_experience;
        int order;
        int is_default;
};

std::ostream &operator<<(std::ostream &o, const pokemon &p);

std::istream &operator>>(std::istream &i, const pokemon &p);

struct move {
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

int load_pokemon(std::vector<pokemon> &poke_list);
