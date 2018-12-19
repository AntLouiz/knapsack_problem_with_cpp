#include <stdio.h>
#include <vector>
#include "./chromossome.hpp"

using namespace std;

Chromossome::Chromossome(vector <int> new_value): value(new_value) {}
float Chromossome::get_fitness(int bag_size) {
    if (this->total_size > bag_size)
        return 0;

    return (this->total_size * this->total_benefit);
}