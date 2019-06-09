#include <iostream>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

#include <list>
#include <vector>
#include <regex>
#include <algorithm>
#include <iterator>
#include <thread>
#include <chrono>

using namespace Minisat;
using namespace std;
using namespace std::chrono;

/**
 *
p cnf 2 4 // 2 var, 4 clause
1 2 0
1 -2 0
-1 2 0
-1 -2 0

 ***********
  Solver s;
  Var a = s.newVar(),
      b = s.newVar();
  s.addClause(mkLit(a, false), mkLit(b, false));
  s.addClause(mkLit(a, false), mkLit(b, true));
  s.addClause(mkLit(a, true), mkLit(b, false));
  s.addClause(mkLit(a, true), mkLit(b, true));
  bool res = s.solve();
  if (res) {
    cout << "SAT" << endl;
  } else {
    cout << "not SAT" << endl;
  }
 */

// ¬ var, means negatedLiteral == true
Lit Minisat::mkLit(Var var, bool negatedLiteral);

void extractVertex(int tup[], const string edge) {
  string s(edge);
  smatch m;
  regex e("\\d+");
  regex_match(s, m, e);
  int i = 0;
  while (regex_search(s, m, e)) {
    for (string x:m) {
      tup[i] = stoi(x);
      i++;
    }
    s = m.suffix().str();
  }
}

bool extractAdj(vector<int> *adj, const string line, const int maxNodeNum) {
  string s(line);
  smatch m;
  regex e("\\d+,\\d+");
  regex_match(s, m, e);
  while (regex_search(s, m, e)) {
    for (string x:m) {
      int *tup = new int[2];
      extractVertex(tup, x);
      if (tup[0] >= maxNodeNum || tup[1] >= maxNodeNum) {
        cout << "Error: vertex number exceed!" << endl;
        return false;
      }
      adj[tup[0]].push_back(tup[1]);
      adj[tup[1]].push_back(tup[0]);
    }
    s = m.suffix().str();
  }
  return true;

}

string printValue(lbool v) {
  return v == l_True ? "true" : "false";
}

void deep_copy_vector(vector<int> *&src, vector<int> *&dest, int array_len){
  for(int i =0;i<array_len;i++) {
    std::copy(src[i].begin(), src[i].end(), std::back_inserter(dest[i]));
  }
}

/**
 * running VC using miniSAT
 * @param v
 * @param adj an array, which element is vector<int>
 * @param result
 * @param elapsed time, microseconds
 */
void cnf_sat_vc(const int &v, vector<int> *&_adj, vector<int> &result, double &elapsed) {
  auto start = high_resolution_clock::now();
  result.clear();
  int n = v; // node number

  // deep copy array
  vector<int> *adj = new vector<int>[v];
  deep_copy_vector(_adj, adj, v);

  int k = 1; // min vertex cover
  while (k <= n) {
    int a, b, d; // a for count k, b for count n, d for extra count
    Var vars[n][k];// Var matrix
    Solver s;

    // initialize the value of every atom, in all number of n * k atoms.
    for (a = 0; a < k; a++) {
      for (b = 0; b < n; b++) {
        vars[b][a] = s.newVar();
      }
    }

    //case 1
    for (a = 0; a < k; a++) {
      vec<Lit> case1;
      for (b = 0; b < n; b++) {
        case1.push(mkLit(vars[b][a], false));
      }
      s.addClause(case1);
    }

    //case 2
    if (k > 1) {
      for (b = 0; b < n; b++) {
        for (a = 0; a < k - 1; a++) { // p = a
          for (d = 1; a + d < k; d++) { // q = a+d
            s.addClause(mkLit(vars[b][a], true),
                        mkLit(vars[b][a + d], true));
          }
        }
      }
    }

    //case 3
    for (a = 0; a < k; a++) {
      for (b = 0; b < n - 1; b++) { // p = b
        for (d = 1; b + d < n; d++) { // q = b+d
          s.addClause(mkLit(vars[b][a], true),
                      mkLit(vars[b + d][a], true));
        }
      }
    }

    //case 4
    for(int src=0; src < n; src++) {
      for (int dest: adj[src]) {
        if(src < dest) {
          vec<Lit> case4;
          for (a = 0; a < k; a++) {
            case4.push(mkLit(vars[src][a], false));
            case4.push(mkLit(vars[dest][a], false));
          }
          s.addClause(case4);
        }
      }
    }

    bool res = s.solve();
    if (res) {
//        cout << "SAT, k = " << k << endl;
      for (a = 0; a < k; a++) {
        for (b = 0; b < n; b++) {
          if (s.modelValue(vars[b][a]) == l_True) {
            result.push_back(b);
          }
        }
      }
      break;
    }
    k++;
  }
  if (k > n) {
    result.clear();
//    cout << "Error: UNSAT!" << endl;
  }
  delete []adj;
  elapsed = (duration_cast<microseconds>(high_resolution_clock::now() - start)).count();
}

int compute_total_degrees(vector<int> *&adj, int ct[], int n){
  int sum = 0;
  for(int src=0; src < n; src++) {
    if(adj[src].size() >0) { // it does have edges
      sum += ct[src];
    }
  }
  return sum;
}

void approx_vc_1(const int &v, vector<int> *&_adj, vector<int> &result, double &elapsed) {
  auto start = high_resolution_clock::now();
  int n = v; // node number

  // deep copy array
  vector<int> *adj = new vector<int>[v];
  deep_copy_vector(_adj, adj, v);

  int ct[n];// count each vertex's inbound degrees
  int i;
  int max, index, sum = 0;

  for (i = 0; i < n; i++) {
    ct[i] = 0;
  }

  for(int src=0; src < n; src++) {
    if(adj[src].size() >0) { // it does have edges
      for (int dest: adj[src]) {
        if(src < dest) {
          ct[src] += 1;
          ct[dest] += 1;
        }
      }
    }
  }
  sum = compute_total_degrees(adj, ct, n);

  while (sum != 0) {
    max = ct[0];
    index = 0;
    // find the node with highest degree
    for (i = 1; i < n; i++) {
      if (max < ct[i]) {
        max = ct[i];
        index = i;
      }
    }
    ct[index] = 0;

    //add index to the vertex cover
    result.push_back(index);
    for (int src: adj[index]) {
      ct[src]--;
    }
    ct[index] = 0;
    adj[index].clear();

    sum = compute_total_degrees(adj, ct, n);
//    cout << sum << endl;
  }//end while

  delete []adj;
  elapsed = (duration_cast<microseconds>(high_resolution_clock::now() - start)).count();
}

int compute_total_edges(vector<int> *&adj, int n) {
  int edges =0;
  for(int src=0; src < n; src++) {
    if(adj[src].size() >0) { // it does have edges
      for(auto dest: adj[src]) {
        if(src < dest) {
          edges++;
        }
      }
    }
  }
  return edges;
}

// remove element of vertex from vec vector
void remove_element(int vertex, vector<int> &vec) {
  int pos = -1;
  for (unsigned i=0; i<vec.size(); i++){
    if (vertex == vec.at(i)) {
      pos = i;
      break;
    }
  }
  if(pos != -1)
    vec.erase(vec.begin()+pos);
}

// remove all edges that start or ends with vertex
void remove_vertex_edge(int vertex, vector<int> *&adj, const int v) {
  for(int src=0; src < v; src++) {
    if(adj[src].size() >0) { // it does have edges
      remove_element(vertex, adj[src]);
    }
  }
}

void approx_vc_2(const int &v, vector<int> *&_adj, vector<int> &result, double &elapsed) {
  auto start = high_resolution_clock::now();
  int n = v;
  // deep copy array
  vector<int> *adj = new vector<int>[v];
  deep_copy_vector(_adj, adj, v);

  int edges = compute_total_edges(adj, n);
  while(edges > 0) {
    for(int src = 0;src < n; src ++) {
      if(adj[src].size() > 0) { // it does have edges
        for(int dest: adj[src]){
          // pick up a src->dest
          remove_vertex_edge(src, adj, v);
          remove_vertex_edge(dest, adj, v);
          result.push_back(src);
          result.push_back(dest);
          adj[src].clear();
          adj[dest].clear();
          edges = compute_total_edges(adj, n);
          break;
        }
      }
    }
  }
  delete []adj;
  elapsed = (duration_cast<microseconds>(high_resolution_clock::now() - start)).count();
}

void sort_print_result(vector<int> &result, string prefix) {
  std::sort(result.begin(), result.end());
  if (result.size() > 0) {
    std::cout << prefix;
    for (std::vector<int>::iterator it = result.begin(); it != result.end(); ++it)
      std::cout << *it << " ";
    std::cout << '\n';
  } else {
    cout << "Error: UNSAT!" << endl;
  }
}



/**
 *
 * @param line
 * @param v
 * @param adj vector<int> *, 2 dimensional vector
 */
void handle_input(string line, int &v, vector<int> *&adj) {
  if (line[0] == 'V') {
    v = stoi(line.substr(2, string::npos));
  }
  if (line[0] == 'E') {
    adj = new vector<int>[v];
    bool ret = extractAdj(adj, line, v);
    if (ret == false) {
      delete[]adj;
      return;
    }


    int rounds = 10;
    for(int i = 0;i < rounds;i++) {
      vector<int> result_cnf, result_a1, result_a2;
      double running_seconds_cnf,running_seconds_a1,running_seconds_a2;// acctually they are microseconds, which is 10^(-6) seconds

      std::thread threadCNF(cnf_sat_vc, std::ref(v), std::ref(adj), std::ref(result_cnf), std::ref(running_seconds_cnf));
      std::thread threadA1(approx_vc_1, std::ref(v), std::ref(adj), std::ref(result_a1), std::ref(running_seconds_a1));
      std::thread threadA2(approx_vc_2, std::ref(v), std::ref(adj), std::ref(result_a2), std::ref(running_seconds_a2));
      threadCNF.join();
      threadA1.join();
      threadA2.join();

      cout << "cnf_runtime: " << running_seconds_cnf << endl;
      cout << "a1_runtime: " << running_seconds_a1 << endl;
      cout << "a2_runtime: " << running_seconds_a2 << endl;

      // convert to double type
      cout << "a1_ratio: " << result_a1.size()/(result_cnf.size()*1.0) << endl;
      cout << "a2_ratio: " << result_a2.size()/(result_cnf.size()*1.0) << endl;
    }

    delete[]adj;
  }// end if "E "

}

int main(int argc, char **argv) {

  int v;
  vector<int> *adj;
  string line;
  while (getline(cin, line)) {
    handle_input(line, v, adj);
  }
  return 0;

}
