struct kuhn {
    int n, m, t;
    int *f, *b, *w;
    vector<int> *g;
    kuhn(int _n, int _m) :
        n(_n),
        m(_m),
        t(1),
        f(new int[n]),
        b(new int[m]),
        w(new int[n]),
        g(new vector<int>[n]) {
        fill_n(f, n, -1);
        fill_n(b, m, -1);
    }
    void edge(int v, int u) {
        assert(0 <= v && v < n);
        assert(0 <= u && u < m);
        g[v].push_back(u);
    }
    bool dfs(int v) {
        if (w[v] == t) {
            return false;
        }
        w[v] = t;
        for (int u : g[v]) {
            if (b[u] == -1) {
                f[v] = u;
                b[u] = v;
                return true;
            }
        }
        for (int u : g[v]) {
            if (dfs(b[u])) {
                f[v] = u;
                b[u] = v;
                return true;
            }
        }
        return false;
    }
    int run() {
        int sz = 0;
        for (bool flag = true; flag; ++t) {
            flag = false;
            for (int i = 0; i < n; ++i) {
                if (f[i] == -1 && dfs(i)) {
                    ++sz;
                    flag = true;
                }
            }
        }
        return sz;
    }
};
