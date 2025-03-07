#pragma once

#include <chrono>
#include <map>

class TimeStatistic {
public:
	TimeStatistic(std::string name);
	~TimeStatistic();

	static void print_statistic();
private:
	static std::map<std::string, double> avg_times;
	static std::map<std::string, long long> cnts;
	static bool need_print;
	static int closed_cnt;
	std::string name;
	std::chrono::steady_clock::time_point start;
};