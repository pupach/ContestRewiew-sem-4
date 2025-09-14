#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<chrono>

#define MOD_1 14820325592701
#define MOD_2 12337029564711
#define MNOJ 61

long long int hash_begin(std::string str, std::size_t size, long long int mod) {
  long long int to_ret = 0;
  for(long long int i = 0; i < size; i++) {
    to_ret = (to_ret * MNOJ + (long long int)str[i]) % mod;
  }
  return to_ret;
}
int main() {
  std::string str_main;
  std::string str_in;


  auto started = std::chrono::high_resolution_clock::now();

  std::cin >> str_main >> str_in;
  std::string pref_str = str_in + "#" + str_main;
  std::vector<int> p(pref_str.size(), 0);
  std::vector<std::size_t> ans;

  if( str_in.size() <= str_main.size()) {

   for(int i = 1; i < pref_str.size(); i++) {
     int prev = p[i - 1];
     while((prev > 0) && (pref_str[i] != pref_str[prev])) {
       prev = p[prev - 1];
     }
     if(pref_str[i] == pref_str[prev]) {
       ++prev;
     }
     p[i] = prev;
   }

   for(std::size_t i = str_in.size() + 1; i < pref_str.size(); i++) {
     if(p[i] == str_in.size()) {
       ans.push_back(i - str_in.size() - str_in.size() );
     }
   }
   for(const std::size_t& i : ans) {
     std::cout << i << "\n";
   }
  }
}
