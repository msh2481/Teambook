#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
using namespace std;
using mega_set = tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>;

const int N = 10;
int fact[N + 1];

int order_of_permutation(const vector<int> & p)
{
	mega_set unused;
	for (auto elem : p)
	{
		unused.insert(elem);
	}
	assert(unused.size() == p.size());
	int answer = 0;
	for (int i = 0; i < (int)p.size(); ++i)
	{
        answer += (int)unused.order_of_key(p[i]) * fact[(int)p.size() - i - 1];
        unused.erase(p[i]);
	}
	return answer;
}

int f()
{
	vector<int> p(N);
	iota(p.begin(), p.end(), 0);

	for (int i = 0; i < N; ++i)
	{
		swap(p[i], p[rand() % N]);
	}

	return order_of_permutation(p);
}

vector<int> res;
int cnt[101];

int main()
{
	fact[0] = 1;
	for (int i = 1; i <= N; ++i)
	{
		fact[i] = fact[i - 1] * i;
	}

	for (int i = 0; i < 1000; ++i)
	{
		res.push_back(f());
	}

	sort(res.begin(), res.end());

	for (int i = 0; i < 1000; ++i)
	{
       ++cnt[(100 * res[i]) / res[999]];
	}

	for (int i = 0; i <= 100; ++i)
	{
		cout << cnt[i] << endl;
	}

	return 0;
}
