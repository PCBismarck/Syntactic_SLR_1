// Syntactic_SLR_1.cpp: 定义应用程序的入口点。
//

#include "Syntactic_SLR_1.h"

using namespace std;

#ifdef DEBUG



int main()
{
	generator G;
	auto it = G.generation;
	for (int i = 0 ; i < it.size(); ++i)
	{
		cout << i << '.' << it[i].first << "->" << it[i].second << endl;
	}
	DFA D;
	vector<pair<char, string>> result;
	//D.closure('E', result);
	D.closure('T', result);
	return 0;
}

#endif // DEBUG

DFA::DFA()
{
	//初始化开始条件
	node I;
	I.id = state.size();
	I.branch_.push_back(pair<string, int>("e>E", 0));
	state.push_back(I);
	StateID[I.get_branch_token()] = 0;
	int len = state.size();
	for (int cnt = 0; cnt < state.size(); ++cnt)
	{
		//利用节点的branch生成节点信息
		create_node_info(state[cnt], state[cnt].branch_);
			
		//生成下一个节点的branch
		set<char> noteSet = get_non_terminator();
		noteSet.insert(get_terminator().begin(), get_terminator().end());
		for (auto it = noteSet.begin(); it != noteSet.end(); ++it)
		{
			vector<pair<string, int>> branch_temp;
			get_node_branch(*it, branch_temp, state[cnt]);
			if (!branch_temp.empty())
			{
				node new_node;
				new_node.id = state.size();
				new_node.branch_ = branch_temp;
				//如果branch对照表中没有同样的branch，则创建由该branch生成的节点
				if (StateID[new_node.get_branch_token()] == NULL)
				{
					state.push_back(new_node);
					StateID[new_node.get_branch_token()] = new_node.id;
				}
			}
		}
	}
}

bool DFA::closure(char nonTerminator, vector<pair<char, string>>& result)
{
	if (!is_non_terminator(nonTerminator))
		return false;
	//获取当前非终结符对应的所有产生式
	set<string> generation = G.grammar[nonTerminator];
	set<char> other_nonTerminator;
	for (auto it = generation.begin(); it != generation.end(); ++it)
	{
		result.push_back(pair<char, string>(nonTerminator, (*it)));
		if (is_non_terminator((*it)[0]) && (*it)[0] != nonTerminator)//产生式中可以推导出其他非终结符
			other_nonTerminator.insert((*it)[0]);
	}
	for (auto it = other_nonTerminator.begin(); it != other_nonTerminator.end(); ++it)
	{
		vector<pair<char, string>> to_add;
		closure((*it), to_add);
		for (auto it = to_add.begin(); it != to_add.end(); ++it)
			result.push_back((*it));
	}
	return true;
}

bool DFA::create_node_info(node& to_create, vector<pair<string, int>>& branch)
{
	set<char> left_nonTerminator;
	for (auto it = branch.begin(); it != branch.end(); ++it)
	{
		int pos = it->second + 2;
		if (is_non_terminator(it->first[pos]))
			left_nonTerminator.insert(it->first[pos]);
		to_create.Generation[it->first[0]].push_back(pair<string, int>((it->first).substr(2), it->second));
	}

	for (auto it = left_nonTerminator.begin(); it != left_nonTerminator.end(); ++it)
	{
		vector<pair<char, string>> result;
		closure((*it), result);
		for (auto it_r = result.begin(); it_r != result.end(); ++it_r)
			to_create.Generation[it_r->first].push_back(pair<string, int>(it_r->second, 0));
	}
	return true;
}

bool DFA::get_node_branch(char note, vector<pair<string, int>>& result, node &I)
{
	for (auto map_g = I.Generation.begin(); map_g != I.Generation.end(); ++map_g)
	{
		for (auto it_g = map_g->second.begin(); it_g != map_g->second.end(); ++it_g)//该非终结符的每个产生式
		{
			if ((it_g->first)[it_g->second] == note)
			{
				string key;
				key.push_back(map_g->first);
				key.push_back('>');
				key.append(it_g->first);
				result.push_back(pair<string, int>(key, it_g->second + 1));
			}
		}
	}
	return true;
}

std::string node::get_branch_token()
{
	string token;
	for (auto it = branch_.begin(); it != branch_.end(); ++it)
		token.append(it->first).append(to_string(it->second));
	return token;
}
