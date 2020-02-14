#pragma once

#include <vector>
#include <iostream>
#define LOG

struct interval_map
{
	std::vector<uint16_t> keys;
	std::vector<uint16_t> values;

	interval_map() {
		keys.push_back(0);
		values.push_back(0);
	}

	int lowDico(uint16_t key, int pos = 0) {
		int iStart = pos;
		int iEnd = keys.size() - 1;

		while (iStart < iEnd)
		{
			// Calcul de la position du milieu
			int iMilieu = (iStart + iEnd) / 2;

			if (keys[iMilieu] == key)
				break;
			else
			{
				// Si la valeur recherchée est plus petite
				// que la valeur du l'élément du milieu
				// Alors on regarde le sous-tableau de gauche
				if (keys[iMilieu] > key) iEnd = iMilieu - 1;
				// sinon on regarde le sous-tableau de droite
				else iStart = iMilieu + 1;
			}

		}
		if (iStart == iEnd && key < keys[iStart]) {
			return iStart - 1;
		}

		return iStart;
	}

	int low(uint16_t key, int pos = 0) {
		return lowDico(key, pos);
		while (pos < keys.size() && keys[pos] <= key)
		{
			pos++;
		}
		return pos - 1;
	}

	void reserve(int size)
	{
		keys.reserve(size);
		values.reserve(size);
	}

	void insert(uint16_t min, uint16_t max, uint16_t value) {
		
		// std::cout << "\n----- start ------" << std::endl;

		struct BreakNode {
			int pos;
			uint16_t key;
			uint16_t value;
			uint16_t len;
		};

		BreakNode avant;
		BreakNode apres;
		avant.pos = low(min);
		apres.pos = low(max + 1, avant.pos);
		int suppression = apres.pos - avant.pos;

		if (avant.pos != -1) {
			avant.key = keys[avant.pos];
			avant.value = values[avant.pos];
		}
		if (apres.pos != -1) {
			apres.key = keys[apres.pos];
			apres.value = values[apres.pos];
			if (apres.pos + 1 < keys.size())
			apres.len = keys[apres.pos + 1] - keys[apres.pos];
			else
			apres.len = 1000;
		}
		// std::cout << "insert " << " [" << min << "," << max << "] " << value << "\n"
		// 		<< "\tavant:" << avant.pos << " [key " << avant.key  << " => value " << avant.value << "]\n" 
		// 		<< "\tapres:" << apres.pos << " [key " << apres.key  << " => value " << apres.value << "] len=" << apres.len << "\n";
		// std::cout << "Suppression " << suppression << std::endl;
		// std::cout << "init: "; map_display();

		// ajout de l'intervale avant
		// si c'est au debut ou
		// si il y a un un interval avant on regarde si il est bien different
		int pos = avant.pos;
		if (avant.pos == -1 || avant.value != value || apres.pos == avant.pos)
		{
			if (avant.key == min)
			{
				values[avant.pos] = value;
			}
			else
			{
				apres.pos++;
				keys.insert(keys.begin() + avant.pos + 1, min);
				values.insert(values.begin() + avant.pos + 1, value);
				pos++;
			}
		}

		// suppression
		// std::cout << "ajou: "; map_display();
		if (suppression > 1)
		{
			pos++;
			keys.erase(keys.begin() + pos, keys.begin() + pos + suppression);
			values.erase(values.begin() + pos, values.begin() + pos + suppression);
			apres.pos -= suppression;
		}
		// std::cout << "supp: "; map_display();

		if (suppression > 1 || (apres.value != value && apres.key != max + 1))
		{
			if (suppression == 1)
			{
				keys[apres.pos] = max + 1;
				values[apres.pos] = apres.value;
			}
			else
			{
				keys.insert(keys.begin() + apres.pos + 1, max + 1);
				values.insert(values.begin() + apres.pos + 1, apres.value);
			}
		}

		// std::cout << "fini: "; map_display();
	}

	void print_map() {
		for (int i = 0; i < keys.size(); i++)
		{
			std::cout << "[" << keys[i] << ":" << values[i] << "] ";
		} 
		std::cout << std::endl;
	}

	uint16_t get(uint16_t key) {
		return values[low(key)];
	}

	void getData(uint16_t *blocks, int len)
	{
		int i = 0;
		while (i < len)
		{
			blocks[i] = get(i);
			i++;
		}
	}

	void parsing_array(std::vector<uint16_t> datas)
	{
		int start = 0;
		uint16_t value = 0;
		int min = -1;
		for (auto data : datas)
		{
			if (min > -1 && value != data)
			{
				insert(min, start - 1, value);
				min = -1;
			}

			if (min == -1)
			{
				min = start;
				value = data;
			}
			start++;
		}

		if (min > -1)
		{
			insert(min, datas.size() - 1, value);
		}
	}
};