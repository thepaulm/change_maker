#include <iostream>
#include <vector>
#include <unordered_map>


bool debug = false;

/*
    :4 
     20 14  4  1
	[10 11 12 13]
	    s  e

	:14
     20 14  4  1
	[10 11 12 13]
	 s     c  e
        c  e
        e
*/

int le_index(int target, std::vector<int>v) {
	if (v.size() == 0) {
		return -1; // Error
	}

	int s = 0;
	int e = v.size() - 1;

	// early return for everybody less
	if (v[s] <= target)
		return s;

	while (true) {
		// Base case: 1 apart, at the split point
		if (e == (s + 1)) {
			if (v[e] <= target &&
				v[s] > target) {
				return e;
			}
			std::cerr << "No solution" << std::endl;
			exit(-1);
		}
		// take the half way point
		int c = int(((s + e) / 2.0) + 0.5);

		// if it's still larger, move bottom part up
		if (v[c] > target) {
			s = c;
		// else move the smaller part back
		} else {
			e = c;
		}
	}
	exit(-1);
	return -1;
}

std::shared_ptr<std::vector<int>>
r_solve(int target, std::vector<int> coins,
	    std::unordered_map<int, std::shared_ptr<std::vector<int>>>& memo) {

	// do we have a solution for target?
	auto e = memo.find(target);
	if (e != memo.end()) {
		return e->second;
	}

	// find place to start searching
	int i = le_index(target, coins);

	// is the solution just the coin?
	if (coins[i] == target) {
		std::shared_ptr<std::vector<int>> n(new std::vector<int> {target});
		auto r = memo.emplace(target, n);
		return r.first->second;
	}

	// walk them in descending order, solving for target - coins[i]
	int best_count = 0;
	int best_i = 0;
	std::shared_ptr<std::vector<int>> best;
	if (debug) {
		std::cout << "computing solution to " << target << std::endl;
	}

	for (; i < coins.size(); i++) {
		auto sol = r_solve(target - coins[i], coins, memo);
		if (best_count == 0 || sol->size() < best_count) {
			best = sol;
			best_count = sol->size();
			best_i = i;
		}
	}
	// now make final vector prepending what we built it from
	std::shared_ptr<std::vector<int>> res(new std::vector<int> {coins[best_i]});
	res->insert(res->end(), best->begin(), best->end());
	auto r = memo.emplace(target, res);
	return r.first->second;
}

std::shared_ptr<std::vector<int>>
solve(int target, std::vector<int>coins) {
	// memoize results
	std::unordered_map<int, std::shared_ptr<std::vector<int>>> memo;
	return r_solve(target, coins, memo);
}

/*
 If the coin denominations are 1,4,5,15,20 and the given amount is 23,
 this greedy algorithm gives a non-optimal solution of 20+1+1+1,
 while the optimal solution is 15+4+4.
 */

int main(int argc, char *argv[]) {
	std::string target;
	std::string csv;

	if (argc > 1 && !strcmp(argv[1], "-v"))
		debug = true;

	/* read input lines */
	std::getline(std::cin, target);
	std::getline(std::cin, csv);

	/* parse out values */
	std::vector<int> coins;
	const char *p = csv.c_str();
	for (auto e = p;; e++) {
		if (*e == ',' || *e == '\0' || std::isspace(*e)) {
			if (e != p) {
				int x = std::atoi(p);
				coins.push_back(x);
			}
			if (*e == '\0') {
				break;
			}
			p = e+1;
		}
	}

	/* Keep them sorted */
	std::sort(coins.begin(), coins.end(), std::greater<int>());

	auto s = solve(std::atoi(target.c_str()), coins);

	std::cout << "Solved:" << std::endl;
	for (auto e = s->begin(); e != s->end(); e++) {
		std::cout << *e << " ";
	}
	std::cout << std::endl;
}
