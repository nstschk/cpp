#include <vector>
#include <thread>
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>

using namespace std;

class ImageProcessor {
private:
    vector<vector<int>> image;
    vector<vector<int>> result;
    int width, height;

    void processStrip(int start_row, int end_row) {
        for (int y = start_row; y < end_row; ++y) {
            for (int x = 0; x < width; ++x) {
                result[y][x] = blurPixel(x, y);
            }
        }
    }

public:
    ImageProcessor(int w, int h) : width(w), height(h) {
        image.resize(height, vector<int>(width));
        result.resize(height, vector<int>(width));

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, 255);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                image[y][x] = dist(gen);
            }
        }
    }

    void applyFilterSingleThread() {
        auto start = chrono::high_resolution_clock::now();

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                result[y][x] = blurPixel(x, y);
            }
        }

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Single thread time: " << duration.count() << " ms\n";
    }

    void applyFilterMultiThread(int num_threads) {
        auto start = chrono::high_resolution_clock::now();

        vector<thread> threads;
        int rows_per_thread = height / num_threads;

        for (int i = 0; i < num_threads; ++i) {
            int start_row = i * rows_per_thread;
            int end_row = (i == num_threads - 1) ? height : start_row + rows_per_thread;

            threads.emplace_back(&ImageProcessor::processStrip, this, start_row, end_row);
        }

        for (auto& t : threads) {
            t.join();
        }

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << num_threads << " threads time: " << duration.count() << " ms\n";
    }

    int blurPixel(int x, int y) {
        int sum = 0;
        int count = 0;

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                int nx = x + dx;
                int ny = y + dy;

                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    sum += image[ny][nx];
                    count++;
                }
            }
        }

        return (count > 0) ? sum / count : image[y][x];
    }

    void printStats() {
        int min_val = 255, max_val = 0, sum = 0;

        for (const auto& row : result) {
            for (int val : row) {
                min_val = min(min_val, val);
                max_val = max(max_val, val);
                sum += val;
            }
        }

        cout << "Min brightness: " << min_val << "\n";
        cout << "Max brightness: " << max_val << "\n";
        cout << "Avg brightness: " << sum / (width * height) << "\n";
    }
};

int main() {
    const int WIDTH = 1000;
    const int HEIGHT = 1000;

    ImageProcessor processor(WIDTH, HEIGHT);

    cout << "=== Image Processing Test ===\n";
    cout << "Image size: " << WIDTH << "x" << HEIGHT << "\n\n";

    cout << "Single thread processing:\n";
    processor.applyFilterSingleThread();
    processor.printStats();

    cout << "\nMulti-thread processing:\n";


    processor.applyFilterMultiThread(4);
    processor.printStats();


    return 0;
}