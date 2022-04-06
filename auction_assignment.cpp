#include <bits/stdc++.h>
using namespace std;
#define all(x) x.begin(), x.end()

/*
Finds permutation p[i] maximizing sum of value[i][p[i]].
Algorithm from:
web.eecs.umich.edu/~pettie/matching/Bertsekas-auction-algorithms-for-network-flow.pdf
*/
vector<size_t> assignment(const vector<vector<int64_t>> &value) {
	size_t n = value.size();
	if (n && value[0].size() != n)
		throw std::runtime_error("Unbalanced input");
	if (n < 2)
		return vector<size_t>(n, 0);

	const int64_t SCALE = n + 1, MIN = numeric_limits<int64_t>::min();
	const size_t UNMATCHED = n;

	vector<int64_t> price(n, 0), bids(n);
	vector<size_t> match(n, UNMATCHED), inv(n, UNMATCHED);
	for (bool any_unmatched = true; any_unmatched; ) {
		fill(all(bids), 0);
		any_unmatched = false;
		for (size_t i = 0; i < n; ++i) {
			if (match[i] != UNMATCHED)
				continue;
			any_unmatched = true;
			vector<int64_t> profit;
			transform(all(value[i]), price.begin(), back_inserter(profit),
				[&](int64_t v, int64_t p) { return SCALE * v - p; });
			size_t j = max_element(all(profit)) - profit.begin();
			int64_t bid = profit[j] + 1;
			profit[j] = MIN;
			bid -= *max_element(all(profit));
			if (bids[j] < bid) {
				size_t i2 = inv[j];
				if (i2 != UNMATCHED)
					match[i2] = UNMATCHED;
				match[i] = j;
				inv[j] = i;
				bids[j] = bid;
			}
		}
		transform(all(price), bids.begin(), price.begin(), plus<int64_t>());
	}
	return match;
}

signed main() {
#ifdef LC
	assert(freopen("input.txt", "r", stdin));
#endif
	ios::sync_with_stdio(0); cin.tie(0);
	
	size_t n;
	cin >> n;
	vector<vector<int64_t>> a(n, vector<int64_t>(n));
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			cin >> a[i][j];
		}
	}
	auto p = assignment(a);
	int64_t s = 0;
	for (size_t i = 0; i < n; ++i) {
		s += a[i][p[i]];
		cout << p[i] << " ";
	}
	cout << endl << s << endl;
	return 0;
}
