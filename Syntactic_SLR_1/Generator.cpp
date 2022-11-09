
#include "Generator.h"



set<char> terminator = {
	'+',	'-',	'*',	'/',	'(',	')',	'd'
};

set<char> nonTerminator = {
	'E',	'T',	'F'
};


set<string> E = {
	"E+T", "E-T", "T"
};


set<string>	T = {
	"T*F", "T/F", "F"
};

set<string> F = {
	"(E)",	"d"
};

unordered_map<char, set<string>> pre_generator = {
	{'E', E},
	{'T', T},
	{'F', F}
};


bool is_terminator(char id)
{
	return terminator.count(id);
}

bool is_non_terminator(char id)
{
	return nonTerminator.count(id);
}

std::set<char>& get_non_terminator()
{
	return nonTerminator;
}

std::set<char>& get_terminator()
{
	return terminator;
}

int generator::get_relation(char _nonTerminator, string _generation)
{
	for (int seq = 0; seq < generation.size(); ++seq)
	{
		if (generation[seq].first == _nonTerminator && generation[seq].second == _generation)
		{
			return seq;
		}
	}
	return -1;
}

generator::generator()
{
	nonTerminator.insert('S');
	set<string> start;
	start.insert("E");
	grammar['S'] = start;

	for (auto it = pre_generator.begin(); it != pre_generator.end(); ++it)
		grammar[it->first] = it->second;

	generation.push_back(pair<char, string>('S', "E"));

	for (auto it = pre_generator.begin(); it != pre_generator.end(); ++it)
		for (auto it_g = it->second.begin(); it_g != it->second.end(); ++it_g)
			generation.push_back(pair<char, string>(it->first, *it_g));
}

