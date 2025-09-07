#include <iostream>
#include <vector>
#include <optional>
#include <map>
#include<fstream>

using namespace std;


std::vector<int> LCP(std::string str, std::vector<int> suff) {
  int n = str.size(), temp = 0;
  std::vector<int> pos(n, -1);
  std::vector<int> lcp(n, -1);

  for(int i = 0; i < str.size(); i++) {
    pos[suff[i]] = i;
  }

  for(int i = 0; i < str.size(); i++) {
    temp = std::max(0, temp - 1);

    if(pos[i] == str.size() - 1) {
      temp = 0;
    }
    else {
      int p = suff[pos[i] + 1];
      while(std::max(i + temp, p  + temp) < n) {
        if (str[i + temp] != str[p + temp]) {
          break;
        }
        temp++;
      }
      lcp[pos[i]] = temp;
    }
  }
  lcp.pop_back();

  return lcp;
}


vector<int> suffix_array (vector<int> &s) {
  s.push_back(-2);
  int n = (int) s.size(),
      cnt = 0,
      cls = 0;
  vector<int> c(n), p(n);

  map< int, vector<int> > t;
  for (int i = 0; i < n; i++)
    t[s[i]].push_back(i);


  for (auto &x : t) {
    for (int u : x.second)
      c[u] = cls, p[cnt++] = u;
    cls++;
  }

  for (int l = 1; cls < n; l++) {
    vector< vector<int> > a(cls);
    vector<int> ct(n);
    int d = (1<<l)/2;
    int cclst  = 0;
    cnt = 0;

    for (int i = 0; i < n; i++) {
      int k = (p[i]-d+n)%n;
      a[c[k]].push_back(k);
    }

    for (int i = 0; i < cls; i++) {
      for (size_t j = 0; j < a[i].size(); j++) {
        if (j == 0 || c[(a[i][j]+d)%n] != c[(a[i][j-1]+d)%n])
          cclst++;
        ct[a[i][j]] = cclst-1;
        p[cnt++] = a[i][j];
      }
    }

    c = ct;
    cls = cclst;
  }

  return vector<int>(p.begin()+1, p.end());
}

struct Node {
  std::vector<std::pair<int, std::string>> edges;
  int depth = -1;
  int max_child_depth = 0;
  int color = -1;

  Node(int depth) : depth(depth) {}



};

struct SuffTree {
  std::vector<int> cur_path;
  std::vector<Node> nods;
  int size = -1;
  int size_f = -1;
  
  private:
    int min_deep = 100000;
    
  public:
    SuffTree(int size, int size_f) : size(size), size_f(size_f) {}

    int addNextSuff(int prev, int lenght, int lcp) {
      //std::cerr << "addNextSuff prev = " << prev << " lenght = " << lenght << " lcp = " << lcp << "\n";
      if((nods[prev].depth == lcp)) {
        auto added = Node(lenght);
        nods.push_back(added);
        cur_path.push_back(nods.size() - 1);
        nods[prev].edges.push_back({cur_path.back(), ""});

        return cur_path.back();
      }
      else {
        auto prev_ind = cur_path.back();
        cur_path.pop_back();
        int parent = cur_path.back();
        if(nods[parent].depth < lcp) {
          auto inserted = Node(lcp);
          nods.push_back(inserted);
          cur_path.push_back(nods.size() - 1);

          nods[parent].edges.back().first = cur_path.back();
          nods[cur_path.back()].edges.push_back({prev_ind, ""});
        }
        return addNextSuff(cur_path.back(), lenght, lcp);
      }
    }

    std::pair<int, int> calculatePositions(Node parent, Node child, int stringLength) {
     // std::cerr << "calculatePositions stringLength = " << stringLength << " child.max_child_depth = " << child.max_child_depth << " child.depth = " << child.depth << " parent.depth = " << parent.depth << "\n";
      int start = stringLength - child.max_child_depth + parent.depth;
      int end = start + child.depth - parent.depth + 1;
      return {start, end};
    }


    int fill_max_depth(int cur, int& color) {
      if(nods[cur].max_child_depth == 0) {
        nods[cur].color = color;
        color++;
        nods[cur].max_child_depth = nods[cur].depth;
        int max = -1;
        for(auto elem : nods[cur].edges) {
          nods[cur].max_child_depth = std::max(nods[cur].max_child_depth, fill_max_depth(elem.first, color));
        }
      }
      return nods[cur].max_child_depth;
    }

    void print_tree(int cur)  {
      for(auto elem : nods[cur].edges) {
        auto [begin, end] = calculatePositions(nods[cur], nods[elem.first], size);
        std::cout << nods[cur].color << ' ';
        if(begin >= size_f) {
          std::cout << 1 << " " << begin - size_f << " " << end - size_f - 1 << "\n";
        }
        else {
          std::cout << 0 << " " << begin << " " << std::min(end - 1,  size_f) << "\n";
        }
        print_tree(elem.first);
      }
    }
};



SuffTree make_suff_tree(std::vector<int> lcp, std::vector<int> suff_arr, std::string str, int sec_str_size = 0) {
  SuffTree tree(str.size(), str.size() - sec_str_size);

  tree.nods.push_back(Node(0));
  tree.cur_path.push_back(0);

  tree.nods.push_back(Node(str.size() - suff_arr[0]));
  tree.cur_path.push_back(1);

  tree.nods[0].edges.push_back({1, ""});
  int cur = 1;

  for(int i = 0; i < lcp.size(); i++) {

    cur = tree.addNextSuff(cur, str.size() - suff_arr[i + 1], lcp[i]);
  }


  return tree;
}

int main() {
  std::string a, b;
  std::vector<int> str;

  std::cin >> a >> b;
  a += b;

  for(int i = 0; i < a.size(); i++) {
    int symb = a[i] - 'a' + 1;
    if(symb > 36)     str.push_back(0);
    else if(symb < 0) str.push_back(0);
    else              str.push_back(symb);
  }

  auto suff_arr = suffix_array(str);
  auto lcp      = LCP(a, suff_arr);

  auto suff_tree = make_suff_tree(lcp, suff_arr, a, b.size());

  int color = 0;


  suff_tree.fill_max_depth(0, color);

  std::cout << suff_tree.nods.size() << "\n";
  suff_tree.print_tree(0);
  
}
