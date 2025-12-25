#include <future>
#include <random>
#include <vector>
#include <iostream>
#include <chrono>
#include <stdexcept>

using namespace std;

class PiCalculator {
private:
    double monteCarloPi(int points) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0.0, 1.0);

        int inside_circle = 0;

        for (int i = 0; i < points; ++i) {
            double x = dis(gen);
            double y = dis(gen);

            if (x * x + y * y <= 1.0) {
                ++inside_circle;
            }
        }

        return 4.0 * inside_circle / points;
    }

public:
    double calculatePiPortion(int total_points) {
        if (total_points <= 0) {
            throw invalid_argument("Number of points must be positive");
        }
        return monteCarloPi(total_points);
    }

    double calculatePiParallel(int total_points, int num_tasks) {
        if (total_points <= 0 || num_tasks <= 0) {
            throw invalid_argument("Points and tasks must be positive");
        }

        vector<future<double>> futures;
        int points_per_task = total_points / num_tasks;

        for (int i = 0; i < num_tasks; ++i) {
            futures.push_back(async(launch::async, &PiCalculator::monteCarloPi, this, points_per_task));
        }

        double sum = 0.0;
        for (auto& fut : futures) {
            sum += fut.get();
        }

        return sum / num_tasks;
    }
};

int main() {
    PiCalculator calculator;
    const int TOTAL_POINTS = 5000000;

    cout << "Calculating Pi with " << TOTAL_POINTS << " points\n\n";

    // Single thread
    auto start = chrono::high_resolution_clock::now();
    double pi_single = calculator.calculatePiPortion(TOTAL_POINTS);
    auto end = chrono::high_resolution_clock::now();
    auto single_time = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Single thread:\n";
    cout << "Pi = " << pi_single << "\n";
    cout << "Time = " << single_time.count() << " ms\n\n";

    // Multi-thread
    int threads = 4;

    start = chrono::high_resolution_clock::now();
    double pi_multi = calculator.calculatePiParallel(TOTAL_POINTS, threads);
    end = chrono::high_resolution_clock::now();
    auto multi_time = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << threads << " threads:\n";
    cout << "Pi = " << pi_multi << "\n";
    cout << "Time = " << multi_time.count() << " ms\n";
    cout << "Speedup = " << (double)single_time.count() / multi_time.count() << "x\n\n";

    return 0;
}