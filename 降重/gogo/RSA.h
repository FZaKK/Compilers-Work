#include <iostream>
#include <fstream>
#include <time.h>
#include <ctime>
#include <string>
#include "BigInt.h"
using namespace std;

char hex_table[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

// 加密函数
BigInt Encrypt(BigInt& m, BigInt& e, BigInt& n) {
	return Pow_Mod(m, e, n);
}

// 解密函数
BigInt Decrypt(BigInt& c, BigInt& d, BigInt& n) {
	return Pow_Mod(c, d, n);
}

// 产生一个512bit的大奇数，利用素数表先筛一遍
bool GenOdd(BigInt& n) {
	char s[129]; // 129位补充\0
	for (int i = 0; i < 512 / 4; i++) {
		s[i] = hex_table[rand() % 16];
	}
	s[127] = '1';
	s[128] = '\0';
	n = BigInt(s);
	BigInt divisor;
	const int length = sizeof(prime) / sizeof(int);
	for (int i = 0; i < length; i++) {
		divisor = prime[i];
		if ((n % divisor) == 0) {
			return false;
		}
	}
	return true;
}


BigInt CreateRandom(BigInt a) {
	unsigned long t = 0;
	do {
		t = rand();
	} while (t == 0);

	BigInt mod(t);
	BigInt ans = mod % a;
	if (ans == BigInt(0)) {
		// 必须非零
		ans = a - BigInt(1);
	}
	return ans;
}

// RabinMiller素性检验五轮
bool RabinMiller(const BigInt& n) {
	BigInt d = n - 1, a, y;
	unsigned int s = 0;
	// d = n - 1;

	//s记录大数n末尾的零的个数，并将d右移 
	while (!d.IsOdd()) {
		s++;
		d >> 1;
	}

	for (int i = 0; i < 5; i++) {
		a = CreateRandom(n - 1);
		y = Pow_Mod(a, d, n);  // y = a^d mod n
		// 如果满足a^d mod n != -1，那么n可能为合数
		if (y == 1) {
			continue;
		}
		bool prob = true;
		// 如果满足对于所有的j∈[0,s-1]，a^(2^j*d) mod n != -1，那么n为合数
		for (unsigned int j = 0; j < s; j++) {
			if (y == n - 1) {
				prob = false;
				break;
			}
			y = (y * y) % n;
		}

		if (prob) {
			return false;
		}
	}

	return true;    // 通过五轮测试
}

// 产生一个大素数
BigInt GenPrime() {
	BigInt n;
	int i = 0;
	while (1) {
		// TODO
		// while (!GenOdd(n)) {}
		while (1) {
			if (GenOdd(n))
				break;
		}
		if (RabinMiller(n))
			break;
	}
	return n;
}

// 欧几里得算法求最大公约数
BigInt Gcd(const BigInt& m, const BigInt& n) {
	if (n == 0)
		return m;
	else
		return Gcd(n, m % n);
}

// 扩展欧几里得算法求乘法逆元
BigInt ExtendedGcd(const BigInt& a, const BigInt& b, BigInt& x, BigInt& y) {
	BigInt t, d;
	// 如果一个操作数为零则无法进行除法运算
	if (b == 0) {
		x = 1;
		y = 0;
		return a;
	}
	d = ExtendedGcd(b, a % b, x, y);
	t = x;
	x = y;
	y = t - ((a / b) * y);
	return d;
}

void Gen_Key_File() {
	// 设置随机数种子
	srand((unsigned)time(NULL));
	cout << endl << "---*** 正在生成大素数(512bit) ***---" << endl << endl;
	clock_t t = clock();
	cout << "---*** 大素数p ***---" << endl;
	BigInt p = GenPrime();
	p.print();
	cout << endl;

	cout << "---*** 大素数q ***---" << endl;
	BigInt q = GenPrime();
	q.print();
	cout << endl;

	cout << "---*** 生成大素数耗时 ***---" << endl;
	cout << (clock() - t) / 1000 << "s" << endl;

	cout << endl << "---*** 正在生成公钥与私钥 ***---" << endl << endl;
	cout << "---*** 大素数之积n ***---" << endl;
	BigInt n = p * q;
	n.print();
	cout << endl;

	BigInt fn = (p - 1) * (q - 1);
	BigInt e, d, temp;
	while (1) {
		// 生成一个与fn互素的大素数e
		do {
			GenOdd(e);
		} while (!(Gcd(e, fn) == 1));

		// 用扩展欧几里德算法试图求出e模fn的乘法逆元,保存在d中
		ExtendedGcd(e, fn, d, temp);

		// 检测产生的乘法逆元的正确性，不正确就重新生成
		BigInt temp1 = (e * d) % fn;
		if (temp1 == 1) {
			break;
		}
	}
	cout << "---*** 公钥{e, n} ***---" << endl;
	cout << "大素数e为：" << endl;
	e.print();
	cout << endl;

	// 输出文件流，公钥
	ofstream fout1("RSA公钥.txt");
	for (int i = e.length() - 1; i >= 0; i--) {
		unsigned int temp = e.data[i];
		for (int j = 0; j < 8; j++) {
			unsigned int result = temp & 0xf0000000;
			result = (result >> 28);
			temp = (temp << 4);
			if (result >= 0 && result <= 9)
				fout1 << result;
			else
				fout1 << char('A' + result - 10);
		}
	}
	fout1 << endl;


	cout << "素数乘积n为：" << endl;
	n.print();
	cout << endl;
	// ofstream fout("RSA公钥.txt");
	for (int i = n.length() - 1; i >= 0; i--) {
		unsigned int temp = n.data[i];
		for (int j = 0; j < 8; j++) {
			unsigned int result = temp & 0xf0000000;
			result = (result >> 28);
			temp = (temp << 4);
			if (result >= 0 && result <= 9)
				fout1 << result;
			else
				fout1 << char('A' + result - 10);
		}
	}
	fout1 << endl;
	fout1.close();


	cout << "---*** 私钥{d, n} ***---" << endl;
	cout << "大素数d为：" << endl;
	d.print();
	cout << endl;

	// 输出文件流，私钥
	ofstream fout2("RSA私钥.txt");
	for (int i = d.length() - 1; i >= 0; i--) {
		unsigned int temp = d.data[i];
		for (int j = 0; j < 8; j++) {
			unsigned int result = temp & 0xf0000000;
			result = (result >> 28);
			temp = (temp << 4);
			if (result >= 0 && result <= 9)
				fout2 << result;
			else
				fout2 << char('A' + result - 10);
		}
	}
	fout2 << endl;

	cout << "素数乘积n为：" << endl;
	n.print();
	cout << endl;

	for (int i = n.length() - 1; i >= 0; i--) {
		unsigned int temp = n.data[i];
		for (int j = 0; j < 8; j++) {
			unsigned int result = temp & 0xf0000000;
			result = (result >> 28);
			temp = (temp << 4);
			if (result >= 0 && result <= 9)
				fout2 << result;
			else
				fout2 << char('A' + result - 10);
		}
	}
	fout2 << endl;
	fout2.close();
}