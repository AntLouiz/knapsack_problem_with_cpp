#include <stdio.h>
#include <vector>
#include "./chromossome.hpp"

using namespace std;

Chromossome::Chromossome(vector <int> new_value): value(new_value) {
	vector<vector<int>> itens = {
		{1, 3},
		{5, 10},
		{3, 80},
		{7, 10},
		{9, 30}
	};

	for (int i = 0; i < this->value.size(); ++i)
	{
		if (this->value[i]) {
			this->total_size += itens[i][0];
			this->total_benefit += itens[i][1];
		}
	}
}
float Chromossome::get_fitness(int bag_size) {
    if (this->total_size > bag_size)
        return 0;

    return (this->total_size * this->total_benefit);
}