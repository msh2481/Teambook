#include <bits/stdc++.h>
using namespace std;
#define all(x) x.begin(), x.end()

using dbl = long double;
const dbl EPS = 1e-12;
const dbl INF = 1e18;
mt19937 rnd;

struct simplex {
	int n; 		// количество исходных неравенств
	int m; 		// количество исходных переменных
	int z;		// количество уже добавленных неравенств
	dbl **k; 	// коэффициенты (n x (n + m))
	dbl *f;  	// целевая функция (n + m)
	int *p;  	// номер переменной (у исходных лежит в [0; m))
	dbl *lim;	// насколько большой можно сделать переменную базиса, не сломав неотрицательность
	int *who;	// чья неотрицательность сломается первой

	simplex(int n_eq, int n_var) : n(n_eq), m(n_var) {
		k = new dbl*[n];
		f = new dbl[n + m + 1];
		p = new int[n + m];
		lim = new dbl[n + m];
		who = new int[n + m];
		iota(p, p + n + m, 0);

		z = 0;
	}

	void less_equal(vector<pair<int, dbl>> poly, dbl con) {
		assert(z < n);
		k[z] = new dbl[n + m + 1];
		fill_n(k[z], n + m + 1, 0);
		k[z][z] = 1;
		for (auto e : poly) {
			k[z][n + e.first] = e.second;
		}
		k[z++][n + m] = con;
	}
	void greater_equal(vector<pair<int, dbl>> poly, dbl con) {
		assert(z < n);
		k[z] = new dbl[n + m + 1];
		fill_n(k[z], n + m + 1, 0);
		k[z][z] = 1;
		for (auto e : poly) {
			k[z][n + e.first] = -e.second;
		}
		k[z++][n + m] = -con;
	}
	void objective(vector<pair<int, dbl>> poly) {
		fill_n(f, n + m + 1, 0);
		for (auto e : poly) {
			assert(0 <= e.first && e.first < m);
			f[n + e.first] = e.second;
		}
	}
	void print() {
		cout << setprecision(2);
		cout << endl;
		for (int i = 0; i < n + m; ++i) {
			cout << p[i] << "\t";
		}
		cout << endl;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n + m; ++j) {
				cout << k[i][j] << "\t";
			}
			cout << "= " << k[i][n + m] << endl;
		}
		cout << "max" << endl;
		for (int i = 0; i <= n + m; ++i) {
			cout << f[i] << "\t";
		}
		cout << endl;
	}

	void exchange(int i, int j) {
		// i = (int)(find(p, p + n + m, i) - p);
		// j = (int)(find(p, p + n + m, j) - p);
		assert(0 <= i && i < n && n <= j && j < n + m);
		swap(p[i], p[j]);
		swap(k[i][i], k[i][j]);
		for (int pos = n + m; pos >= i; --pos) {
			k[i][pos] /= k[i][i];
		}
		for (int row = 0; row < n; ++row) {
			if (row == i) {
				continue;
			}
			dbl cur = k[row][j];
			k[row][j] = 0;
			for (int pos = n; pos <= n + m; ++pos) {
				k[row][pos] -= k[i][pos] * cur;
			}
		}
		dbl cur = f[j];
		f[j] = 0;
		for (int pos = n; pos <= n + m; ++pos) {
			f[pos] -= k[i][pos] * cur;
		}
	}

	dbl getf() {
		return -f[n + m];
	}

	dbl profit(int i, int j) {
		assert(i < n && j >= n);
		assert(abs(k[i][i] - 1) < EPS);
		dbl vj = k[i][n + m] / k[i][j];
		if (vj < 0) {
			return -INF;
		}
		dbl delta = f[j] * vj;
		for (int row = 0; row < n; ++row) {
			delta -= f[row] * vj * k[row][j];
		}
		return delta;
	}

	bool iteration() {
		// cout << "iteration " << getf() << endl;
		for (int i = 0; i < n; ++i) {
			assert(abs(k[i][i] - 1) < EPS);
			if (k[i][n + m] >= -EPS) {
				continue;
			}
			vector<int> can;
			for (int j = n; j < n + m; ++j) {
				if (k[i][j] < -EPS) {
					can.push_back(j);
				}
			}
			if (can.empty()) {
				return false;
			}
			int j = *min_element(all(can), [&](int x, int y) {
				return k[i][x] < k[i][y];
			});
			exchange(i, j);
			return true;
		}
		fill_n(lim, n + m, INF);
		fill_n(who, n + m, -1);
		for (int i = 0; i < n; ++i) {
			assert(abs(k[i][i] - 1) < EPS);
			if (k[i][n + m] < -EPS) {
				cout << "not feasible" << endl;
				return false;
			}
			for (int j = n; j < n + m; ++j) {
				if (k[i][j] <= EPS) {
					continue;
				}
				dbl cur = k[i][n + m] / k[i][j];
				if (lim[j] > cur) {
					lim[j] = cur;
					who[j] = i;
				}
			}
		}
		// for (int i = n; i < n + m; ++i) {
		// 	assert(lim[i] >= 0);
		// 	if (lim[i] <= EPS || who[i] == -1) {
		// 		continue;
		// 	}
		// 	dbl old = getf();
		// 	// if (profit(who[i], i) <= EPS) {
		// 	// 	continue;
		// 	// }
		// 	exchange(who[i], i);
		// 	dbl cur = getf();
		// 	if (old < cur) {
		// 		return true;
		// 	}
		// 	exchange(who[i], i);
		// }
		for (int i = n; i < n + m; ++i) {
			if (who[i] == -1) {
				continue;
			}
			if (f[i] > EPS) {
				exchange(who[i], i);
				return true;
			}
		}
		return false;
	}

	// mul = +1 for maximization
	// mul = -1 for minimization
	dbl solve(dbl mul) {
		for (int i = 0; i <= n + m; ++i) {
			f[i] *= mul;
		}
		for (int it = 0; iteration(); ++it) {
	    	// cout << "#" << it << ":\t" << getf() << endl;
	    }
	    for (int i = 0; i <= n + m; ++i) {
			f[i] *= mul;
		}
		// print();
		// cout << "recovery " << endl;
		// for (int i = 0; i < n; ++i) {
		// 	if (p[i] < n) {
		// 		continue;
		// 	}
		// 	assert(abs(k[i][i] - 1) < EPS);
		// 	cout << p[i] - n << ": " << k[i][n + m] << endl;
		// }
	    return getf();
	}
};

const int N = 10000;
int n, m;
int a[N];
set<int> g[N];

signed main() {
#ifdef LC
    assert(freopen("input.txt", "r", stdin));
#endif
    ios::sync_with_stdio(0); cin.tie(0);

	cin >> n;
    for (int i = 0; i < n; ++i) {
    	cin >> a[i];
    }
    for (int i = 0; i < n; ++i) {
    	for (int j = i + 1; j <= n; ++j) {
    		assert(m + 2 < N);
    		for (int t = i; t < j; ++t) {
    			g[m].insert(t);
    		}
    		// cout << m << "\t" << i << " " << j << " seq" << endl;
    		++m;
    		if ((j - i) % 2 == 0 || j - i < 3) {
    			continue;
    		}
    		for (int t = i; t < j; t += 2) {
    			g[m].insert(t);
    		}
    		// cout << m << "\t" << i << " " << j << " sparse" << endl;
    		++m;
    	}
    }
    simplex lp(2 * n, m);
    for (int i = 0; i < n; ++i) {
    	vector<pair<int, dbl>> p;
    	for (int j = 0; j < m; ++j) {
    		if (g[j].count(i)) {
    			p.emplace_back(j, 1);
    		}
    	}
    	lp.greater_equal(p, a[i]);
    	lp.less_equal(p, a[i]);
    }
    vector<pair<int, dbl>> p;
    for (int j = 0; j < m; ++j) {
    	p.emplace_back(j, 1);
    }
    lp.objective(p);
    cout << lp.solve(-1) << endl;
    cout << 1000 * clock() / CLOCKS_PER_SEC << " ms" << endl;
    return 0;
}
