#include <bits/stdc++.h>
using namespace std;

int *build(string str) {
	int a = *max_element(str.begin(), str.end()) + 2;
	int n = (int)str.size() + 1;
	int *s    = new int[n];
	int *c    = new int[n];
	int *suf  = new int[n];
	int *nc   = new int[n];
	int *nsuf = new int[n];
	int *beg  = new int[max(n, a)];
	for (int i = 0; i < n - 1; ++i) {
		s[i] = str[i] + 1;
	}
	s[n - 1] = -1;
	auto safe = [&](int x) {
		return x >= n ? x - n : x;
	};
    memset(beg, 0, sizeof(beg));
    for (int i = 0; i < n; ++i) {
        c[i] = s[i];
        beg[c[i] + 1]++;
        suf[i] = i;
    }
    for (int i = 0; i + 1 < a; ++i)
        beg[i + 1] += beg[i];
    for (int l = 0; l < n; l = (l ? l * 2 : 1)) {
        for (int pos, i = 0; i < n; ++i) {
            pos = safe(suf[i] - l + n);
            nsuf[beg[c[pos]]++] = pos;
        }
        int total = 0;
        for (int i = 0; i < n; ++i) {
            if ((i == 0) || (c[nsuf[i - 1]] != c[nsuf[i]]) || (c[safe(nsuf[i - 1] + l)] != c[safe(nsuf[i] + l)]))
                beg[total++] = i;
            nc[nsuf[i]] = total - 1;
        }
        memcpy(suf, nsuf, n * sizeof(suf[0]));
        memcpy(c, nc, n * sizeof(c[0]));
    }
	return suf + 1;
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0);
	string str;
	cin >> str;
	int *suf = build(str);
	for (int i = 0; i < (int)str.size(); ++i) {
		cout << suf[i] << "\n";
	}
    return 0;
}
