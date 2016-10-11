/* Missonaries and Canivals Problem
   Genetic Algorithm
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <list>
using namespace std;

struct chromesome {
	int gene[30];
	int fitness;
};

struct state {
	int m[2];
	int c[2];
	int right;
};

struct state init = { {3,0}, {3,0}, 0 };
struct state goal = { {0,3}, {0,3}, 1 };

void print_state(struct state *x)
{
	cout << "M{" << x->m[0] << "," << x->m[1] << "}, ";
	cout << "C{" << x->c[0] << "," << x->c[1] << "}, ";
	cout << x->right << endl;
}

int fitness(struct chromesome *x, bool print)
{
	int i = 0;
	x->fitness = 0;
	struct state cond = init;

	if (print) {
		cout << "M{left,right}, C{left,right}, Board[left:0, right:1]" << endl;
		print_state(&cond);
	}

	for (i = 0; i < 30; i++) {
		if (print) {
			if (!cond.right)
				cout << "-> ";
			else
				cout << "<- ";
		}

		/* movement */
		switch (x->gene[i]) {
		case 1:
			if (cond.m[cond.right] < 1)
				goto out_fit;
			cond.m[cond.right] -= 1;
			cond.m[!cond.right] += 1;
			if (print)
				cout << "M1" << endl;
			break;
		case 2:
			if (cond.m[cond.right] < 2)
				goto out_fit;
			cond.m[cond.right] -= 2;
			cond.m[!cond.right] += 2;
			if (print)
				cout << "M2" << endl;
			break;
		case 3:
			if (cond.c[cond.right] < 1)
				goto out_fit;
			cond.c[cond.right] -= 1;
			cond.c[!cond.right] += 1;
			if (print)
				cout << "C1" << endl;
			break;
		case 4:
			if (cond.c[cond.right] < 2)
				goto out_fit;
			cond.c[cond.right] -= 2;
			cond.c[!cond.right] += 2;
			if (print)
				cout << "C2" << endl;
			break;
		case 5:
			if (cond.m[cond.right] < 1 || cond.c[cond.right] < 1)
				goto out_fit;
			cond.m[cond.right] -= 1;
			cond.m[!cond.right] += 1;
			cond.c[cond.right] -= 1;
			cond.c[!cond.right] += 1;
			if (print)
				cout << "M1C1" << endl;
			break;
		}
		cond.right = !cond.right;

		if (print) {
			print_state(&cond);
		}

		/* dead */
		if ((cond.m[0] != 0 && cond.m[0] < cond.c[0]) || (cond.m[1] != 0 && cond.m[1] < cond.c[1]))
			break;

		/* found goal */
		if (cond.m[1] + cond.c[1] == 6 && cond.right) {
			x->fitness = 600;
			break;
		}

		x->fitness = (cond.m[1] + cond.c[1]) * 100 - (i * 10);
	}

out_fit:
	return x->fitness;
}

void make_child(struct chromesome *p1, struct chromesome *p2,
		struct chromesome *child)
{
	int i;

	/* crossover */
	memcpy(child->gene, p1->gene, sizeof(int)*15);
	memcpy((void *)&child->gene[15], (void *)&p2->gene[15], sizeof(int)*15);

	/* mutation */
	for (i = 0; i < 30; i++) {
		/* 20% */
		if (rand() % 5 == 0)
			child->gene[i] = (rand() % 5) + 1;
	}
}

void generate(struct chromesome *x)
{
	int i = 0;
	for (i = 0; i < 30; i++) {
		x->gene[i] = (rand() % 5) + 1;
	}
}

bool compare(const struct chromesome& first, const struct chromesome& second)
{
	if (first.fitness < second.fitness)
		return false;
	else
		return true;
}

void print_pop(list<struct chromesome> &l)
{
	int i;
	list<struct chromesome>::iterator it;

	it = l.begin();
	while (it != l.end()) {
		for (i = 0; i < 30; i++)
			cout << " " << it->gene[i];
		cout << endl;
		cout << "fitness=" << it->fitness << endl;
		it++;
	}
}

int main(void)
{
	int generation = 0;
	int i = 0, j;
	list<struct chromesome> pop;
	list<struct chromesome>::iterator it, it2;

	for (i = 0; i < 10; i++) {
		struct chromesome node;
		generate(&node);
		fitness(&node, false);
		pop.push_back(node);
	}

	//print_pop(pop);
	pop.sort(compare);
	//cout << endl;
	//print_pop(pop);

	for (i = 0; i < 100000; i++) {
		list<struct chromesome> childs;
		cout << "generation: " << i << endl;

		it = pop.begin();
		while (it != pop.end()) {
			struct chromesome child;
			list<struct chromesome>::iterator p1, p2;

			/* choose parents: best */
			p1 = it;
			p2 = ++it;
			/* make child */
			make_child(&*p1, &*p2, &child);
			if (fitness(&child, false) == 600) {
				cout << endl << "found solution!!!" << endl;
				cout << "[chromesome]" << endl;
				for (j = 0; j < 30; j++)
					cout << " " << child.gene[j];
				cout << endl << endl;
				cout << "[solution]" << endl;
				fitness(&child, true);
				goto out;
			}

			childs.push_back(child);
			it++;
		}

		it = childs.begin();
		while (it != childs.end()) {
			pop.push_back(*it);
			it++;
		}

		pop.sort(compare);
		//cout << "child " << endl << endl;
		//print_pop(pop);
		it = it2 = pop.end();
		advance(it, -6);
		it2--;

		/* drop last 5 */
		pop.erase(it, it2);
		//cout << "after erase" << endl;
		//print_pop(pop);
		//cout << "loop end" << endl;
		it = pop.begin();
		cout << "fitness:";
		while (it != pop.end()) {
			cout << " " << it->fitness;
			it++;
		}
		cout << endl;
	}

out:
	return 0;
}
