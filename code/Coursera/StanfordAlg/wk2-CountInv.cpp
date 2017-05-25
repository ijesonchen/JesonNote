#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include "common.h"

using namespace std;

int64_t CountSpliteInv(int* const p, int* const q, const int l, const int r, const int m)
{
	auto i = l, j = m + 1, k = 0, len = r - l + 1;
	int64_t cnt = 0;
	while (i <= m && j <= r)
	{
		if (p[i] < p[j])
		{
			q[l + k++] = p[i++];
		}
		else
		{
			cnt += m - i + 1;
			q[l + k++] = p[j++];
		}
	}
	auto t = (i == m + 1) ? j : i;
	while (k < len)
	{
		q[l + k++] = p[t++];
	}
	memcpy(p + l, q + l, len * sizeof(int));
	return cnt;
}

int64_t SortCountInv(int* const p, int* const q, const int l, const int r)
{
	if (l == r)
	{
		return 0;
	}
	auto m = (l + r) / 2;
	auto a = SortCountInv(p, q, l, m);
	auto b = SortCountInv(p, q, m+1, r);
	auto c = CountSpliteInv(p, q, l, r, m);
	return a + b + c;
}

int64_t BruteForce(vector<int>& v)
{
	uint64_t x = 0;
	for (unsigned ii = 1, ni = v.size(); ii < ni; ++ii)
	{
		auto a = v[ii];
		for (unsigned jj = 0, nj = ii; jj < nj; ++jj)
		{
			if (v[jj] > a)
			{
				x++;
			}
		}
	}
	return x;
}

void CountInversions(void)
{
	auto fn = "IntegerArray.txt"; // 2407905288 
	fstream fs(fn, ios::in);
	if (!fs)
	{
		cout << "error open file " << fn << endl;
		return;
	}
	vector<int> vt1;
	int x;
	while (fs >> x)
	{
		vt1.push_back(x);
	}

//	vector<int> vt1{1,3,5,2,4,6}; // 3 
//	vector<int> vt1{ 1, 3, 6, 2, 4, 5 }; // 4


	cout << "read count " << vt1.size() << endl;
	vector<int> vt2(vt1.size(), 0);

//	auto n = BruteForce(vt1);
	auto n = SortCountInv(vt1.data(), vt2.data(), 0, vt1.size() - 1);
	CHECK(n == 2407905288);
	cout << "inv count " << n << endl;
}