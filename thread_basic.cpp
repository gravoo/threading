#include <array>
#include <chrono>
#include <iostream>
#include <sstream>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>
#include <vector>

int getTerminalWidth() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
}
void print_completion_bar(float progress, std::string id) {
  int width = getTerminalWidth();

  std::string start = "id: " + id + " 0%";
  std::string stop = "100%\n";
  std::string marker = "#";
  std::stringstream output;
  output << start;
  int line_length = width - start.length() - stop.length();
  int progress_bar = progress * line_length;
  for (int i = 0; i < progress_bar; i++) {
    output << marker;
  }
  for (int i = progress_bar; i < line_length; i++) {
    output << " ";
  }
  output << stop;
  std::cout << output.str();
}
void super_heavy_task(int time_to_complete_task) {
  std::this_thread::sleep_for(std::chrono::seconds(time_to_complete_task));
}
void loading(int time_of_single_task, int count_of_tasks, std::string id) {
  for (int i{0}; i <= count_of_tasks; i++) {
    print_completion_bar(i / static_cast<float>(count_of_tasks), id);
    super_heavy_task(time_of_single_task);
  }
}
int main() {
  std::cout << "\n";
  std::array<std::thread, 4> threads;
  for (auto i{0}; i < threads.size(); i++) {
    threads[i] = std::thread(loading, i + 1, 500, std::to_string(i));
  }
  for (auto &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }
}