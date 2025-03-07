#pragma once
#include <sstream>

class Logs {
public:
	static Logs& get_instance();
	void add(std::stringstream& ss);
	void print();
private:
	std::stringstream stream;
};