#include <stdlib.h>
#include <string>
#include "Bigint.h"
using namespace std;


BigInt::BigInt()
{
	for (int i = 0; i < 64; i++) {
		data[i] = 0;
	}
	// 默认为正数
	sign = true;
}

BigInt::BigInt(const int& input) {
	// 只赋值给第一位unsigned int
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
	// 添加判断s字符串的长度
	// 将输入的字符串转化为Bigint类型
	/*
	if (strlen(s) <= 128) {
		for (int i = 0; i < 16; i++) {
			// 一个unsigned int是4个16进制数
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
			// 一个unsigned int是4个16进制数
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
		// 一个unsigned int是4个16进制数
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
	// 从高位向低位进行比较
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

// 加法运算
BigInt operator+ (const BigInt& a, const BigInt& b) {
	BigInt result;
	unsigned int carry = 0; // 加法进位
	int len = (a.length() >= b.length() ? a.length() : b.length());

	// 同号进行加法运算
	if (a.sign == b.sign) {
		for (int i = 0; i < len; i++) {
			// 这里真的很讨厌，32位加完大于32位所以需要使用64位的类型来存储
			unsigned __int64 sum = (unsigned __int64)a.data[i] + b.data[i] + carry;
			result.data[i] = (unsigned int)sum; // 存储低32位
			carry = (sum >> 32);
		}
		result.sign = a.sign;
		return result;
	}
	// 异号则用绝对值进行减法运算
	else {
		BigInt temp_a;
		BigInt temp_b;

		// temp_a为绝对值大的数，即被减数
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
			// 不需要借位
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

// 减法运算
BigInt operator- (const BigInt& a, const BigInt& b) {
	BigInt result;
	unsigned int carry = 0;

	// 同号用绝对值进行减法，取a，b中绝对值较大的数作为被减数
	if (a.sign == b.sign) {
		BigInt temp_a, temp_b;
		// 这里的BigInt类型的变量>和<符号，均为绝对值进行比较
		if (a < b) {
			temp_a = b;
			temp_b = a;
			result.sign = !temp_a.sign; // a绝对值较小，符号取反
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

	// 异号时正数-负数为+，负数-正数为-
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

// 乘法运算
BigInt operator* (const BigInt& a, const BigInt& b) {
	BigInt result, last; // 结果和最近一次的运算结果、积
	unsigned int carry;

	if (a.sign == b.sign) {
		result.sign = true;
	}
	else {
		result.sign = false;
	}

	// 参考x乘法
	for (int i = 0; i < b.length(); i++) {
		BigInt temp; // 当前这一行的计算结果
		carry = 0;
		for (int j = 0; j < a.length() + 1; j++) {
			unsigned __int64 sum = ((unsigned __int64)a.data[j]) * (b.data[i]) + carry;
			// 用i和j来调整位置，并没有考虑溢出的事项
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

// 除法运算
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
	// 将被除数与除数对齐存储，作为新的被除数参与运算
	BigInt divide;
	for (int i = 0; i < length_b; i++) {
		divide.data[i] = a.data[length_a - length_b + i];
	}
	for (int i = length_a - length_b; i >= 0; i--) {
		//如果被除数小于除数，再往后补位
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
		// 二分查找法试商
		while (left < right) {
			temp_try = (((unsigned __int64)right) + left) / 2;
			current = (b * temp_try); // 除数与当前试商的积
			next = (b * (temp_try + 1)); // 除数与 当前试商＋1 的积
			//成功找到商，为current
			if (((current < divide) && (next > divide)) || (current == divide))
				break;
			//成功找到商，为next
			if (next == divide) {
				temp_try++;
				current = next;
				break;
			}
			//没找到，商比当前试商大，更新二分查找的下限
			if ((current < divide) && (next < divide)) {
				left = temp_try;
				continue;
			}
			//没找到，商比当前试商小，更新二分查找的上限
			if ((current > divide) && (next > divide)) {
				right = temp_try;
				continue;
			}
		}

		quotient.data[i] = temp_try;
		// 更新被除数
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

// 求模运算
BigInt operator% (const BigInt& a, const BigInt& b) {
	BigInt divide;
	// 余数的符号与被除数a相同
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
		// 如果被除数小于除数,再往后补位
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
		// 二分查找法试商，同样应用于取模运算之中
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
		quotient.data[i] = temp_try; // 保存（32bit）商

		// 更新被除数
		divide = divide - current;
		if ((i - 1) >= 0) {
			for (int j = length_b; j > 0; j--) {
				divide.data[j] = divide.data[j - 1];
			}
			divide.data[0] = a.data[i - 1];
		}
	}

	// 当前被除数即为所求余数
	divide.sign = a.sign;
	return divide;
}

// 幂模运算 a^n mod p
BigInt Pow_Mod(const BigInt& a, const BigInt& n, const BigInt& p) {
	BigInt new_n = n, base = a % p;
	BigInt d(1);
	// 将幂模运算转换为模乘运算
	while (!(new_n <= 1)) {
		// 去掉低位的0，1才参与运算
		if (new_n.IsOdd()) {
			d = (d * base) % p;
		}
		base = (base * base) % p;
		new_n >> 1;
	}
	return (base * d) % p;
}

void BigInt::print() const {
	// 遍历，每4bit转成一个16进制数输出
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
	// 将BigInt数据类型转换为string类型的
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
		// 不能加空格，如果加了空格就得在传输后处理，很麻烦
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
