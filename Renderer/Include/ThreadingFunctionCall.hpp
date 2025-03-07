#pragma once
#include <functional>
#include <map>

typedef std::function<void(int, int)> Func;
class ThreadingFunctionCall {
public:
	static void call(Func f, const std::string& name);
	static void print();
private:
	static std::map<std::string, std::vector<double>> avg_times;
	static std::map<std::string, std::vector<long long>> cnts;
	static std::map<std::string, int> num_threads_data;
	static const int max_num_threads;
};