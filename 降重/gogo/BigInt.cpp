#include <stdlib.h>
#include <string>
#include "Bigint.h"
using namespace std;


BigInt::BigInt()
{
	for (int i = 0; i < 64; i++) {
		data[i] = 0;
	}
	// Ĭ��Ϊ����
	sign = true;
}

BigInt::BigInt(const int& input) {
	// ֻ��ֵ����һλunsigned int
	for (int i = 0; i < 64; i++) {
		data[i] = 0;
	}
	data[0] = input;
	if (input >= 0)
		sign = true;
	else
		sign = false;
}

BigInt::BigInt(const char* s) {
	for (int i = 0; i < 64; i++) {
		data[i] = 0;
	}
	sign = true;
	// ����ж�s�ַ����ĳ���
	// ��������ַ���ת��ΪBigint����
	/*
	if (strlen(s) <= 128) {
		for (int i = 0; i < 16; i++) {
			// һ��unsigned int��4��16������
			for (int j = 0; j < 8; j++) {
				if (s[i * 8 + j] >= '0' && s[i * 8 + j] <= '9')
					data[15 - i] = data[15 - i] * 16 + s[i * 8 + j] - '0';
				if (s[i * 8 + j] >= 'a' && s[i * 8 + j] <= 'f')
					data[15 - i] = data[15 - i] * 16 + s[i * 8 + j] - 'a' + 10;
				if (s[i * 8 + j] >= 'A' && s[i * 8 + j] <= 'F')
					data[15 - i] = data[15 - i] * 16 + s[i * 8 + j] - 'A' + 10;
			}
		}
	}
	if (strlen(s) > 128 && strlen(s) <= 256) {
		for (int i = 0; i < 32; i++) {
			// һ��unsigned int��4��16������
			for (int j = 0; j < 8; j++) {
				if (s[i * 8 + j] >= '0' && s[i * 8 + j] <= '9')
					data[31 - i] = data[31 - i] * 16 + s[i * 8 + j] - '0';
				if (s[i * 8 + j] >= 'a' && s[i * 8 + j] <= 'f')
					data[31 - i] = data[31 - i] * 16 + s[i * 8 + j] - 'a' + 10;
				if (s[i * 8 + j] >= 'A' && s[i * 8 + j] <= 'F')
					data[31 - i] = data[31 - i] * 16 + s[i * 8 + j] - 'A' + 10;
			}
		}
	}
	*/
	int len = strlen(s) / 8;
	for (int i = 0; i < len; i++) {
		// һ��unsigned int��4��16������
		for (int j = 0; j < 8; j++) {
			if (s[i * 8 + j] >= '0' && s[i * 8 + j] <= '9')
				data[len - 1 - i] = data[len - 1 - i] * 16 + s[i * 8 + j] - '0';
			if (s[i * 8 + j] >= 'a' && s[i * 8 + j] <= 'f')
				data[len - 1 - i] = data[len - 1 - i] * 16 + s[i * 8 + j] - 'a' + 10;
			if (s[i * 8 + j] >= 'A' && s[i * 8 + j] <= 'F')
				data[len - 1 - i] = data[len - 1 - i] * 16 + s[i * 8 + j] - 'A' + 10;
		}
	}
}

BigInt::BigInt(const BigInt& input) {
	for (int i = 0; i < 64; i++) {
		data[i] = input.data[i];
	}
	sign = input.sign;
}

void BigInt::operator= (const BigInt& input) {
	for (int i = 0; i < 64; i++) {
		data[i] = input.data[i];
	}
	sign = input.sign;
}

bool operator< (const BigInt& a, const BigInt& b) {
	// �Ӹ�λ���λ���бȽ�
	for (int i = 64 - 1; i > 0; i--) {
		if (a.data[i] < b.data[i])
			return true;
		if (a.data[i] > b.data[i])
			return false;
	}
	return a.data[0] < b.data[0];
}

bool operator> (const BigInt& a, const BigInt& b) {
	for (int i = 64 - 1; i > 0; i--)
	{
		if (a.data[i] > b.data[i])
			return true;
		if (a.data[i] < b.data[i])
			return false;
	}
	// TODO
	// return false;
	return a.data[0] > b.data[0];
}

bool operator== (const BigInt& a, const BigInt& b) {
	for (int i = 0; i < 64; i++) {
		if (a.data[i] != b.data[i])
			return false;
	}
	return true;
}

bool operator== (const BigInt& a, const int& b) {
	for (int i = 1; i < a.length(); i++) {
		if (a.data[i] != 0)
			return false;
	}
	return a.data[0] == b;
}

void BigInt::operator>> (const int& a) {
	unsigned int bit;
	data[0] = (data[0] >> a);
	for (int i = 1; i < length(); i++) {
		bit = data[i] & 1;
		bit = bit << (32 - a);;
		data[i - 1] = data[i - 1] | bit;
		data[i] = (data[i] >> a);
	}
}

bool operator<= (const BigInt& a, const int& b) {
	for (int i = 1; i < a.length(); i++) {
		if (a.data[i] != 0) {
			return false;
		}
	}
	if (a.data[0] <= b)
		return true;
	else
		return false;
}

// �ӷ�����
BigInt operator+ (const BigInt& a, const BigInt& b) {
	BigInt result;
	unsigned int carry = 0; // �ӷ���λ
	int len = (a.length() >= b.length() ? a.length() : b.length());

	// ͬ�Ž��мӷ�����
	if (a.sign == b.sign) {
		for (int i = 0; i < len; i++) {
			// ������ĺ����ᣬ32λ�������32λ������Ҫʹ��64λ���������洢
			unsigned __int64 sum = (unsigned __int64)a.data[i] + b.data[i] + carry;
			result.data[i] = (unsigned int)sum; // �洢��32λ
			carry = (sum >> 32);
		}
		result.sign = a.sign;
		return result;
	}
	// ������þ���ֵ���м�������
	else {
		BigInt temp_a;
		BigInt temp_b;

		// temp_aΪ����ֵ���������������
		if (a < b) {
			temp_a = b;
			temp_b = a;
		}
		else {
			temp_a = a;
			temp_b = b;
		}

		for (int i = 0; i < len; i++) {
			unsigned int temp = temp_b.data[i] + carry;
			// ����Ҫ��λ
			if (temp_a.data[i] >= temp) {
				result.data[i] = temp_a.data[i] - temp;
				carry = 0;
			}
			else {
				unsigned __int64 borrow = 1 << 32;
				result.data[i] = borrow + (unsigned __int64)temp_a.data[i] - temp;
				carry = 1;
			}
		}
		result.sign = temp_a.sign;
		return result;
	}
}

// ��������
BigInt operator- (const BigInt& a, const BigInt& b) {
	BigInt result;
	unsigned int carry = 0;

	// ͬ���þ���ֵ���м�����ȡa��b�о���ֵ�ϴ������Ϊ������
	if (a.sign == b.sign) {
		BigInt temp_a, temp_b;
		// �����BigInt���͵ı���>��<���ţ���Ϊ����ֵ���бȽ�
		if (a < b) {
			temp_a = b;
			temp_b = a;
			result.sign = !temp_a.sign; // a����ֵ��С������ȡ��
		}
		else {
			temp_a = a;
			temp_b = b;
			result.sign = temp_a.sign;
		}

		for (int i = 0; i < 64; i++) {
			unsigned int sub = temp_b.data[i] + carry;
			if (temp_a.data[i] >= sub) {
				result.data[i] = temp_a.data[i] - sub;
				carry = 0;
			}
			else {
				unsigned __int64 borrow = 1 << 32;
				result.data[i] = borrow + (unsigned __int64)temp_a.data[i] - sub;
				carry = 1;
			}
		}
		return result;
	}

	// ���ʱ����-����Ϊ+������-����Ϊ-
	else {
		for (int i = 0; i < 64; i++) {
			unsigned __int64 sum = (unsigned __int64)a.data[i] + b.data[i] + carry;
			result.data[i] = (unsigned int)sum;
			carry = (sum >> 32);
		}
		result.sign = a.sign;
		return result;
	}
}

BigInt operator* (const BigInt& a, const unsigned int& b) {
	BigInt result;
	// TODO
	// unsigned __int64 sum;
	unsigned int carry = 0;

	for (int i = 0; i < 64; i++) {
		unsigned __int64 sum = ((unsigned __int64)a.data[i]) * b + carry;
		result.data[i] = (unsigned int)sum;
		carry = (sum >> 32);
	}
	result.sign = a.sign;
	return result;
}

// �˷�����
BigInt operator* (const BigInt& a, const BigInt& b) {
	BigInt result, last; // ��������һ�ε�����������
	unsigned int carry;

	if (a.sign == b.sign) {
		result.sign = true;
	}
	else {
		result.sign = false;
	}

	// �ο�x�˷�
	for (int i = 0; i < b.length(); i++) {
		BigInt temp; // ��ǰ��һ�еļ�����
		carry = 0;
		for (int j = 0; j < a.length() + 1; j++) {
			unsigned __int64 sum = ((unsigned __int64)a.data[j]) * (b.data[i]) + carry;
			// ��i��j������λ�ã���û�п������������
			if ((i + j) < 64) {
				temp.data[i + j] = (unsigned int)sum;
			}
			carry = (sum >> 32);
		}
		result = (temp + last);
		last = result;
	}
	return result;
}

// ��������
BigInt operator/ (const BigInt& a, const BigInt& b) {
	BigInt quotient;
	if (a.sign == b.sign) {
		quotient.sign = true;
	}
	else {
		quotient.sign = false;
	}
	if (a < b) {
		return quotient;
	}

	int length_a = a.length();
	int length_b = b.length();
	// �����������������洢����Ϊ�µı�������������
	BigInt divide;
	for (int i = 0; i < length_b; i++) {
		divide.data[i] = a.data[length_a - length_b + i];
	}
	for (int i = length_a - length_b; i >= 0; i--) {
		//���������С�ڳ�����������λ
		if (divide < b) {
			for (int j = length_b; j > 0; j--) {
				divide.data[j] = divide.data[j - 1];
			}
			divide.data[0] = a.data[i - 1];
			continue;
		}

		unsigned int temp_try;
		unsigned int left = 0;
		unsigned int right = 0xffffffff;
		BigInt current, next;
		// ���ֲ��ҷ�����
		while (left < right) {
			temp_try = (((unsigned __int64)right) + left) / 2;
			current = (b * temp_try); // �����뵱ǰ���̵Ļ�
			next = (b * (temp_try + 1)); // ������ ��ǰ���̣�1 �Ļ�
			//�ɹ��ҵ��̣�Ϊcurrent
			if (((current < divide) && (next > divide)) || (current == divide))
				break;
			//�ɹ��ҵ��̣�Ϊnext
			if (next == divide) {
				temp_try++;
				current = next;
				break;
			}
			//û�ҵ����̱ȵ�ǰ���̴󣬸��¶��ֲ��ҵ�����
			if ((current < divide) && (next < divide)) {
				left = temp_try;
				continue;
			}
			//û�ҵ����̱ȵ�ǰ����С�����¶��ֲ��ҵ�����
			if ((current > divide) && (next > divide)) {
				right = temp_try;
				continue;
			}
		}

		quotient.data[i] = temp_try;
		// ���±�����
		divide = divide - current;
		if ((i - 1) >= 0) {
			for (int j = length_b; j > 0; j--) {
				divide.data[j] = divide.data[j - 1];
			}
			divide.data[0] = a.data[i - 1];
		}
	}

	return quotient;
}

// ��ģ����
BigInt operator% (const BigInt& a, const BigInt& b) {
	BigInt divide;
	// �����ķ����뱻����a��ͬ
	divide.sign = a.sign;
	if (a < b) {
		divide = a;
		return divide;
	}
	BigInt quotient;
	int length_a = a.length();
	int length_b = b.length();

	for (int i = 0; i < length_b; i++) {
		divide.data[i] = a.data[length_a - length_b + i];
	}
	for (int i = length_a - length_b; i >= 0; i--) {
		// ���������С�ڳ���,������λ
		if (divide < b) {
			for (int j = length_b; j > 0; j--) {
				divide.data[j] = divide.data[j - 1];
			}
			divide.data[0] = a.data[i - 1];
			continue;
		}

		unsigned int temp_try;
		unsigned int left = 0;
		unsigned int right = 0xffffffff;
		BigInt current, next;
		// ���ֲ��ҷ����̣�ͬ��Ӧ����ȡģ����֮��
		while (left < right) {
			temp_try = (((unsigned __int64)right) + left) / 2;
			current = (b * temp_try);
			next = (b * (temp_try + 1));
			if (((current < divide) && (next > divide)) || (current == divide)) {
				break;
			}
			if (next == divide) {
				temp_try++;
				current = next;
				break;
			}
			if ((current < divide) && (next < divide)) {
				left = temp_try;
				continue;
			}
			if ((current > divide) && (next > divide)) {
				right = temp_try;
				continue;
			}
		}
		quotient.data[i] = temp_try; // ���棨32bit����

		// ���±�����
		divide = divide - current;
		if ((i - 1) >= 0) {
			for (int j = length_b; j > 0; j--) {
				divide.data[j] = divide.data[j - 1];
			}
			divide.data[0] = a.data[i - 1];
		}
	}

	// ��ǰ��������Ϊ��������
	divide.sign = a.sign;
	return divide;
}

// ��ģ���� a^n mod p
BigInt Pow_Mod(const BigInt& a, const BigInt& n, const BigInt& p) {
	BigInt new_n = n, base = a % p;
	BigInt d(1);
	// ����ģ����ת��Ϊģ������
	while (!(new_n <= 1)) {
		// ȥ����λ��0��1�Ų�������
		if (new_n.IsOdd()) {
			d = (d * base) % p;
		}
		base = (base * base) % p;
		new_n >> 1;
	}
	return (base * d) % p;
}

void BigInt::print() const {
	// ������ÿ4bitת��һ��16���������
	for (int i = length() - 1; i >= 0; i--) {
		unsigned int temp = data[i];
		for (int j = 0; j < 8; j++) {
			unsigned int result = temp & 0xf0000000;
			result = (result >> 28);
			temp = (temp << 4);
			if (result >= 0 && result <= 9)
				cout << result;
			else
				cout << char('A' + result - 10);
		}
		cout << " ";
	}
	cout << endl;
}

string BigInt::tostr() {
	// ��BigInt��������ת��Ϊstring���͵�
	string str_result;
	for (int i = length() - 1; i >= 0; i--) {
		unsigned int temp = data[i];
		for (int j = 0; j < 8; j++) {
			unsigned int result = temp & 0xf0000000;
			result = (result >> 28);
			temp = (temp << 4);
			if (result >= 0 && result <= 9)
				str_result = str_result + to_string(result);
			else
				str_result = str_result + char('A' + result - 10);
		}
		// ���ܼӿո�������˿ո�͵��ڴ���������鷳
		// str_result = str_result + " ";
	}
	return str_result;
}

void BigInt::MakeEmpty() {
	for (int i = 0; i < 64; i++) {
		data[i] = 0;
	}
}

int BigInt::length() const {
	int length = 64;
	for (int i = 63; i >= 0; i--) {
		if (data[i] == 0)
			length--;
		else
			break;
	}
	return length;
}
