#include <stdio.h>
#include <vector>

#ifndef CHROMOSSOME_H
    #define CHROMOSSOME_H
#endif

using namespace std;

class Chromossome
{
    public:
        vector <int> value;
        int total_benefit = 0;
        int total_size = 0;
        int bag_size = 0;
        Chromossome(
            vector <int> value = {},
            int bag_size = 0,
            vector<vector<int>> itens = {}
        );
        int get_fitness();
        void mutate(float mutation_percent);
        void show_fitness();
        void show_value();
};