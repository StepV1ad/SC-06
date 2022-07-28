#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdlib> // для получения рандомных чисел
#include <ctime>

using namespace std;
#define PART_ARR 10
#define PART 10
#define RAND_ARR_SIZE PART * PART_ARR

void write_arr(int randomArray[])
{
    srand(time(nullptr));
    for (unsigned int j = 0; j < RAND_ARR_SIZE; j++) 
        randomArray[j] = rand() % 100 ; 
}

void print(int randomArray[])
{
    for (unsigned int i = 0; i < RAND_ARR_SIZE; i++)
        cout << randomArray[i] << " ";
    cout << endl;
}
int calculate_without_threads(int randomArray[])
{
    int sum = 0;
    for (unsigned int i = 0; i < RAND_ARR_SIZE; i++)
        sum += randomArray[i];
    return sum;
}

void calc_sum_part(int partArr[])
{
    int sum = 0, result = 0;
    for (unsigned int i = 0; i < PART_ARR; i++)
        sum += partArr[i];
    result += sum;
    //return result;
}


int calculate_with_threads(int randomArray[])
{
    std::vector<std::thread> threads;
    int* partArr = new int[PART_ARR];
    int k = 0, result = 0;
    for (unsigned int j = 0; j < PART; j++) {
        for (unsigned int i = j * PART_ARR; i < (j + 1) * PART_ARR; i++)
        {
            partArr[k++] = randomArray[i];
        }
        k = 0;
        std::thread t(&calc_sum_part, partArr);
        threads.emplace_back(std::move(t));
    }

    for (auto& t : threads)
    {
        if (t.joinable())
            t.join();
    }
    delete[] partArr;
    return result;
}


int main()
{
    int* randomArray = new int[RAND_ARR_SIZE];
    
    write_arr(randomArray); // записываем числа в массив
    //print(randomArray);

    auto t_start = std::chrono::system_clock::now();
    int sum = calculate_without_threads(randomArray);
    auto t_end = std::chrono::system_clock::now();
    long diff = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count();
    std::cout << "Sum in main thread is " << sum << "  Time (ms): " << diff << std::endl;
    
    t_start = std::chrono::system_clock::now();
    sum = calculate_with_threads(randomArray);
    t_end = std::chrono::system_clock::now();
    diff = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count();
    std::cout << "Sum in threads is " << sum << "  Time (ms): " << diff << std::endl;

    delete[] randomArray;
    return 0;
}