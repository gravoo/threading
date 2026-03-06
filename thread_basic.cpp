#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>
#include <vector>

std::mutex g_pages_mutex;
std::map<std::string, bool> task_completed;
bool stop_printing{false};
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
  g_pages_mutex.lock();
  task_completed[id] = true;
  g_pages_mutex.unlock();
}
void status_printer() {

  bool all_tasks_finished{false};
  while (!stop_printing) {
    g_pages_mutex.lock();
    for (auto &x : task_completed) {
      if (x.second) {
        std::stringstream ss;
        ss << "task:" << x.first << " " << "finished \n";
        std::cout << ss.str();
        x.second = false;
      }
    }
    g_pages_mutex.unlock();
  }
}
int main() {
  std::cout << "\n";
  std::array<std::thread, 2> threads;
  std::thread status_printer_thread;
  for (auto i{0}; i < threads.size(); i++) {
    task_completed[std::to_string(i)] = false;
    threads[i] = std::thread(loading, i + 1, 1, std::to_string(i));
  }
  status_printer_thread = std::thread(status_printer);
  for (auto &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }
  stop_printing = true;
  status_printer_thread.join();
}