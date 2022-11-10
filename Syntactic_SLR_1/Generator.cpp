
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


bool generator::Follow(char _nonTerminator, set<char>& result)
{
	if (!is_non_terminator(_nonTerminator))
		return false;
	if (_nonTerminator == 'S')
		result.insert('$');
	for (auto it = grammar.begin(); it != grammar.end(); ++it)
	{
		for (set<string>::iterator it_s = it->second.begin(); it_s != it->second.end(); ++it_s)
		{
			size_t pos = (*it_s).find(_nonTerminator);
			if (pos == string::npos)
				break;
			if (pos == (*it_s).length() - 1 && _nonTerminator != it->first)//出现在末尾且不在生成式左边
			{
				set<char> temp;
				if (Follow(it->first, temp))
					merge(result, temp);
			}
			else if(pos++ < (*it_s).length())
			{
				if (is_terminator((*it_s)[pos]))
					result.insert((*it_s)[pos]);
				else if (is_non_terminator((*it_s)[pos]) && (*it_s)[pos] != _nonTerminator)
				{
					set<char> temp;
					if (Follow(it->first, temp))
						merge(result, temp);
				}
			}
		}
	}

	return true;
}



bool generator::merge(set<char>& des, set<char>& src)
{
	des.insert(src.begin(), src.end());
	return true;
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

