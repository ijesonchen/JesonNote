#include <cstdlib>

using namespace std;

void CHECK(bool b)
{
	if (!b)
	{
		abort();
	}
}

template <typename TA, typename TB>
void CHECK(const TA& a, const TB& b)
{
	if (a != b)
	{
		abort();
	}
}
