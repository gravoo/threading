#include <atomic>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
std::mutex mut;
std::atomic<int> acnt;
void greet(const std::string &name, const int &times) {
  std::cout << name << " " << times << "\n";
  for (int i = 0; i < times; ++i)
    std::cout << "Hi, " << name << "!\n";
}

void increment(int &val) {
  for (int i = 0; i < 100000; i++) {
    ++acnt;
    std::lock_guard lg(mut);
    ++val;
  }

} // modifies caller's int

int main() {
  // Args are *copied* by default — use std::ref for references
  std::thread t1(greet, "Alice", 3);

  int counter = 0;
  std::thread t2(increment, std::ref(counter));
  std::thread t3(increment, std::ref(counter));
  std::thread t4(increment, std::ref(counter));
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  std::cout << "counter = " << counter << "\n";
  std::cout << "acnt = " << acnt << "\n";
}