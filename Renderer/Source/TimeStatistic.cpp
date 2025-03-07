#include "../Include/includes.hpp"
#include <chrono>
#include <sstream>
#include <map>

std::map<std::string, double> TimeStatistic::avg_times;
std::map<std::string, long long> TimeStatistic::cnts;
bool TimeStatistic::need_print = false;
int TimeStatistic::closed_cnt = 0;

TimeStatistic::TimeStatistic(std::string name)
	: name(name)
{
	closed_cnt++;
	start = std::chrono::steady_clock::now();
}
void TimeStatistic::print_statistic()
{
#ifdef DEBUG
	std::stringstream ss;
	ss << "*----------------------------------------------------*\n";
	ss << "|              name               |   avg time [ms]  |\n";
	
	for (auto& record : avg_times)
	{
		ss << "|---------------------------------|------------------|\n";
		ss << "|";
		for (int i = 0; i < 33; ++i)
			if (i < record.first.size())
				ss << record.first[i];
			else
				ss << " ";
		ss << "|  ";
		ss << std::fixed << std::setprecision(8) << std::setw(14);
		ss << record.second;
		ss << "  |\n";
	}
	ss << "*----------------------------------------------------*\n";
	Logs::get_instance().add(ss);
#endif // DEBUG
}
TimeStatistic::~TimeStatistic()
{
	auto end = std::chrono::steady_clock::now();
	--closed_cnt;
	double cur_time =
		std::chrono::duration_cast<std::chrono::nanoseconds>
		(end - start).count() / 1000000.0;

	double& avg = avg_times[name];
	long long& cnt = cnts[name];

	avg = avg * (cnt / (cnt + 1.0)) + cur_time / (cnt + 1.0);

	cnt++;
	if (cnt > INT64_MAX - 2)
		cnt = INT64_MAX - 2;
}
