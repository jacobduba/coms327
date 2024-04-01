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

int load_pokemon(std::vector<pokemon> &poke_list);
