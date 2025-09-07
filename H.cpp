#include <iostream>
#include <vector>
#include <cmath>
#include <numbers>
#include <complex>
#include <algorithm>
#include <cassert>

int pow_2(int n) {
  int p = 1;
  while (p < n) p <<= 1;
  return p;
}

template<typename T = double>
std::complex<T> twiddle(size_t k, size_t N, bool invert)
{
  double ang = 2* std::numbers::pi * k / static_cast<double>(N);
  if (invert) ang = -ang;
  return std::exp(std::complex<T>(0.0, ang));
}

template<typename T = double>
void fft(std::vector<std::complex<double>>& a, bool invert)
{
  size_t N = a.size();
  if (N == 1) return;

  std::vector<std::complex<double>> even(N / 2), odd(N / 2);
  for (size_t i = 0; i < N / 2; ++i) {
    even[i] = a[2 * i];
    odd[i]  = a[2 * i + 1];
  }

  fft(even, invert);
  fft(odd,  invert);

  for (size_t k = 0; k < N / 2; ++k) {
    std::complex<double> t = odd[k] * twiddle<T>(k, N, invert);

    if (invert) {
      a[k]        = (even[k] + t) * 0.5;
      a[k + N/2]  = (even[k] - t) * 0.5;
    } else {
      a[k]        =  even[k] + t;
      a[k + N/2]  =  even[k] - t;
    }
  }
}

int main()
{
  int n, m;
  auto read_vec = [](size_t k) {
    std::vector<std::complex<double>> v(k);
    for (auto& x : v) {
      double t; std::cin >> t;
      x = { t, 0.0 };
    }
    return v;
  };

  std::cin >> n;
  n += 1;
  std::vector<std::complex<double>> a = read_vec(n);
  std::cin >> m;
  m += 1;
  std::vector<std::complex<double>> b = read_vec(m);
  
  reverse(a.begin(), a.end());
  reverse(b.begin(), b.end());
  
  int N = pow_2(n + m - 1);

  a.resize(N);
  b.resize(N);
  
  fft(a, false);
  fft(b, false);
  
  for (int i = 0; i < N; ++i) a[i] *= b[i];
  
  fft(a, true);
  std::reverse(a.begin(), a.end());
  int first = 0;
  while (first < n + m - 1 && std::abs(a[first].real()) < 1e-3)
    ++first;

  std::cout << n + m - 2 << ' ';
  for (int i = first; i < first + n + m - 1; ++i)
    std::cout << llround(a[i].real()) << ' ';
}
