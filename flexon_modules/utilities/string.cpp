#include "./string.hpp"

struct buffer_data {
  int count;
  char *buffer_ptr;
};

struct buffer_data captured_buffer;

void mount_buffer(char *buffer) {
  captured_buffer.buffer_ptr = buffer;
  captured_buffer.count = 0;
  return;
}

void match_and_call(const char *match_buffer, void (*callback)()) {

  if (match_buffer == nullptr)
    return;

  bool exit = false;
  bool found = false;
  char to_match = match_buffer[0];
  int match_count = 0;

  while (exit != true) {
    if (captured_buffer.count[captured_buffer.buffer_ptr] != '\0' &&
        to_match != '\0') {
      if (captured_buffer.count[captured_buffer.buffer_ptr] == to_match) {
        captured_buffer.count += 1;
        continue;
      } else {
        captured_buffer.count += 1;
      }
      exit = true;
    };
  }

  if (found != false) {
    callback();
  }
  return;
};
