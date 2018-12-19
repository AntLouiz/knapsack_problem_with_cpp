#include <stdio.h>
#include <vector>
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

void Chromossome::show_fitness() {
	cout << "Fitness: [" << this->get_fitness() << "]" << endl;
}