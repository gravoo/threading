#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>
struct Product {
  std::uint32_t product_id;
};
std::mutex g_mutex;
std::vector<Product> products;

void consumer(std::string consumer_id) {
  std::cout << "Start consumer thread\n";
  while (true) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (!products.empty()) {
      std::stringstream out;
      out << "Product consumed by:" << consumer_id << "\n";
      std::cout << out.str();
      products.pop_back();
    }
  }
}
std::uint32_t product_id_generator() { return 5; }

void producer() {
  std::cout << "Start producer thread\n";
  while (true) {
    products.push_back(Product{product_id_generator()});
    std::this_thread::sleep_for(std::chrono::seconds(5));
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