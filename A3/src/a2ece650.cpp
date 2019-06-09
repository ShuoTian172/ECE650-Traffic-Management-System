#include <iostream>
#include <list>
#include <vector>
#include <regex>
#include <sstream>

using namespace std;

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

void printShortestDistance(vector<int> *adj, int src, int dest, int v);

void generate(string line, const int &v, vector<int> *adj) {
    string word;
    istringstream iss(line, istringstream::in);
    iss >> word; // skip the 's'
    iss >> word;
    int source = stoi(word);
    iss >> word;
    int dest = stoi(word);

    if (source > v || dest > v) {
        cout << "Error: wrong edge!" << endl;
    } else printShortestDistance(adj, source, dest, v);

}

bool BFS(vector<int> *adj, int src, int dest, int v, int pred[]) {
    list<int> queue;
    bool visited[v];
    for (int i = 0; i < v; i++) {
        visited[i] = false;
        pred[i] = -1;
    }
    visited[src] = true;
    queue.push_back(src);

    // standard BFS algorithm 
    while (!queue.empty()) {
        int u = queue.front();
        queue.pop_front();
        for (int i = 0; i < adj[u].size(); i++) {
            if (visited[adj[u][i]] == false) {
                visited[adj[u][i]] = true;
                pred[adj[u][i]] = u;
                queue.push_back(adj[u][i]);

                // We stop BFS when we find destination.
                if (adj[u][i] == dest)
                    return true;
            }
        }
    }

    return false;
}

void printShortestDistance(vector<int> *adj, int src, int dest, int v) {
    int pred[v];

    if (BFS(adj, src, dest, v, pred) == false) {
        cout << "Error: given source and destination are not connected";
        return;
    }

    // vector path stores the shortest path
    vector<int> path;
    int crawl = dest;
    path.push_back(crawl);
    while (pred[crawl] != -1) {
        path.push_back(pred[crawl]);
        crawl = pred[crawl];
    }

    // printing path from source to destination
    for (int i = path.size() - 1; i > 0; i--)
        cout << path[i] << "-";
    cout << path[0] << endl;
}

void handle_input(string line, int &v, vector<int> *&adj) {
    if (line[0] == 'V') {
        v = stoi(line.substr(2, string::npos));
        cout << line << endl; // just for a3
    }
    if (line[0] == 'E') {
        adj = new vector<int>[v];
        bool ret = extractAdj(adj, line, v);
        if (ret == false) {
            delete []adj;
        }
        cout << line << endl;  // just for a3
    }
    if (line[0] == 's') {
        generate(line, v, adj);
    }

}


// Driver program to test above functions 
int main() {
    int v;
    vector<int> *adj;
    string line;
    while (getline(cin, line)) {
        handle_input(line, v, adj);
    }
    return 0;
}