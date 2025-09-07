#include <iostream>
#include <vector>

int main() {
  int n;

  std::cin >> n;
  std::vector<int> arr(n + 1, 0);
  int f_ptr = 2;
  while(f_ptr < (n / 2 + 1)) {
    for(int i = f_ptr * 2; i < n + 1; i += f_ptr) {
      if(arr[i] == 0) {
        arr[i] = f_ptr;
      }
    }

    f_ptr++;
    while (arr[f_ptr] != 0) {
      f_ptr++;
      if(f_ptr >= ((n + 1) / 2 + 1)) {
        break;
      }
    }
  }

  long long int sum = 0;
  for(int i = 0; i < n + 1; i++) {
    sum += arr[i];
  }

  std::cout << sum;

}
