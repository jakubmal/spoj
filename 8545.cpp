#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

unsigned long long counter = 0;
int n, n_1, n_2, _min, _max;
int inputs_1[17];
int inputs_2[17];

long long sums_1[131072]; // 2 * 2^17
long long sums_2[131072]; // 2 * 2^17

bool comp(long long sum_a, long long sum_b);
int min_path(long long min_value, bool first_half);
int max_path(long long max_value, bool first_half);
long long get_sum(unsigned long long path, bool first_half);
void fill_in_sum (unsigned long long path, int current_bit, bool first_half);

int main() {
  scanf("%d %d %d", &n, &_min, &_max);

  n_1 = n / 2;
  n_2 = n - n_1;

  for (int i = 0; i < n_1; ++i) {
    scanf("%d", &inputs_1[i]);
  }
  for (int i = 0; i < n_2; ++i) {
    scanf("%d", &inputs_2[i]);
  }

  sums_1[0] = 0;
  for (int i = 0; i < n_1; ++i) {
    fill_in_sum(1 << i, i, true);
  }
  sums_2[0] = 0;
  for (int i = 0; i < n_2; ++i) {
    fill_in_sum(1 << i, i, false);
  }

  std::sort(sums_1, sums_1 + (1 << n_1), comp);
  std::sort(sums_2, sums_2 + (1 << n_2), comp);

  // for (int i = 0; i < (1 << n_2); ++i) {
  //   printf("%lld\n", sums_2[i]);
  // }
  // return 0;

  // we could use merge sort but what the heck
  for (int i = 0; i < (1 << n_1); ++i) {
    // printf("min: %lld\tmax: %lld\n", _min - sums_1[i], _max - sums_1[i]);

    int min_index = min_path(_min - sums_1[i], false);
    int max_index = max_path(_max - sums_1[i], false);

    // printf("sums_1: %lld\tsums_2_min: %d\tsums_2_max: %d\n", sums_1[i], min_index, max_index);

    if (min_index > (1 << n_2)) continue;
    if (max_index < 0) continue;
    if (min_index > max_index) continue;
    counter += max_index - min_index + 1;
  }

  printf("%llu\n", counter);
  return 0;
}

void fill_in_sum (unsigned long long path, int current_bit, bool first_half) {
  long long current_sum = get_sum(path, first_half);
  if (first_half) {
    sums_1[path] = current_sum;
  } else {
    sums_2[path] = current_sum;
  }

  if (first_half && current_bit >= n_1)
    return;
  if (!first_half && current_bit >= n_2)
    return;

  for (int i = current_bit + 1; i < (first_half ? n_1 : n_2); ++i) {
    fill_in_sum(path + (1 << i), i, first_half);
  }
}

long long get_sum(unsigned long long path, bool first_half) {
  long long sum = 0;
  for (int i = 0; i < n; ++i) {
    unsigned long long matcher = 1;
    matcher = matcher << i;

    if (path & matcher) {
      sum += (first_half ? inputs_1 : inputs_2)[i];
    }
  }
  return sum;
}

int min_path(long long min_value, bool first_half) {
  int max_i = (1 << (first_half ? n_1 : n_2)) - 1;
  int min_i = 0;

  int upper = max_i;
  int lower = min_i;
  int i;

  while(true) {
    i = (upper + lower) / 2;
    long long current_val = (first_half ? sums_1 : sums_2)[i];
    long long prev_val;

    if (i > 0)
      prev_val = (first_half ? sums_1 : sums_2)[i - 1];
    else
      prev_val = -(((long long)1) << 60);

    // printf("min upper:%d\tlower: %d\tmin: %lld\tcurrent: %lld\tprev: %lld\n", upper, lower, min_value, current_val, prev_val);

    if (current_val > min_value && prev_val < min_value) {
      return i;
    } else if (current_val == min_value && prev_val < current_val) {
      return i;
    }

    if (current_val == min_value) {
      if (lower == i) throw;
      upper = i;
    } else if (current_val > min_value) {
      if (upper == i) return -1;
      upper = i;
    } else if (current_val < min_value) {
      if (lower == upper - 1) {
        lower = upper;
      } else if (lower == i) {
        return max_i + 1;
      } else {
        lower = i;
      }
    }
  }

  return i;
}

int max_path(long long max_value, bool first_half) {
  int max_i = (1 << (first_half ? n_1 : n_2)) - 1;
  int min_i = 0;

  int upper = max_i;
  int lower = min_i;
  int i;

  while(true) {
    i = (upper + lower) / 2;
    long long current_val = (first_half ? sums_1 : sums_2)[i];
    long long after_val;

    if (i < max_i)
      after_val = (first_half ? sums_1 : sums_2)[i + 1];
    else
      after_val = ((long long)1) << 60;

    // printf("max upper:%d\tlower: %d\tmax: %lld\tcurrent: %lld\tafter: %lld\n", upper, lower, max_value, current_val, after_val);

    if (current_val < max_value && after_val > max_value) {
      return i;
    } else if (current_val == max_value && after_val > current_val) {
      return i;
    }

    if (current_val == max_value) {
      lower = i;
    } else if (current_val < max_value) {
      if (lower == i) return max_i;
      lower = i;
    } else if (current_val > max_value) {
      if (lower == upper - 1) {
        lower = upper;
      } else if (upper == i) {
        return -1;
      } else {
        upper = i;
      }
    }
  }

  return i;
}

bool comp(long long sum_a, long long sum_b) {
  return sum_a < sum_b;
}
