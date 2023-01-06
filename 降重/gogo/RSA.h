#include <iostream>
#include <fstream>
#include <time.h>
#include <ctime>
#include <string>
#include "BigInt.h"
using namespace std;

char hex_table[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

// ���ܺ���
BigInt Encrypt(BigInt& m, BigInt& e, BigInt& n) {
	return Pow_Mod(m, e, n);
}

// ���ܺ���
BigInt Decrypt(BigInt& c, BigInt& d, BigInt& n) {
	return Pow_Mod(c, d, n);
}

// ����һ��512bit�Ĵ�������������������ɸһ��
bool GenOdd(BigInt& n) {
	char s[129]; // 129λ����\0
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
		// �������
		ans = a - BigInt(1);
	}
	return ans;
}

// RabinMiller���Լ�������
bool RabinMiller(const BigInt& n) {
	BigInt d = n - 1, a, y;
	unsigned int s = 0;
	// d = n - 1;

	//s��¼����nĩβ����ĸ���������d���� 
	while (!d.IsOdd()) {
		s++;
		d >> 1;
	}

	for (int i = 0; i < 5; i++) {
		a = CreateRandom(n - 1);
		y = Pow_Mod(a, d, n);  // y = a^d mod n
		// �������a^d mod n != -1����ôn����Ϊ����
		if (y == 1) {
			continue;
		}
		bool prob = true;
		// �������������е�j��[0,s-1]��a^(2^j*d) mod n != -1����ônΪ����
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

	return true;    // ͨ�����ֲ���
}

// ����һ��������
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

// ŷ������㷨�����Լ��
BigInt Gcd(const BigInt& m, const BigInt& n) {
	if (n == 0)
		return m;
	else
		return Gcd(n, m % n);
}

// ��չŷ������㷨��˷���Ԫ
BigInt ExtendedGcd(const BigInt& a, const BigInt& b, BigInt& x, BigInt& y) {
	BigInt t, d;
	// ���һ��������Ϊ�����޷����г�������
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
	// �������������
	srand((unsigned)time(NULL));
	cout << endl << "---*** �������ɴ�����(512bit) ***---" << endl << endl;
	clock_t t = clock();
	cout << "---*** ������p ***---" << endl;
	BigInt p = GenPrime();
	p.print();
	cout << endl;

	cout << "---*** ������q ***---" << endl;
	BigInt q = GenPrime();
	q.print();
	cout << endl;

	cout << "---*** ���ɴ�������ʱ ***---" << endl;
	cout << (clock() - t) / 1000 << "s" << endl;

	cout << endl << "---*** �������ɹ�Կ��˽Կ ***---" << endl << endl;
	cout << "---*** ������֮��n ***---" << endl;
	BigInt n = p * q;
	n.print();
	cout << endl;

	BigInt fn = (p - 1) * (q - 1);
	BigInt e, d, temp;
	while (1) {
		// ����һ����fn���صĴ�����e
		do {
			GenOdd(e);
		} while (!(Gcd(e, fn) == 1));

		// ����չŷ������㷨��ͼ���eģfn�ĳ˷���Ԫ,������d��
		ExtendedGcd(e, fn, d, temp);

		// �������ĳ˷���Ԫ����ȷ�ԣ�����ȷ����������
		BigInt temp1 = (e * d) % fn;
		if (temp1 == 1) {
			break;
		}
	}
	cout << "---*** ��Կ{e, n} ***---" << endl;
	cout << "������eΪ��" << endl;
	e.print();
	cout << endl;

	// ����ļ�������Կ
	ofstream fout1("RSA��Կ.txt");
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


	cout << "�����˻�nΪ��" << endl;
	n.print();
	cout << endl;
	// ofstream fout("RSA��Կ.txt");
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


	cout << "---*** ˽Կ{d, n} ***---" << endl;
	cout << "������dΪ��" << endl;
	d.print();
	cout << endl;

	// ����ļ�����˽Կ
	ofstream fout2("RSA˽Կ.txt");
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

	cout << "�����˻�nΪ��" << endl;
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