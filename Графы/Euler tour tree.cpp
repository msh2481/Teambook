#include <bits/stdc++.h>
using namespace std;
#define all(x) x.begin(), x.end()
#define x first
#define y second

mt19937 rnd;

struct node {
    node *l, *r, *p;
    int v, s;
    node(int value) :
        l(nullptr),
        r(nullptr),
        p(nullptr),
        v(value),
        s(1) {}
};

int get_size(node *v) {
    return v ? v->s : 0;
}

node *pull(node *v) {
    assert(v);
    v->s = 1;
    if (v->l) {
        v->l->p = v;
        v->s += v->l->s;
    }
    if (v->r) {
        v->r->p = v;
        v->s += v->r->s;
    }
    return v;
}

node *merge(node *l, node *r) {
    if (!l) {
        return r;
    }
    if (!r) {
        return l;
    }
    if ((int)(rnd() % (l->s + r->s)) < l->s) {
        l->r = merge(l->r, r);
        return pull(l);
    } else {
        r->l = merge(l, r->l);
        return pull(r);
    }
}

pair<node*, node*> split(node *t, int k) {
    if (!t) {
        return {nullptr, nullptr};
    }
    int lef_size = get_size(t->l);
    node *l, *r;
    if (k <= lef_size) {
        tie(l, t->l) = split(t->l, k);
        r = pull(t);
    } else {
        tie(t->r, r) = split(t->r, k - lef_size - 1);
        l = pull(t);
    }
    if (l) {
        l->p = nullptr;
    }
    if (r) {
        r->p = nullptr;
    }
    return {l, r};
}

int get_index(node *v) {
    assert(v);
    int s = 0;
    node *f = nullptr;
    for (; v; v = v->p) {
        if (!f) {
            s += get_size(v->l);
        } else if (f == v->r) {
            s += get_size(v->l) + 1;
        }
        f = v;
    }
    return s;
}

node *treap_root(node *v) {
    assert(v);
    while (v->p) {
        v = v->p;
    }
    return v;
}

int first_value(node *v) {
    assert(v);
    while (v->l) {
        v = v->l;
    }
    return v->v;
}

void print(node *v, bool out = true) {
    if (!v) {
        return;
    }
    print(v->l, false);
    cerr << v->v << " ";
    print(v->r, false);
    if (out) {
        cerr << endl;
    }
}

vector<node*> lef, rig;
vector<int> mark; // корень кактуса -> нижняя вершина цикла

void init(int n) {
    for (int i = 0; i < n; ++i) {
        lef.push_back(new node(i));
        rig.push_back(new node(i));
        mark.push_back(i);
        merge(lef[i], rig[i]);
    }
}

bool anc(int v, int u) {
    return treap_root(lef[v]) == treap_root(lef[u]) && get_index(lef[v]) <= get_index(lef[u]) && get_index(rig[u]) <= get_index(rig[v]);
}

node *cut(int v) {
    node *l, *m, *r, *tv = treap_root(lef[v]);
    int a = get_index(lef[v]), b = get_index(rig[v]) + 1;
    assert(a < b);
    tie(l, m) = split(tv, a);
    tie(m, r) = split(m, b - a);
    merge(l, r);
    return m;
}

void link(int v, int u) {
    node *l, *r, *tv = cut(v), *tu = treap_root(lef[u]);
    if (tv == tu) {
        mark[v] = u;
        return;
    }
    assert(tv != tu);
    assert(first_value(tv) == v);
    int a = get_index(rig[u]);
    tie(l, r) = split(tu, a);
    merge(merge(l, tv), r);
}




// Меняет переход из вершины v
void update(int v, int to) {
    int r = first_value(treap_root(lef[v]));
    int m = mark[r];
    if (v == r || !anc(v, m)) {
        link(v, to);
    } else {
        link(v, to);
        link(r, m);
    }
}

int subtree_size(int v) {
    int ans = get_index(rig[v]) - get_index(lef[v]) + 1;
    assert(ans % 2 == 0);
    return ans / 2;
}

// Из скольки вершин достижима v?
int query(int v) {
    int r = first_value(treap_root(lef[v]));
    int m = mark[r];
    if (anc(v, m)) {
        return subtree_size(r);
    } else {
        return subtree_size(v);
    }
}

long long number_of_pairs(int n, const vector<vector<int>> &r, const vector<vector<int>> &u) {
    init(n);
    vector<vector<pair<int, int>>> scanline(n);
    for (int i = 0; i < n; ++i) {
        int lst = 0;
        for (int j = 0; j < (int)r[i].size(); ++j) {
            scanline[lst].emplace_back(i, u[i][j]);
            lst = r[i][j] + 1;
        }
    }
    vector<int> p(n, -1);
    vector<vector<int>> g(n);
    long long answer = 0;
    for (int i = 0; i < n; ++i) {
        for (auto e : scanline[i]) {
            update(e.x, e.y);
        }
        int cur = query(i);
        assert(cur >= 1);
        answer += cur - 1;
    }
    return answer;
}

#ifdef LC
int main() {
    assert(freopen("input.txt", "r", stdin));
    int n;
    cin >> n;
    vector<vector<int>> r(n), u(n);
    for (int i = 0; i < n; ++i) {
        int k;
        cin >> k;
        r[i].resize(k);
        u[i].resize(k);
        for (int &e : r[i]) {
            cin >> e;
        }
        for (int &e : u[i]) {
            cin >> e;
        }
    }
    long long answer = number_of_pairs(n, r, u);
    cout << answer << "\n";
    return 0;
}
#endif
