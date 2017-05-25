#include <string>
#include <sstream>
#include <algorithm>
#include <cstdint>
#include <tuple>
#include <iostream>
#include "common.h"

using namespace std;

// assumption: x.len == y.len, both positive

const unsigned LEN_TH = 2;

uint64_t StrToUll(const std::string& x)
{
	return x.length() ? stoull(x) : 0;
}



std::tuple<std::string, std::string> SplitInter(const std::string& x, unsigned len)
{
	string a = "0";
	string b;
	auto n = x.length();
	if (n <= len)
	{
		return make_tuple(a, x);
	}
	a = x.substr(0, n - len);
	b = x.substr(n - len, string::npos);
	return make_tuple(a, b);
}

std::string AddPlaceHolder(std::string s, int len)
{
	auto n = len - s.length();

	while (n-- > 0)
	{
		s.insert(0, "0");
	}
	return s;	
}

std::string TrimInterger(std::string x)
{
	for (auto ii = 0; ii < x.length(); ++ii)
	{
		if (x[ii] != '0')
		{
			x = x.substr(ii, string::npos);
			break;
		}
	}
	if (x[0]== '0')
	{
		return "0";
	}
	return x;
}

std::string AddInteger(std::string x, std::string y)
{
	x = TrimInterger(x);
	y = TrimInterger(y);
	auto lx = x.length();
	auto ly = y.length();
	if (ly > lx)
	{
		swap(x, y);
		lx = x.length();
		ly = y.length();
	}
	string z(lx + 1, 0);
	int ix = lx - 1;
	int iy = ly - 1;
	char carry = 0;
	char cx = 0;
	char cy = 0;
	for (; ix >= 0; --ix, --iy)
	{
		cx = x[ix];
		cy = iy >= 0 ? y[iy] : '0';
		auto cz = cx + cy + carry - '0' - '0';
		carry = 0;
		if (cz > 9)
		{
			cz -= 10;
			carry = 1;
		}
		z[ix + 1] = cz;
	}

	if (carry)
	{
		z[0] = carry;
	}

	for (auto ii = 0; ii < z.length(); ++ii)
	{
		if (z[ii])
		{
			z = z.substr(ii, string::npos);
			break;
		}
	}

	for (auto& c : z)
	{
		c += '0';
	}
	return z;
}

std::string SubInteger(std::string x, std::string y)
{
	x = TrimInterger(x);
	y = TrimInterger(y);
	auto lx = x.length();
	auto ly = y.length();
	if (lx < ly)
	{
		throw "SubInteger length error.";
	}
	if (lx == ly && x[0] < y[0])
	{
		throw "SubInteger first error.";
	}
	string z(x.length(), 0);
	int ix = lx - 1;
	int iy = ly - 1;
	char carry = 0;
	char cx = 0;
	char cy = 0;
	for (; ix >= 0; --ix, --iy)
	{
		cx = x[ix];
		cy = iy >= 0 ? y[iy] : '0';
		auto cz = cx - cy - carry;
		carry = 0;
		if (cz < 0)
		{
			cz += 10;
			carry = 1;
		}
		z[ix] = cz;
	}

	for (auto ii = 0; ii < z.length(); ++ii)
	{
		if (z[ii])
		{
			z = z.substr(ii, string::npos);
			break;
		}
	}

	for (auto& c : z)
	{
		c += '0';
	}
	return z;
	
}


std::tuple<std::string, std::string> AddCarryRest(const std::string& x, const std::string& y, unsigned len)
{
	string carry = "0";
	auto z = AddInteger(x, y);
	z = TrimInterger(z);
	auto n = z.length();
	if (n <= len)
	{
		return make_tuple(carry, z);
	}
	string rest;
	carry = z.substr(0, n - len);
	rest = z.substr(n - len, string::npos);

	return make_tuple(carry, rest);
}

std::string MulInteger(const std::string& x, const std::string& y)
{
	auto lx = x.length();
	auto ly = y.length();

	string z;
	if (lx <= LEN_TH && ly < LEN_TH)
	{
		uint64_t ix = StrToUll(x);
		uint64_t iy = StrToUll(y);
		auto iz = ix*iy;
		z = to_string(iz);
		return z;
	}

	auto n = max(lx, ly) / 2;
	string a;
	string b;
	string c;
	string d;

	std::tie(a, b) = SplitInter(x, n);
	std::tie(c, d) = SplitInter(y, n);

	// ac, ad + bc, bd

	auto ac = MulInteger(a, c);
	auto bd = MulInteger(b, d);

	auto add_ab = AddInteger(a, b);
	auto add_cd = AddInteger(c, d);
	auto mul_ab_cd = MulInteger(add_ab, add_cd);

	auto add_ac_bd = AddInteger(ac, bd);
	auto add_ad_bc = SubInteger(mul_ab_cd, add_ac_bd);

	// sum
	string carry_1;
	string carry_2;
	string rest_1;
	string rest_2;
	std::tie(carry_1, rest_1) = AddCarryRest(bd, "0", n);

	std::tie(carry_2, rest_2) = AddCarryRest(add_ad_bc, carry_1, n);

	auto rest_3 = AddInteger(carry_2, ac);

	stringstream ss;
	ss << rest_3 << AddPlaceHolder(rest_2, n) << AddPlaceHolder(rest_1, n);

	auto s = ss.str();
	
	return TrimInterger(s);              
}

void MulInteger(void)
{
	AddPlaceHolder("x", 3);
	CHECK(MulInteger("85005719", "55641863") == "4729876570814497");
	AddInteger("6", "10");
	AddInteger("1", "1");
	AddInteger("999", "1");
	AddInteger("899", "1");
	SubInteger("10", "9");
	SubInteger("1000", "999");
	MulInteger("59", "28");
	MulInteger("14", "10");
	auto x = "3141592653589793238462643383279502884197169399375105820974944592";
	auto y = "2718281828459045235360287471352662497757247093699959574966967627";
	auto z = MulInteger(x, y);
	cout << z << endl;

	string result = "8539734222673567065463550869546574495034888535765114961879601127067743044893204848617875072216249073013374895871952806582723184";

	if (z == result)
	{
		cout << "ok" << endl;
	}
	else
	{
		cout << "error" << endl;
	}
}