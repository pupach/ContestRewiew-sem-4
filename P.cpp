#include <iostream>
#include <vector>
#include <optional>
#include <map>


std::vector<int> LCP(const std::string& str, const std::vector<int>& suff) {
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
      int cur = suff[pos[i] + 1];
      while(std::max(i + temp, cur  + temp) < n) {
        if (str[i + temp] != str[cur + temp]) {
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


std::vector<int> suffix_array (std::vector<int> &str) {
  str.push_back(-2);
  int n = str.size();
  std::vector<int> rank(n, 0);
  std::vector<int> sa(n, 0);

  std::map<int, std::vector<int>> valueBuckets;
  for (int i = 0; i < n; ++i) valueBuckets[str[i]].push_back(i);

  int classCount = 0;
  int writePos = 0;
  for (auto &bucket : valueBuckets) {
    for (int idx : bucket.second) {
      rank[idx] = classCount;
      sa[writePos++] = idx;
    }
    ++classCount;
  }
  for (int step = 1; classCount < n; step++) {
    int offset = (1 << step) / 2;
    std::vector<std::vector<int>> buckets(classCount);
    for (int i = 0; i < n; ++i) {
      int start = (sa[i] - offset + n) % n;
      buckets[rank[start]].push_back(start);
    }

    std::vector<int> newRank(n, 0);
    int newClassCount = 0;
    writePos = 0;
    for (int c = 0; c < classCount; ++c) {
      auto &bucket = buckets[c];
      for (size_t j = 0; j < bucket.size(); j++) {
        if ((j == 0) || (rank[(bucket[j] + offset) % n] != rank[(bucket[j - 1] + offset) % n])) {
          ++newClassCount;
        }
        newRank[bucket[j]] = newClassCount - 1;
        sa[writePos++] = bucket[j];
      }
    }

    rank.swap(newRank);
    classCount = newClassCount;
  }

  return std::vector<int>(sa.begin() + 1, sa.end());
}

struct Node {
  std::vector<int> edges;
  int depth = -1;
  int max_child_depth = 0;
  int color = -1;

  Node(int depth) : depth(depth) {}
};

struct SuffTree {
  std::vector<int> cur_path;
  std::vector<Node> nodes;

private:
  int size = -1;
  int size_f = -1;

public:
  SuffTree(int size, int size_f) : size(size), size_f(size_f) {}

  int addNextSuff(int prev, int lenght, int lcp) {
    if(nodes[prev].depth == lcp) {
      auto added = Node(lenght);
      nodes.push_back(added);
      cur_path.push_back(nodes.size() - 1);
      nodes[prev].edges.push_back(cur_path.back());

      return cur_path.back();
    }
    else {
      auto prev_ind = cur_path.back();
      cur_path.pop_back();
      int parent = cur_path.back();
      if(nodes[parent].depth < lcp) {
        auto inserted = Node(lcp);
        nodes.push_back(inserted);
        cur_path.push_back(nodes.size() - 1);

        nodes[parent].edges.back() = cur_path.back();
        nodes[cur_path.back()].edges.push_back(prev_ind);
      }
      return addNextSuff(cur_path.back(), lenght, lcp);
    }
  }

  std::pair<int, int> calculatePositions(const Node& parent, const Node& child, int stringLength) {
    int start = stringLength - child.max_child_depth + parent.depth;
    int end = start + child.depth - parent.depth + 1;
    return {start, end};
  }


  int fill_max_depth(int cur, int& color) {
    if(nodes[cur].max_child_depth == 0) {
      nodes[cur].color = color;
      color++;
      nodes[cur].max_child_depth = nodes[cur].depth;
      for(auto elem : nodes[cur].edges) {
        nodes[cur].max_child_depth = std::max(nodes[cur].max_child_depth, fill_max_depth(elem, color));
      }
    }
    return nodes[cur].max_child_depth;
  }

  void print_tree(int cur)  {
    for(auto elem : nodes[cur].edges) {
      auto [begin, end] = calculatePositions(nodes[cur], nodes[elem], size);
      std::cout << nodes[cur].color << ' ';
      if(begin >= size_f) {
        std::cout << 1 << " " << begin - size_f << " " << end - size_f - 1 << "\n";
      }
      else {
        std::cout << 0 << " " << begin << " " << std::min(end - 1,  size_f) << "\n";
      }
      print_tree(elem);
    }
  }
};


SuffTree make_suff_tree(const std::vector<int>& lcp, const std::vector<int>& suff_arr, const std::string& str, int sec_str_size = 0) {
  SuffTree tree(str.size(), str.size() - sec_str_size);

  tree.nodes.push_back(Node(0));
  tree.cur_path.push_back(0);

  tree.nodes.push_back(Node(str.size() - suff_arr[0]));
  tree.cur_path.push_back(1);

  tree.nodes[0].edges.push_back(1);
  int cur = 1;

  for(int i = 0; i < lcp.size(); i++) {

    cur = tree.addNextSuff(cur, str.size() - suff_arr[i + 1], lcp[i]);
  }

  return tree;
}

int main() {
  std::string f_str, s_str;
  std::vector<int> str;

  std::cin >> f_str >> s_str;
  f_str += s_str;

  for(int i = 0; i < f_str.size(); i++) {
    int symb = f_str[i] - 'a' + 1;
    if((symb > 35) || (symb <= 0))     str.push_back(0);
    else              str.push_back(symb);
  }

  auto suff_arr = suffix_array(str);
  auto lcp      = LCP(f_str, suff_arr);

  auto suff_tree = make_suff_tree(lcp, suff_arr, f_str, s_str.size());

  int color = 0;

  suff_tree.fill_max_depth(0, color);

  std::cout << suff_tree.nodes.size() << "\n";
  suff_tree.print_tree(0);

}
