#include <chrono>
#include <iostream>
#include <thread>

void do_j_work(std::stop_token token) {
  int counter{0};
  while (!token.stop_requested()) {
    std::cout << "tick " << counter++ << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }
}
int main() {
  std::jthread jthrd(do_j_work);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  jthrd.request_stop();
}