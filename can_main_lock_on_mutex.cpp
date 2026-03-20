#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex lock;
std::uint32_t counter{0};

void simple_function() {
  std::lock_guard lg(lock);
  std::cout << "Thread started\n";
  std::this_thread::sleep_for(std::chrono::seconds(5));
  counter++;
  std::cout << "Thread stopped\n";
}

int main() {
  std::thread t_1(simple_function);
  t_1.detach();
  //   std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "Mutex reached\n";
  std::lock_guard lg(lock);
  counter++;
  std::cout << counter << "\n";

  //   t_1.join();
}