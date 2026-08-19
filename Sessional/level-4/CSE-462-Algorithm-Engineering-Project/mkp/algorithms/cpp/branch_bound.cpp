/*
 * Branch & Bound for Multidimensional Knapsack Problem (MKP)
 *
 * Input (stdin or file):
 *   n m
 *   p_1 ... p_n
 *   w_{1,1} ... w_{1,n}
 *   ...
 *   w_{m,1} ... w_{m,n}
 *   c_1 ... c_m
 *   time_limit_seconds
 *
 * Output (stdout):
 *   PROFIT <value>
 *   SOLUTION <x_1> <x_2> ... <x_n>
 */

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <queue>
#include <vector>
#include <cmath>
#include <cassert>

using namespace std;
using Clock = chrono::steady_clock;

// ---- global data ----
int N, M;
vector<long long> P;
vector<vector<long long>> W;   // W[i][j] = weight of item j in constraint i
vector<long long> C;
double TIME_LIMIT;
Clock::time_point START;

long long BEST_PROFIT;
vector<int> BEST_SOL;

// ---- LP relaxation upper bound ----
// For each constraint i independently, compute fractional knapsack bound.
// The real UB is the minimum across all single-constraint relaxations.
// (Simplified but fast; Dantzig-style.)
double ub_single_constraint(int i, const vector<long long>& remaining_cap,
                             const vector<int>& fixed,   // 0=free, 1=in, -1=out
                             double current_profit) {
    // Collect free items sorted by p_j / w_ij desc for constraint i
    vector<pair<double,int>> items;
    items.reserve(N);
    for (int j = 0; j < N; j++) {
        if (fixed[j] != 0) continue;
        double wij = (double)W[i][j];
        double ratio = (wij > 0) ? (double)P[j] / wij : 1e18;
        items.push_back({ratio, j});
    }
    sort(items.begin(), items.end(), [](auto& a, auto& b){ return a.first > b.first; });

    double cap = (double)remaining_cap[i];
    double val = current_profit;
    for (auto& [r, j] : items) {
        double wij = (double)W[i][j];
        if (wij <= cap) {
            val += P[j];
            cap -= wij;
        } else {
            val += r * cap;
            break;
        }
    }
    return val;
}

double upper_bound(const vector<long long>& rem_cap,
                   const vector<int>& fixed,
                   double cur_profit) {
    double ub = 1e18;
    for (int i = 0; i < M; i++) {
        double u = ub_single_constraint(i, rem_cap, fixed, cur_profit);
        ub = min(ub, u);
    }
    return ub;
}

// ---- DFS Branch & Bound ----
void dfs(int depth, vector<long long>& rem_cap, vector<int>& fixed,
         long long cur_profit) {
    // Time limit check
    auto elapsed = chrono::duration<double>(Clock::now() - START).count();
    if (elapsed >= TIME_LIMIT) return;

    // Reached leaf
    if (depth == N) {
        if (cur_profit > BEST_PROFIT) {
            BEST_PROFIT = cur_profit;
            BEST_SOL = fixed;
        }
        return;
    }

    // UB pruning
    double ub = upper_bound(rem_cap, fixed, (double)cur_profit);
    if (ub <= BEST_PROFIT) return;

    // Branch: try item `depth` in, then out (best-first by include)
    int j = depth;

    // Include branch
    bool feasible = true;
    for (int i = 0; i < M; i++) {
        if (W[i][j] > rem_cap[i]) { feasible = false; break; }
    }
    if (feasible) {
        fixed[j] = 1;
        for (int i = 0; i < M; i++) rem_cap[i] -= W[i][j];
        dfs(depth + 1, rem_cap, fixed, cur_profit + P[j]);
        for (int i = 0; i < M; i++) rem_cap[i] += W[i][j];
        fixed[j] = 0;
    }

    // Exclude branch
    fixed[j] = -1;
    dfs(depth + 1, rem_cap, fixed, cur_profit);
    fixed[j] = 0;
}

int main() {
    cin >> N >> M;
    P.resize(N);
    for (int j = 0; j < N; j++) cin >> P[j];

    W.assign(M, vector<long long>(N));
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            cin >> W[i][j];

    C.resize(M);
    for (int i = 0; i < M; i++) cin >> C[i];

    cin >> TIME_LIMIT;

    // Sort items by Toyoda ratio descending for better pruning
    auto safe_c = C;
    vector<double> ratio(N);
    for (int j = 0; j < N; j++) {
        double d = 0;
        for (int i = 0; i < M; i++)
            d += (double)W[i][j] / (safe_c[i] > 0 ? safe_c[i] : 1.0);
        ratio[j] = (d > 0) ? (double)P[j] / d : 1e18;
    }
    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b){ return ratio[a] > ratio[b]; });

    // Reorder P, W by descending ratio
    vector<long long> P2(N);
    vector<vector<long long>> W2(M, vector<long long>(N));
    for (int k = 0; k < N; k++) {
        P2[k] = P[order[k]];
        for (int i = 0; i < M; i++) W2[i][k] = W[i][order[k]];
    }
    P = P2; W = W2;

    BEST_PROFIT = 0;
    BEST_SOL.assign(N, 0);

    vector<long long> rem_cap = C;
    vector<int> fixed(N, 0);

    START = Clock::now();
    dfs(0, rem_cap, fixed, 0);

    // Map solution back to original item order
    vector<int> orig_sol(N, 0);
    for (int k = 0; k < N; k++) {
        if (BEST_SOL[k] == 1) orig_sol[order[k]] = 1;
    }

    cout << "PROFIT " << BEST_PROFIT << "\n";
    cout << "SOLUTION";
    for (int j = 0; j < N; j++) cout << " " << orig_sol[j];
    cout << "\n";

    return 0;
}
