#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include "./chromossome.cpp"

#ifndef POPULATION_SIZE
    #define POPULATION_SIZE 200
#endif

#ifndef MAX_ITENS
    #define MAX_ITENS 10
#endif

#ifndef SELECTION_PERCENT
    #define SELECTION_PERCENT 10
#endif

#ifndef BAG_SIZE
    #define BAG_SIZE 269
#endif

#ifndef MAX_ITERATIONS
    #define MAX_ITERATIONS 100
#endif

#ifndef MUTATION_PERCENT
    #define MUTATION_PERCENT 0.5
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

Chromossome get_best(vector<Chromossome> population) {
    Chromossome best;
    int best_index = 0;

    best = population[best_index];

    for (int i = 0; i < population.size(); ++i)
    {
        if(population[i].get_fitness() > best.get_fitness()) {
            best = population[i];
            best_index = i;
        }
    }

    return best;
}

Chromossome extract_best(vector<Chromossome*> &population) {
    Chromossome best;
    int best_index = 0;
    best = *population[best_index];

    for (int i = 0; i < population.size(); ++i)
    {
        if(population[i]->get_fitness() > best.get_fitness()) {
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
        best_solution = extract_best(population);
        bests.push_back(best_solution);
    }

    return bests;
}


Chromossome roulette_selection(vector<Chromossome*> &population) {
    // Applying the roulette selection and return the selected Chromossome.

    vector<int> population_fitness;
    int total_population_benefit = 0;
    int selected_position = 0;
    int random_number;

    for (int i = 0, fit = 0; i < population.size(); ++i)
    {
        fit = population[i]->get_fitness();
        population_fitness.push_back(fit);
        total_population_benefit += fit;
    }

    random_number = rand() % total_population_benefit;

    while (random_number > 0) {
        selected_position += 1;
        random_number -= population_fitness[selected_position - 1];
    }

    return *population[selected_position - 1];
}


vector<int> generate_binary_mask() {
    // Generate a binary mask to make the crossover values selection
    vector<int> binary_mask;

    for (int i = 0; i < MAX_ITENS; ++i)
        binary_mask.push_back(rand() % 2);

    return binary_mask;
}


vector<Chromossome> crossover(Chromossome x, Chromossome y, vector<vector<int>> itens) {
    // Make the crossover of two Chromossomes and returns they sons in a vector

    vector<int> son1_value;
    vector<int> son2_value;

    Chromossome son1;
    Chromossome son2;

    vector<Chromossome> sons;

    vector<int> binary_mask;

    binary_mask = generate_binary_mask();

    for (int i = 0; i < binary_mask.size(); ++i)
    {
        if(binary_mask[i]){
            son1_value.push_back(x.value[i]);
            son2_value.push_back(y.value[i]);
        }
        else {
            son2_value.push_back(x.value[i]);
            son1_value.push_back(y.value[i]);
        }
    }
    son1 = Chromossome(son1_value, BAG_SIZE, itens);
    son2 = Chromossome(son2_value, BAG_SIZE, itens);

    son1.mutate(MUTATION_PERCENT);
    son2.mutate(MUTATION_PERCENT);

    sons.push_back(son1);
    sons.push_back(son2);

    return sons;
}


void refresh_population(
    vector<Chromossome*> &population,
    vector<Chromossome> new_population,
    vector<vector<int>> itens){
    for (int i = 0; i< population.size();i++) {
         delete (population[i]);
    }

    population.clear();

    for (int i = 0; i < new_population.size(); i++)
    {
        Chromossome* new_chromossome = new Chromossome(
            new_population[i].value,
            BAG_SIZE,
            itens
        );
        population.push_back(new_chromossome);
    }
}


int main(int argc, char const *argv[])
{
    srand((unsigned)time(NULL));

    vector <Chromossome*> population;
    vector <Chromossome> bests_by_elitism;
    vector <Chromossome> roulette_selected_population;
    vector <Chromossome> separated_chromossome;
    vector <Chromossome> new_population;

    FILE *file;
    vector <vector<int>> itens;
    int weight, benefit;
    int max_itens, bag_size;

    if (!(file = fopen(argv[1], "r"))) {
        exit(1);
    }

    fscanf(file, "%d %d", &max_itens, &bag_size);

    while (fscanf(file, "%d %d", &weight, &benefit) > 0) {
        itens.push_back({weight, benefit});
    }

    cout << max_itens << endl;
    cout << bag_size << endl;
    cout << itens.size() << endl;

    exit(1);

    vector <int> chromossome_value;
    Chromossome best_solution;

    int generation = 1;
    int elitism_selecteds_size;

    cout << "Generating a initial valid population..." << endl;

    while(population.size() < POPULATION_SIZE) {
        chromossome_value = generate_random_values(MAX_ITENS);
        population.push_back(new Chromossome(chromossome_value, BAG_SIZE, itens));
    }

    while(generation <= MAX_ITERATIONS) {
        new_population = {};
        roulette_selected_population = {};
        elitism_selecteds_size = ((int) (POPULATION_SIZE * SELECTION_PERCENT) / 100);
        bests_by_elitism = get_bests_by_elitism(population, elitism_selecteds_size);

        // Adding the bests chooses of elitism to the new population
        for (int i = 0; i < bests_by_elitism.size(); ++i)
            new_population.push_back(bests_by_elitism[i]);

        for (int i = 0; i < (POPULATION_SIZE - elitism_selecteds_size); ++i)
        {
            roulette_selected_population.push_back(
                roulette_selection(population)
            );
        }

        // check if the population is impar and isolate a random chromossome
        if (roulette_selected_population.size() % 2 != 0) {
            int random_index = rand() % roulette_selected_population.size();

            separated_chromossome.push_back(
                roulette_selected_population[random_index]
            );

            roulette_selected_population.erase(
                roulette_selected_population.begin() + random_index
            );
        }

        int middle_size = roulette_selected_population.size() / 2;

        std::size_t const half_size = middle_size;

        vector<Chromossome> roulette_selected_x(
            roulette_selected_population.begin(),
            roulette_selected_population.begin() + half_size
        );

        vector<Chromossome> roulette_selected_y(
            roulette_selected_population.begin() + half_size,
            roulette_selected_population.end()
        );

        vector<Chromossome> sons;


        for (int i = 0; i < middle_size; ++i)
        {
            sons = crossover(
                roulette_selected_x[i],
                roulette_selected_y[i],
                itens
            );

            new_population.push_back(sons[0]);
            new_population.push_back(sons[1]);
        }

        if(separated_chromossome.size())
            new_population.push_back(separated_chromossome[0]);

        best_solution = get_best(new_population);


        refresh_population(population, new_population, itens);

        best_solution.show_fitness();
        generation++;
    }


    best_solution.show_value();

    return 0;
}