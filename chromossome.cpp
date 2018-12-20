#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "./chromossome.hpp"

using namespace std;

Chromossome::Chromossome(vector <int> new_value, int bag_size, vector<vector<int>> itens):
value(new_value),
bag_size(bag_size)
{
	for (int i = 0; i < this->value.size(); ++i)
	{
		if (this->value[i]) {
			this->total_size += itens[i][0];
			this->total_benefit += itens[i][1];
		}
	}
}
int Chromossome::get_fitness() {
    if (this->total_size > this->bag_size)
        return 0;

    return (this->total_size * this->total_benefit);
}

void Chromossome::mutate(float mutation_percent) {
	float random_value = ((float)(rand() % 100)/100);

	for (int i = 0; i < this->value.size(); ++i)
	{
		if(random_value < mutation_percent) {
			if(this->value[i])
				this->value[i] = 0;
			else
				this->value[i] = 1;
		}
	}
}

void Chromossome::show_value() {
	cout << "[";
	for (int i = 0; i < this->value.size(); ++i) {
		cout << this->value[i];
		if(i != this->value.size() - 1)
			cout << ",";
	}

	cout << "]" << endl;
}

void Chromossome::show_fitness() {
	cout << "Fitness: [" << this->get_fitness() << "]" << endl;
}