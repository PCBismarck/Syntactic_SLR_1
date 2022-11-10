#include "Syntactic_SLR_1.h"
#ifndef DEBUG

using namespace std;

int main(int argc, char** argv)
{
	LRT table;
	if (argc > 1)
	{
		int arg = atoi(argv[1]);
		if (arg == 1 || arg == 2)
		{
			cout << "New Grammar:" << endl;
			auto it = table.D.G.generation;
			for (int i = 0; i < it.size(); ++i)
			{
				cout << i << '.' << it[i].first << "->" << it[i].second << endl;
			}
			if (arg == 2)
			{
				table.print_LRT();
			}
		}
	}

	cout << "Input note stream: " << endl;

	string input;
	cin >> input;
	vector<char> sentence;
	for (int i = 0; i < input.length(); ++i)
		sentence.push_back(input[i]);
	sentence.push_back('$');
	table.analysis(sentence);
	return 0;
}


#endif