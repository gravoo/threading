#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

std::mutex g_mutex;
int g_int{0};
void consumer(std::string consumer_id) {
  std::cout << "Start consumer thread\n";
  while (true) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (g_int > 0) {
      std::stringstream out;
      out << "Product consumed by:" << consumer_id << "\n";
      g_int -= 1;
      std::cout << out.str();
    }
  }
}
void producer() {
  std::cout << "Start producer thread\n";
  int products{0};
  while (true) {
    if (g_int == 0) {
      std::stringstream out;
      products++;
      out << products << " produced \n";
      std::cout << out.str();
      g_int = products;
    } else {
      std::cout << "Products not yet consumed\n";
      std::this_thread::sleep_for(std::chrono::seconds(5));
    }
  }
}
int main() {
  std::thread consumer_thread_1(consumer, "#0");
  std::thread consumer_thread_2(consumer, "#1");
  std::thread producer_thread(producer);
  producer_thread.join();
  consumer_thread_1.join();
  consumer_thread_2.join();
}