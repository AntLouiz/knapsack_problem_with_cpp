#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include<algorithm>
#include "./chromossome.cpp"

#ifndef POPULATION_SIZE
    #define POPULATION_SIZE 100
#endif

#ifndef MAX_ITENS
    #define MAX_ITENS 5
#endif

#ifndef SELECTION_PERCENT
    #define SELECTION_PERCENT 30
#endif

#ifndef BAG_SIZE
    #define BAG_SIZE 80
#endif

using namespace std;

vector<int> generate_random_values(int max_itens) {
    vector<int> value;

    for (int i = 0; i < max_itens; ++i)
    {
        value.push_back((int) rand()%2);
    }

    return value;
}

Chromossome get_best(vector<Chromossome*> &population) {
    Chromossome best;
    int best_index = 0;
    best = *population[best_index];

    for (int i = 0; i < population.size(); ++i)
    {
        if(population[i]->get_fitness(BAG_SIZE) > best.get_fitness(BAG_SIZE)) {
            best = *population[i];
            best_index = i;
        }
    }

    *population.erase(population.begin() + best_index);

    return best;
}

vector<Chromossome> get_bests_by_elitism(vector<Chromossome*> &population, int elitism_value) {
    vector<Chromossome> bests;
    Chromossome best_solution;

    for (int i = 0; i < elitism_value; ++i)
    {
        best_solution = get_best(population);
        bests.push_back(best_solution);
    }

    return bests;
}


int main(int argc, char const *argv[])
{
    srand((unsigned)time(NULL));

    vector <Chromossome*> population;
    vector <Chromossome> bests_by_elitism;
    vector <int> chromossome_value;
    Chromossome best_solution;

    int generation = 1;
    int elitism_selecteds_size;

    cout << "Generating a initial valid population..." << endl;

    while(population.size() < POPULATION_SIZE) {
        chromossome_value = generate_random_values(MAX_ITENS);
        population.push_back(new Chromossome(chromossome_value));
    }

    elitism_selecteds_size = ((int) (POPULATION_SIZE * SELECTION_PERCENT) / 100);
    bests_by_elitism = get_bests_by_elitism(population, elitism_selecteds_size);

    cout << population.size() << endl;
    cout << bests.size() << endl;

    return 0;
}