// Syntactic_SLR_1.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#ifndef SYNTACTIC_H
#define SYNTACTIC_H

#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include "Generator.h"


//#define DEBUG

using namespace std;

class node
{
public:
	string get_branch_token();//根据node的branch返回由节点的branch生成的一个token

public:
	int id;//节点的id
	unordered_map<char, vector<pair<string, int>>> Generation;//节点包含的产生式
	//string  E>E+T
	vector<pair<string, int>> branch_;
	unordered_map<char, string> jump;

};

class DFA
{
public:
	bool closure(char nonTerminator, vector<pair<char, string>>& result);
	bool create_node_info(node& to_create, vector<pair<string, int>> &branch);
	
	//获取节点I对于给定的字符note的下一个状态的branch生成式
	bool get_node_branch(char note, vector<pair<string, int>>& result, node &I);
public:
	DFA();
	vector<node> state;
	unordered_map<string, int> StateID;//通过一个节点的branch获取其节点id
	generator G;
};

class LRT
{
public:
	LRT();

	void analysis(vector<char> input);

	void print_LRT();

	string get_next_state(int state, char note);

	bool print_stack(auto stk);
	bool print_input(auto input, int pos);
public:
	unordered_map<string, string> pat;
	DFA D;
	vector<char> note_stk;
	vector<int> state_stk;

};





#endif
// TODO: 在此处引用程序需要的其他标头。
