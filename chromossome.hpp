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
        Chromossome(vector <int> value = {}, vector<vector<int>> itens = {});
        float get_fitness(int bag_size);
};