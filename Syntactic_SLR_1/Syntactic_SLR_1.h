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


#define DEBUG

using namespace std;

class node
{
public:
	string get_branch_token();//根据node的branch返回由节点的branch生成的一个token

public:
	int id;
	unordered_map<char, vector<pair<string, int>>> Generation;
	//set<string> branch;//string  E>E+T
	vector<pair<string, int>> branch_;

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

//class LRT
//{
//public:
//	
//};





#endif
// TODO: 在此处引用程序需要的其他标头。
