#include "../Include/includes.hpp"
#include <thread>
#include <chrono>
#include <sstream>
#include <map>

std::map<std::string, double>
            ThreadingFunctionCall::avg_times;
std::map<std::string, long long>
            ThreadingFunctionCall::cnts;
std::map<std::string, int>
            ThreadingFunctionCall::num_threads_data;
const int ThreadingFunctionCall::max_num_threads = 30;
void ThreadingFunctionCall::call(Func f, const std::string& name)
{
	if (avg_times.find(name) == avg_times.end())
	{
		avg_times[name] = 0;
		cnts[name] = 0;
	}
	double& avg = avg_times[name];
	long long& cnt = cnts[name];

	int num_threads = 8;
	if (cnt != 0)
		num_threads = std::max(1, std::min<int>(30, 
			round(2*std::log10(avg))));

	num_threads_data[name] = num_threads;
	std::vector<std::unique_ptr<std::thread>> threads(num_threads);

	auto start = std::chrono::steady_clock::now();

	for (int i = 0; i < num_threads; ++i)
		threads[i] = std::make_unique<std::thread>(f, i, num_threads);
	for (int i = 0; i < num_threads; ++i)
		threads[i]->join();

	auto end = std::chrono::steady_clock::now();
	long long cur_time =
		std::chrono::duration_cast<std::chrono::nanoseconds>
		(end - start).count();

	avg = avg * (cnt / (cnt + 1.0)) + cur_time / (cnt + 1.0);

	cnt++;
	if (cnt > 1000000)
		cnt = 1000000;
}
void ThreadingFunctionCall::print()
{
#ifdef DEBUG
	std::stringstream ss;
	for (auto& data : num_threads_data)
	{
		ss << data.first << ": " << data.second << " threads " <<
			avg_times[data.first] << "ns\n";
	}
	Logs::get_instance().add(ss);
#endif // DEBUG
}
