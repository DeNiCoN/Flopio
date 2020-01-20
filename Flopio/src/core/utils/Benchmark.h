#include <chrono>
#include <functional>

class Benchmark
{
    std::chrono::high_resolution_clock::duration time_passed;
    std::chrono::high_resolution_clock::time_point start;
    int* passed;

public:
    Benchmark(int* passed): passed(passed)
    {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Benchmark()
    {
        time_passed = std::chrono::high_resolution_clock::now() - start;
        *passed = time_passed.count();
    }
};

class CallbackBenchmark
{
    std::chrono::high_resolution_clock::duration time_passed;
    std::chrono::high_resolution_clock::time_point start;
    std::function<void(std::chrono::high_resolution_clock::duration)> callback;

public:
    CallbackBenchmark(std::function<void(std::chrono::high_resolution_clock::duration)> callback) : callback(callback)
    {
        start = std::chrono::high_resolution_clock::now();
    }

    ~CallbackBenchmark()
    {
        time_passed = std::chrono::high_resolution_clock::now() - start;
        callback(time_passed);
    }
};
