#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include "./chromossome.cpp"

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

    return *population[selected_position];
}


vector<int> generate_binary_mask(int max_itens) {
    // Generate a binary mask to make the crossover values selection
    vector<int> binary_mask;

    for (int i = 0; i < max_itens; ++i)
        binary_mask.push_back(rand() % 2);

    return binary_mask;
}


vector<Chromossome> crossover(
    Chromossome x,
    Chromossome y,
    vector<vector<int>> itens,
    int bag_size,
    float mutation_percent) {
    // Make the crossover of two Chromossomes and returns they sons in a vector

    vector<int> son1_value;
    vector<int> son2_value;

    Chromossome son1;
    Chromossome son2;

    vector<Chromossome> sons;

    vector<int> binary_mask;

    binary_mask = generate_binary_mask(itens.size());

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
    son1 = Chromossome(son1_value, bag_size, itens);
    son2 = Chromossome(son2_value, bag_size, itens);

    son1.mutate(mutation_percent);
    son2.mutate(mutation_percent);

    sons.push_back(son1);
    sons.push_back(son2);

    return sons;
}


void refresh_population(
    vector<Chromossome*> &population,
    vector<Chromossome> new_population,
    vector<vector<int>> itens,
    int bag_size){

    population.clear();

    for (int i = 0; i < new_population.size(); i++)
    {
        Chromossome* new_chromossome = new Chromossome(
            new_population[i].value,
            bag_size,
            itens
        );
        population.push_back(new_chromossome);
    }
}


int main(int argc, char const *argv[])
{
    srand((unsigned)time(NULL));

    int population_size;
    population_size = atoi(argv[2]);

    int selection_percent;
    selection_percent = atoi(argv[3]);
    
    float mutation_percent;
    mutation_percent = atoi(argv[4]);
    
    int max_iterations;
    max_iterations = atoi(argv[5]);


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

    vector <int> chromossome_value;
    Chromossome best_solution;

    int generation = 1;
    int elitism_selecteds_size;

    cout << "Generating a initial valid population..." << endl;

    while(population.size() < population_size) {
        chromossome_value = generate_random_values(max_itens);
        population.push_back(new Chromossome(chromossome_value, bag_size, itens));
    }

    while(generation <= max_iterations) {
        new_population = {};
        roulette_selected_population = {};
        elitism_selecteds_size = ((int) (population_size * selection_percent) / 100);
        bests_by_elitism = get_bests_by_elitism(population, elitism_selecteds_size);

        // Adding the bests chooses of elitism to the new population
        for (int i = 0; i < bests_by_elitism.size(); ++i)
            new_population.push_back(bests_by_elitism[i]);

        for (int i = 0; i < (population_size - elitism_selecteds_size); ++i)
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
                itens,
                bag_size,
                mutation_percent
            );

            new_population.push_back(sons[0]);
            new_population.push_back(sons[1]);
        }

        if(separated_chromossome.size())
            new_population.push_back(separated_chromossome[0]);

        best_solution = get_best(new_population);


        refresh_population(population, new_population, itens, bag_size);

        best_solution.show_fitness();
        generation++;
    }


    best_solution.show_value();

    return 0;
}