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

void loading(int sleep_time) {
  int width = getTerminalWidth();
  int refresh_rate = 1;
  int samples = sleep_time / refresh_rate;
  int iteration = 0;
  std::string start = "\033[H0%";
  std::string stop = "100%\n";
  std::string marker = "#";
  std::stringstream output;
  output << start;
  int line_length = width - start.length() - stop.length();
  for (int i = 0; i < line_length; i++) {
    output << " ";
  }
  output << stop;
  std::cout << output.str();
  output.clear();
  output.str("");
  std::this_thread::sleep_for(std::chrono::seconds(refresh_rate));

  const int progress_increment = line_length / samples;
  while (iteration < samples) {
    output.clear();
    output.str("");
    output << start;
    // for (int i = 0; i < progress_increment * iteration; i++) {
    //   output << marker;
    // }
    for (int i = output.str().size(); i < line_length; i++) {
      output << "B";
    }
    ++iteration;
    std::cout << output.str();
    output << stop;
    std::this_thread::sleep_for(std::chrono::seconds(refresh_rate));
  }
}
int main() {
  std::cout << "\033[H";
  loading(5);
}