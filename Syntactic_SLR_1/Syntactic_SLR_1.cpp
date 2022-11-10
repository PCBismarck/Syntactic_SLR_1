// Syntactic_SLR_1.cpp: 定义应用程序的入口点。
//

#include "Syntactic_SLR_1.h"

using namespace std;

#ifdef DEBUG



int main()
{
	LRT lrt;
	auto it = lrt.D.G.generation;
	for (int i = 0 ; i < it.size(); ++i)
	{
		cout << i << '.' << it[i].first << "->" << it[i].second << endl;
	}
	
	vector<pair<char, string>> result;
	//D.closure('E', result);
	set<char> f_result;
	generator g;
	g.Follow('T', f_result);
	lrt.D.closure('T', result);
	lrt.print_LRT();
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
					state[cnt].jump[*it] = new_node.id;
				}
				else
					state[cnt].jump[*it] = StateID[new_node.get_branch_token()];
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
	for (auto it = branch.begin(); it != branch.end(); ++it)//把branch中的项加入，同时找出其他的非终结符
	{
		int pos = it->second + 2;
		if (it->first.length() > pos && is_non_terminator(it->first[pos]))
			left_nonTerminator.insert(it->first[pos]);
		else if (it->first.length() == pos)//为归约项
		{
			//在跳转表中增加归约的式子序号

			to_create.jump['$'] = G.get_relation(it->first[0], it->first.substr(2));
		}
		to_create.Generation[it->first[0]].push_back(pair<string, int>((it->first).substr(2), it->second));
	}
	//对找到的非终结符求闭包，加入到集合中
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
			if (it_g->first.length() > it_g->second && (it_g->first)[it_g->second] == note)
			{
				string key;
				key.push_back(map_g->first);
				key.push_back('>');
				key.append(it_g->first);
				result.push_back(pair<string, int>(key, it_g->second + 1));//小圆点位置右移一位
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

LRT::LRT()
{
	//为每个节点构建一行跳转
	for (int i = 0; i < D.state.size(); ++i)
	{
		for (auto it = D.state[i].jump.begin(); it != D.state[i].jump.end(); ++it)
		{
			string key = to_string(i) + it->first;
			pat[key] = it->second;
		}
	}
}

void LRT::analysis(vector<char> input)
{

}

void LRT::print_LRT()
{
	cout << "LRT:";
	vector<char> head;
	for (auto it = get_terminator().begin(); it != get_terminator().end(); ++it)
		head.push_back(*it);
	head.push_back('$');
	for (auto it = get_non_terminator().begin(); it != get_non_terminator().end(); ++it)
		if (*it != 'S')
			head.push_back(*it);

	string divide(head.size() * 9, '-');
	cout << endl << divide << endl << "|";

	for (auto it = head.begin(); it != head.end(); ++it)
		cout << "\t|" << "   " << *it ;
	
	cout << "\t|" << endl << divide << endl;

	for (int i = 0; i < D.state.size(); ++i)
	{
		cout<< "|" << "   " << i << "\t|";
		for (auto it = head.begin(); it != head.end(); ++it)
		{
			int state = get_next_state(i, *it);
			if (state == -1)
				cout << "  " << "ACC" << "\t|";
			else if (state == 0)
				cout << "\t|";
			else
			{
				if (is_terminator(*it))
					cout << "   S";
				else if (*it == '$')
					cout << "   R";
				else
					cout << "   ";
				cout << state << "\t|";

			}
		}
		cout << endl << divide << endl;
	}
}

int LRT::get_next_state(int state, char note)
{
		return pat[to_string(state) + note];
}
