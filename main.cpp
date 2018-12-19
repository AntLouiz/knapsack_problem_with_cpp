#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "./chromossome.cpp"

#ifndef POPULATION_SIZE
    #define POPULATION_SIZE 50
#endif

#ifndef MAX_ITENS
    #define MAX_ITENS 5
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


int main(int argc, char const *argv[])
{
    srand((unsigned)time(NULL));

    vector <Chromossome> population;
    vector <int> chromossome_value;
    int generation = 1;
    Chromossome best_solution;

    cout << "Generating a initial valid population..." << endl;

    while(population.size() < POPULATION_SIZE) {
        chromossome_value = generate_random_values(MAX_ITENS);
        population.push_back(Chromossome(chromossome_value));
    }

    return 0;
}