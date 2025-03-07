#include "../Include/includes.hpp"
#include <iostream>
#include <sstream>

Logs& Logs::get_instance()
{
	static Logs logs;
	return logs;
}
void Logs::add(std::stringstream& ss)
{
	stream << ss.str();
}
void Logs::print()
{
#ifdef DEBUG
	std::cout << stream.str();
	stream = std::stringstream();
#endif // DEBUG
}