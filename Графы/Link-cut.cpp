#ifdef LC
#include "pch.h"
#else
#include <bits/stdc++.h>
#endif

using namespace std;
#define all(x) x.begin(), x.end()
#define x first
#define y second

const int NODE_CNT = 1e6;

struct node {
	int ch[2] = {0, 0};
	int pr = 0;
	int sz = 0;
	int up = 0;
	bool rev = 0;
} nodes[NODE_CNT];
int allocated = 1;
bool &rev(int v) {
	assert(0 <= v && v < allocated);
	return nodes[v].rev;
}
void push(int v) {
	int &l = nodes[v].ch[0];
	int &r = nodes[v].ch[1];
	if (rev(v)) {
		swap(l, r);
		if (l) {
			rev(l) ^= 1;
		}
		if (r) {
			rev(r) ^= 1;
		}
		rev(v) = false;
	}
}
int &son(int v, int i) {
	assert(0 <= v && v < allocated);
	assert(0 <= i && i < 2);
	push(v);
	return nodes[v].ch[i];
}
int &ls(int v) {
	return son(v, 0);
}
int &rs(int v) {
	return son(v, 1);
}
int &pr(int v) {
	assert(0 <= v && v < allocated);
	return nodes[v].pr;
}
int &sz(int v) {
	assert(0 <= v && v < allocated);
	return nodes[v].sz;
}
int &up(int v) {
	assert(0 <= v && v < allocated);
	return nodes[v].up;
}
int new_node() {
	int v = allocated++;
	sz(v) = 1;
	return v;
}
int which(int v) {
	int p = pr(v);
	if (p) {
		if (v == ls(p)) {
			return 0;
		} else {
			assert(v == rs(p));
			return 1;
		}
	} else {
		return -1;
	}
}
void pull(int v) {
	assert(1 <= v && v < allocated);
	assert(!sz(0));
	sz(v) = sz(ls(v)) + 1 + sz(rs(v));
}
void hang(int v, int u, int i) {
	assert(0 <= v && v < allocated);
	assert(0 <= u && u < allocated);
	if (v) {
		assert(0 <= i && i < 2);
		son(v, i) = u;
		pull(v);
	}
	if (u) {
		pr(u) = v;
	}
}
void rot(int v) {
	int p = pr(v);
	assert(p);
	assert(!rev(v));
	assert(!rev(p));
	int g = pr(p);
	assert(!g || !rev(g));
	int w = which(v);
	hang(g, v, which(p));
	hang(p, son(v, w ^ 1), w);
	hang(v, p, w ^ 1);
	up(v) = up(p);
	up(p) = 0;
}
void rec_push(int v) {
	if (!v) {
		return;
	}
	rec_push(pr(v));
	push(v);
}
void splay(int v) {
	rec_push(v);
	while (pr(v)) {
		int p = pr(v);
		if (!pr(p)) {
			rot(v);
		} else if (which(p) == which(v)) {
			rot(p);
			rot(v);
		} else {
			rot(v);
			rot(v);
		}
	}
}
void print(int v, int h=1) {
	if (v) {
		print(ls(v), h + 1);
		cout << v << " ";
		print(rs(v), h + 1);
	}
	if (h == 1) {
		cout << endl;
	}
}
void expose(int v) {
	splay(v);
	int v0 = v;
	int w = rs(v);
	if (w) {
		pr(w) = 0;
		up(w) = v;
		rs(v) = 0;
		pull(v);
	}
	while (1) {
		splay(v);
		if (!up(v)) {
			break;
		}
		int u = up(v);
		splay(u);
		w = rs(u);
		if (w) {
			pr(w) = 0;
			up(w) = u;
			rs(u) = 0;
			pull(u);
		}
		hang(u, v, 1);
	}
	splay(v0);
}
int find_root(int v) {
	expose(v);
	while (ls(v)) {
		v = ls(v);
	}
	splay(v);
	return v;
}
void make_root(int v) {
	//int u = find_root(v);
	//assert(!up(u) && !up(v));
	expose(v);
	rev(v) ^= 1;
}
int parent(int v) {
	splay(v);
	if (!ls(v)) {
		return up(v);
	}
	v = ls(v);
	while (rs(v)) {
		v = rs(v);
	}
	splay(v);
	return v;
}
void cut(int v) {
	expose(v);
	if (ls(v)) {
		pr(ls(v)) = 0;
		up(ls(v)) = 0;
		ls(v) = 0;
		pull(v);
	}
}
const int N = 1e5 + 10;
int n;
int m;
int where[N];
int who[N];
void debug() {
	return;
	cout << "DEBUG" << endl;
	for (int i = 1; i <= n; ++i) {
		int j = i;
		while (pr(j)) {
			j = pr(j);
		}
		cerr << up(j) << "\t";
		print(j);
	}
	cout << endl;

	cout << "---" << endl;
}

// u is new parent of v
void link(int v, int u) {
	make_root(v);
	expose(u);
	assert(!parent(v));
	hang(v, u, 0);
	assert(parent(v) == u);
}


signed main() {
#ifdef LC
	assert(freopen("input.txt", "r", stdin));
#endif
	ios::sync_with_stdio(0);
	cin.tie(0);
	
	cin >> n >> m;
	for (int i = 0; i < n; ++i) {
		where[i] = new_node();
		who[where[i]] = i;
	}
	string cmd;
	while (m--) {
		debug();
		cin >> cmd;
		int v, u;
		if (cmd == "get") {
			cin >> v >> u;
			v = where[v - 1];
			u = where[u - 1];
			make_root(v);
			expose(u);
			debug();
			int t = sz(u);
			if (find_root(u) != v) {
				cout << "-1\n";
			} else {
				cout << t - 1 << "\n";
			}
		} else if (cmd == "link") {
			cin >> v >> u;
			v = where[v - 1];
			u = where[u - 1];
			link(v, u);
		} else {
			assert(cmd == "cut");
			cin >> v >> u;
			v = where[v - 1];
			u = where[u - 1];
			if (parent(v) == u) {
				cut(v);
			} else {
				assert(parent(u) == v);
				cut(u);
			}
		}
	}
	return 0;
}
