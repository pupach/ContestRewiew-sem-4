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
struct Polynomical {
  std::vector<std::complex<T>> coef;

  Polynomical(size_t size) : coef(size) {

  }

  std::complex<T>& operator[](size_t ind) {
    return coef[ind];
  }

  const std::complex<T>& operator[](size_t ind) const {
    return coef[ind];
  }

  void fft(bool invert)
  {
    size_t N = coef.size();
    if (N == 1) return;

    Polynomical<T> even = Polynomical<T>::make_polynomical_for_fft(*this, 0);
    Polynomical<T> odd = Polynomical<T>::make_polynomical_for_fft(*this, 1);

    even.fft(invert);
    odd.fft(invert);

    for (size_t k = 0; k < N / 2; ++k) {
      std::complex<double> t = odd[k] * twiddle<T>(k, N, invert);

      if (invert) {
        coef[k]        = (even[k] + t) * 0.5;
        coef[k + N/2]  = (even[k] - t) * 0.5;
      } else {
        coef[k]        =  even[k] + t;
        coef[k + N/2]  =  even[k] - t;
      }
    }
  }

  size_t size() const{
    return coef.size();
  }
  
  void forward(int N) {
    reverse(coef.begin(), coef.end());
    coef.resize(N);
    this->fft(false);    
  }
  
  int backward(int N) {
    this->fft(true);
    std::reverse(coef.begin(), coef.end());
    int first = 0;
    while (first < N && std::abs(coef[first].real()) < 1e-3)
      ++first;
    
    return first
  }

  static Polynomical<T> make_polynomical_for_fft(const Polynomical<T>& source, bool parity) {
    size_t size =  source.size() / 2;

    Polynomical<T> to_ret(size);
    for (size_t i = 0; i < size; ++i) {
      to_ret[i] = source[2 * i + parity];
    }

    return to_ret;
  }
};

std::istream& operator>>(std::istream& in, Polynomical<double> poly) {
  for (auto& x : poly.coef) {
    double t; in >> t;
    x = { t, 0.0 };
  }
}


int main()
{
  int n, m;
  
  std::cin >> n;
  n += 1;
  Polynomical<double> a(n);
  std::cin >> a;
  
  std::cin >> m;
  m += 1;
  Polynomical<double> b(m);
  std::cin >> b;
  
  int N = pow_2(n + m - 1);
  
  a.forward(N);
  b.forward(N);
  
  for (int i = 0; i < N; ++i) a[i] *= b[i];
  
  int first = a.backward(n + m - 1);

  std::cout << n + m - 2 << ' ';
  for (int i = first; i < first + n + m - 1; ++i)
    std::cout << llround(a[i].real()) << ' ';
}
