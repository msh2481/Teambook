#include <bits/stdc++.h>
using namespace std;

const int MAXN 	= 1E5;

int n;

int cnt[MAXN];
int cost(int * per)
{
	int answer = 0;

	memset(cnt, 0, 2 * n * sizeof(int));
    for (int i = 0; i < n; ++i)
	{
		++cnt[per[i] + i];
	}
	for (int i = 0; i < 2 * n; ++i)
	{
        answer += cnt[i] * (cnt[i] - 1);
	}

	memset(cnt, 0, 2 * n * sizeof(int));
    for (int i = 0; i < n; ++i)
	{
		++cnt[per[i] + n - i];
	}
	for (int i = 0; i < 2 * n; ++i)
	{
        answer += cnt[i] * (cnt[i] - 1);
	}

	return answer / 2;
}

mt19937 randint(time(nullptr));

int per[MAXN];

namespace burn
{
int res, best_res;
int best_per[MAXN];
int last_res;

void repeat(int k, int eps)
{
	copy_n(best_per, n, per);
    res = cost(per);
    best_res = cost(best_per);

	for (int i, j; k--; )
	{
		last_res = res;
		i = abs((int)randint()) % n;
		j = abs((int)randint()) % n;
		swap(per[i], per[j]);
		res = cost(per);

		if (res < best_res + eps)
		{
            best_res = res;
            copy_n(per, n, best_per);
		}
		else
		{
			swap(per[i], per[j]);
			res = last_res;
		}
	}
}

int run()
{
	for (int i = 0; i < n; ++i)
	{
		swap(per[i], per[randint() % n]);
	}
	res = cost(per);
	copy_n(per, n, best_per);
	best_res = cost(best_per);

	/// For N = 1000:
	/// 100000, 50000
	repeat(20000, 	1);
	repeat(10000, 	0);

	copy_n(best_per, n, per);
	return best_res;
}
}

int main()
{
	srand((unsigned)time(nullptr));
	ios::sync_with_stdio(0);
	cin.tie(nullptr);

	cin >> n;
	iota(per, per + n, 0);

	int burns = 1;

	while (burn::run() > 0)
	{
		++burns;
	}

	for (int i = 0; i < n; ++i)
	{
		cout << per[i] + 1 << ' ';
	}
	cout << endl;

	return 0;
}
