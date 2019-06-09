#include <iostream>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

#include <iostream>
#include <list>
#include <vector>
#include <regex>
#include <sstream>

using namespace Minisat;
using namespace std;

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

void handle_input(string line, int &v, vector<int> *&adj) {
  if (line[0] == 'V') {
    v = stoi(line.substr(2, string::npos));
//    cout << line << endl; // just for a3
  }
  if (line[0] == 'E') {
    adj = new vector<int>[v];
    bool ret = extractAdj(adj, line, v);
    if (ret == false) {
      delete[]adj;
    }
//    cout << line << endl;  // just for a3


    // running VC using miniSAT
    int n = v; // node number
    int k = 1; // min vertex cover
    while (k <= n) {
      int a, b, d; // a for count k, b for count n, d for extra count
      int g = 1;// value of variable
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
      for (std::vector<int>::iterator it = adj->begin(); it != adj->end(); ++it) {
        int src = *it;
        for (int dest: adj[src]) {
          vec<Lit> case4;
          for (a = 0; a < k; a++) {
            case4.push(mkLit(vars[src][a], false));
            case4.push(mkLit(vars[dest][a], false));
          }
          s.addClause(case4);
        }
      }

      bool res = s.solve();
      vector<int> result;
      if (res) {
//        cout << "SAT, k = " << k << endl;
        for (a = 0; a < k; a++) {
          for (b = 0; b < n; b++) {
            if (s.modelValue(vars[b][a]) == l_True) {
              result.push_back(b);
            }
          }
        }
        std::sort(result.begin(), result.end());
        for (std::vector<int>::iterator it = result.begin(); it != result.end(); ++it)
          std::cout << *it << " ";
        std::cout << '\n';
        break;
      }
      k++;
    }
    if (k > n) {
      cout << "Error: UNSAT!" << endl;
    }
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
