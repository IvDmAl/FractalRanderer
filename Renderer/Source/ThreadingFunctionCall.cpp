#include "../Include/includes.hpp"
#include <thread>
#include <chrono>
#include <sstream>
#include <map>

std::map<std::string, 
	std::vector<double>> ThreadingFunctionCall::avg_times;
std::map<std::string,
	std::vector<long long>> ThreadingFunctionCall::cnts;
std::map<std::string,
	int> ThreadingFunctionCall::num_threads_data;
const int ThreadingFunctionCall::max_num_threads = 30;
void ThreadingFunctionCall::call(Func f, const std::string& name)
{
	if (avg_times.find(name) == avg_times.end())
	{
		avg_times[name].resize(max_num_threads);
		cnts[name].resize(max_num_threads);
	}
	std::vector<double>& avg_mass = avg_times[name];
	std::vector<long long>& cnt_mass = cnts[name];

	int num_threads = 1;
	double best_avg = avg_mass[1];
	for (int i = 2; i < max_num_threads; ++i) 
		if (avg_mass[i] < best_avg)
		{
			best_avg = avg_mass[i];
			num_threads = i;
		}

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

	double& avg = avg_mass[num_threads];
	long long& cnt = cnt_mass[num_threads];

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
		ss << data.first << ": " << data.second << " threads\n";
	}
	Logs::get_instance().add(ss);
#endif // DEBUG
}
