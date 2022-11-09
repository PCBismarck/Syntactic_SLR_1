#ifndef GENERATOR_H
#define GENERATOR_H

#include <unordered_map>
#include <set>
#include <iostream>
#include <string>
#include <utility>

using namespace std;

//写入生成式

bool is_terminator(char id);

bool is_non_terminator(char id);

set<char>& get_non_terminator();

set<char>& get_terminator();

class generator
{
public:
	generator();
	unordered_map<char, set<string>> grammar;
	vector<pair<char, string>> generation;
};

#endif // !GENERTATOR_H