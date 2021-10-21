#include <bits/stdc++.h>
using namespace std;

// flow2: 0.047
struct dinic {
    using flowt     = int32_t;
    using answt     = int64_t;
    const flowt INF = numeric_limits<flowt>::max();

    struct edge {
        int   u;
        flowt f;
        flowt c;
        int   r;
        edge(int _u, flowt _c, int _r) :
            u(_u),
            f(0),
            c(_c),
            r(_r) {}
    };

    int n;
    int source;
    int sink;
    vector<edge> *g;
    int *q, *d, *p;
    vector<pair<int, int>> w;

    dinic(int nvertex, int s, int t) :
        n(nvertex),
        source(s),
        sink(t),
        g(new vector<edge>[nvertex]),
        q(new int[nvertex]),
        d(new int[nvertex]),
        p(new int[nvertex]) {}

    void addedge(int v, int u, int c, bool dir = true) {
        w.emplace_back(v, (int)g[v].size());
        g[v].emplace_back(u, c, (int)g[u].size());
        g[u].emplace_back(v, dir ? 0 : c, (int)g[v].size() - 1);
    }

    bool bfs() {
        int l = 0, r = 0;
        fill_n(d, n, -1);                                           /// memset???
        fill_n(p, n, 0);                                            /// memset???
        q[r++] = source;
        d[source] = 0;
        while (l < r) {
            int v = q[l++];
            for (auto e : g[v]) {
                if (d[e.u] == -1 && e.c - e.f >= lim) {
                    d[e.u] = d[v] + 1;
                    q[r++] = e.u;
                }
            }
        }
        return d[sink] != -1;
    }

    int lim;

    flowt dfs(int v, flowt minc) {
        assert(minc >= lim && lim > 0);
        if (v == sink) {
            return minc;
        }
        for (; p[v] < (int)g[v].size(); ++p[v]) {                   /// заменить на while
            edge &e = g[v][p[v]];
            edge &r = g[e.u][e.r];
            if (e.c - e.f < lim || d[e.u] != d[v] + 1) {
                continue;
            }
            int delta = dfs(e.u, min(minc, e.c - e.f));
            if (delta) {
                e.f += delta;
                r.f -= delta;
                return delta;
            }
        }
        return 0;
    }

    answt maxflow() {
        answt answer = 0;
        for (lim = 1 << 30; lim >= 1; lim >>= 1) {
            while (bfs()) {
                for (flowt delta; (delta = dfs(source, INF)); ) {
                    answer += delta;
                }
            }
        }
        return answer;
    }
};

signed main() {
#ifdef LC
	assert(freopen("input.txt", "r", stdin));
#else
#define TASK "flow2"
	assert(freopen(TASK ".in", "r", stdin));
	assert(freopen(TASK ".out", "w", stdout));
#endif
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
    cin >> n >> m;
    dinic solver(n, 0, n - 1); /// global
    for (int v, u, w; cin >> v >> u >> w; ) {
        solver.addedge(v - 1, u - 1, w);
    }
    cout << solver.maxflow() << "\n";
    for (auto ptr : solver.w) {
        cout << solver.g[ptr.first][ptr.second].f << "\n";
    }
    return 0;
}
