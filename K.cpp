#include<iostream>
#include<unordered_map>
#include<vector>
#include<queue>
#include<set>

struct Node {
  std::vector<Node *> children;
  std::vector<Node *> suff_ref;
  Node *suffix = nullptr;
  std::pair<char, Node *> parent = {'\0', nullptr};
  bool last_vert = false;
  std::set<std::size_t> end;
  Node() : suff_ref(32, nullptr), children(32, nullptr)  {}

  bool is_son(char symb) const {
    return children[symb - 'a'] != nullptr;
  }

  bool is_link(char symb) const {
    return suff_ref[symb - 'a'] != nullptr;
  }
};


class Bor{
 public:
  Node arr[10000] = {};
  int cap = 0;
  Node head_node;

  Bor() {
    head_node = Node();
  }

  void AddWord(std::string word, std::size_t numb=0) {
    Node *cur_node = &head_node;
    for(int i = 0; i < word.size(); i++) {
      if(!cur_node->is_son(word[i])) {
        Node *new_ptr = arr + cap;
        cap++;
        new_ptr->parent = {word[i], cur_node};
        cur_node->children[word[i] - 'a'] = new_ptr;
      }
      cur_node = cur_node->children[word[i] - 'a'];
    }
    cur_node->last_vert = true;
    cur_node->end.insert(numb);
  }

  Node *FindSuffix(Node *cur_cond) {
    if(cur_cond->suffix == nullptr) {
      if((cur_cond == &head_node) || (cur_cond->parent.second == &head_node)) {
        cur_cond->suffix = &head_node;
      }
      else {
        cur_cond->suffix = FindSuffixRef(FindSuffix(cur_cond->parent.second), cur_cond->parent.first);
      }
    }
    return cur_cond->suffix;
  }

  Node *FindSuffixRef(Node *cur_cond, char symb){
    if(!cur_cond->is_link(symb)) {
      if(cur_cond->is_son(symb)) {
        cur_cond->suff_ref[symb - 'a'] = cur_cond->children[symb - 'a'];
      }
      else if(cur_cond == &head_node) {
        cur_cond->suff_ref[symb - 'a'] = &head_node;
      }
      else {
        if (cur_cond->suffix == nullptr) {
          cur_cond->suff_ref[symb - 'a'] = FindSuffixRef(FindSuffix(cur_cond), symb);
        }
        else {
          cur_cond->suff_ref[symb - 'a'] = FindSuffixRef(cur_cond->suffix, symb);
        }
      }
    }
    return cur_cond->suff_ref[symb - 'a'];
  }

  Node *NextCondition(Node *cur_cond, char symb) {
    if(cur_cond->is_son(symb)) {
      return cur_cond->children[symb - 'a'];
    }
    if(cur_cond->is_link(symb)) {
      return cur_cond->suff_ref[symb - 'a'];
    }

    else{
      return FindSuffixRef(cur_cond, symb);
    }
  }
};

class AhoKorasik{
 public:
  Bor search_pattern;


  AhoKorasik(const std::vector<std::pair<std::string, std::size_t>>& patterns) : search_pattern()  {
    for(int i = 0; i < patterns.size(); i++) {
      search_pattern.AddWord(patterns[i].first, patterns[i].second);
    }
  }

  std::vector<std::size_t> FindAmountPatterns(std::string word) {
    std::vector<std::size_t> arr_pos(word.size());
    int cnt = 0;
    Node *cond = &search_pattern.head_node;
    for(int i = 0; i < word.size(); i++) {
      cond = search_pattern.NextCondition(cond, word[i]);
      if(cond->last_vert) {
        for(auto&pos : cond->end) {
          if(((int)i - (int)pos) >= 0) {
            arr_pos[i - pos] += 1;
          }
        }
      }
    }
    return arr_pos;
  }

  void build_automaton() {
    std::queue<Node *> q;
    q.push(&search_pattern.head_node);

    while(!q.empty()) {
      Node * cur_ptr = q.front();
      q.pop();

      if(cur_ptr->suffix == nullptr) {
        search_pattern.FindSuffix(cur_ptr);
        if(cur_ptr->suffix->last_vert) {
          cur_ptr->last_vert = true;

          cur_ptr->end.insert(cur_ptr->suffix->end.begin(), cur_ptr->suffix->end.end());
        }
      }


      for (auto& child: cur_ptr->children) {
        if(child != nullptr)q.push(child);
      }
    }
  }
};


int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::string ans;
  std::string patter;
  std::vector<int> matchPositions;
  std::cin >> patter >> ans;

  for(std::size_t i = 0; i <= (ans.size() - patter.size()); i++) {
    bool is = true;
    for(std::size_t j = 0; j < patter.size(); j++) {
      if (patter[j] != '?' &&
          patter[j] != ans[i + j]) {
        is = false;
        break;
      }
    }
    if(is) {
      matchPositions.push_back(i);
    }
  }


  for(auto& elem: matchPositions) {
    std::cout << elem << " ";
  }
}

