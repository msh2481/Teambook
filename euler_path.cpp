#include <bits/stdc++.h>
using namespace std;

const int N = 1100;

int n, deleted;
char adj[N][N];
char eul[N][N];

void add(int a, int b)
{
	adj[a][b] = 1;
	adj[b][a] = 1;
}

void erase(int a, int b)
{
	adj[a][b] = 0;
	adj[b][a] = 0;
}

void read_input()
{
	ios::sync_with_stdio(0);
	cin >> n >> deleted;
	++n;

    for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			add(i, j);
		}
	}

	for (int a, b; deleted--; )
	{
		cin >> a >> b;
		erase(a, b);
	}
}

vector<int> seq;

void dfs(int v)
{
	while (adj[v][v] > 0)
	{
        seq.push_back(v);
        --adj[v][v];
	}
	for (int u = 0; u < n; ++u)
	{
		if (adj[v][u])
		{
			erase(v, u);
			seq.push_back(u);
			dfs(u);
			return;
		}
	}
}

vector<vector<int>> answer;

void find_euler_paths()
{
    for (int sum, i = 0; i < n; ++i)
	{
		sum = accumulate(adj[i], adj[i] + n, 0);
		if (sum % 2)
		{
			seq = { i };
			dfs(i);
			if (seq.size() > 1)
			{
				answer.emplace_back(seq);
			}
		}
	}
	for (int i = 0; i < n; ++i)
	{
		seq = { i };
		dfs(i);
		if (seq.size() > 1)
		{
			answer.emplace_back(seq);
		}
	}
}

int main()
{
	read_input();
	find_euler_paths();
	cout << answer.size() << endl;
	for (auto & e : answer)
	{
		for (int i : e)
		{
			cout << i << ' ';
		}
		cout << "-1\n";
	}
	return 0;
}
