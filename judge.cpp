#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <regex>
#include <windows.h>

using namespace std;

string strip_quotes(string str) {
	str = regex_replace(str,regex("^\"*|\"*$"),"");
	return str;
}

int main() {
	cout << "\n";

	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	string solution_file_path;
	cout << "\x1b[37m" << "Path to exe >> " << "\x1b[m";
	getline(cin,solution_file_path);
	solution_file_path = "\"" + strip_quotes(solution_file_path) + "\"";


	string test_dir_path;
	cout << "\x1b[37m" << "Path to test data dir >> " << "\x1b[m";
	getline(cin,test_dir_path);
	test_dir_path = strip_quotes(test_dir_path);

	const int n = distance(filesystem::directory_iterator(test_dir_path),filesystem::directory_iterator()) / 2;
	cout << '\n' << "\x1b[37m" << "Running " << n << " tests..." << '\n' << endl;
	vector<vector<int>> tests;
	tests.reserve(n);
	const string actual_out = test_dir_path + "\\temp.out";
	for (int i = 1; i <= n; i++) {
		vector<int> test;

		const string expected_out = test_dir_path + "\\" + to_string(i) + ".out";

		string cmd = "\"" + solution_file_path + " < \"" + test_dir_path + "\\" + to_string(i) + ".in\" > \"" + actual_out + "\"\"";
		system(cmd.c_str());

		string line_actual;
		ifstream actual_out_stream(actual_out);
		string line_expected;
		ifstream expected_out_stream(expected_out);
		int line_number = 1;
		while (getline(expected_out_stream,line_expected)) {
			try {
				getline(actual_out_stream,line_actual);
			}
			catch (exception e) {
				cout << "An unexpected error occured (there may be an incorrect amount of lines in output)";
				return 0;
			}
			if (line_actual != line_expected) {
				test.emplace_back(line_number);
			}
			line_number++;
		}
		tests.emplace_back(test);
	}
	filesystem::remove(actual_out);

	bool perfect = true;
	for (int j = 0; j < tests.size(); j++) {
		if (tests[j].empty()) {
			cout << "\x1b[32m" << "Test " << j + 1 << " passed" << endl;
		}
		else {
			perfect = false;
			cout << "\x1b[31m" << "Test " << j + 1 << " failed: " << "\x1b[37m";
			for (int line_number: tests[j]) {
				cout << line_number << " ";
			}
			cout << endl;
		}
	}
	if (perfect) {
		cout << "\x1b[32m" << "Nice!" << endl;
	}
	cout << "\x1b[m";
}