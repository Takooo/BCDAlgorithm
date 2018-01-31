#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef unsigned int word;
int len1 = 0;
int len2 = 0;
int FLAG = 0;
word atom(char p);
word mtoa(word *O, char *p, word L);         //二进制转换成字符串。
word madd(word *B1, word *B2, word *O);    //二进制所表示数的加法。
word msub(word *B1, word *B2, word *O);    //减法。
word mmul(word *B1, word *B2, word *O);    // 乘法。
word mdiv(word *B1, word *B2, word *O);    //除法。
word mmod(word *B1, word *B2, word *O);    // 取余。


int main()
{
	int i;
	int j = 0;
	int len = 0;
	word L;									//数组长度
	char s[1000];							//读取字符串存放数组
	char n1[1000];							
	char n2[1000];
	char F[1000];							//BCD转字符串存放数组
	word B1[1000];							//运算符前操作数BCD码
	word B2[1000];							//运算符后操作数BCD码
	word O[1000] = { 0 };					//结果存放数组
	for (i = 0; i <= 999; i++)				//初始化
	{
		B1[i] = 0;
		B2[i] = 0;
		O[i] = 0;
	}
	scanf("%s", &s);						//读取字符串
	while (s[++len])						//找到运算符位置
		if (s[len] == '+' || s[len] == '-' || s[len] == '*' || s[len] == '/' || s[len] == '%')
			break;
	strncpy(n1, s, len);					//运算符前字符串
	n1[len] = NULL;
	strcpy(n2, &s[len + 1]);				//运算符后字符串
	for (i = 0; i < len; i++)
		B1[i] = atom(s[i]);					//转换成BCD码
	for (i = len + 1; i < strlen(s); i++)
		B2[j++] = atom(s[i]);				//转换成BCD码
	len1 = len - 1;							//BCD码数组长度
	len2 = strlen(s) - len - 2;				//BCD码数组长度
	if (s[len] == '+')						//不同运算符进行不同运算
		L = madd(B1, B2, O);
	else if (s[len] == '-')					//减法需判断正负以及借位要考虑减去0110修正
	{
		L = msub(B1, B2, O);
		for (i = 0; i <= 999; i++)
		{
			B1[i] = O[i];
			B2[i] = 0;
			O[i] = 0;
		}
		for (i = 0; i <= L; i++)
		{
			if (B1[i] > 1001)
				B2[i] = 110;
		}
		len1 = len2 = L;
		if (FLAG == 1)
			printf("-");
		FLAG = 0;
		L = msub(B1, B2, O);
	}
	else if (s[len] == '*')
		L = mmul(B1, B2, O);
	else if (s[len] == '/')
		L = mdiv(B1, B2, O);
	else if (s[len] == '%')
		L = mmod(B1, B2, O);  
	L = mtoa(O, F, L);								//BCD码转回字符串
	for (i = 0; i <= L; i++)
		printf("%c", F[i]);
	printf("\n");
	return 0;
}

word atom(char p)
{
	int B = 0;
	int m;
	int temp;
	int num = p - '0';
	for (int j = 0; j <= 3; j++)
	{
		m = j;
		temp = num % 2;							
		while (m)
		{
			temp *= 10;
			m--;
		}
		B += temp;
		num /= 2;
	}

	return B;
}

word madd(word *B1, word *B2, word *O)
{
	int i = 0;
	int j;
	while (len1 > len2)					//高位不同转化为相同
	{
		O[i] = B1[i];
		i++;
		len1--;
		for (j = len2 + i; j >=i; j--)
			B2[j] = B2[j - 1];
		B2[0] = 0;
	}
	while (len1 < len2)
	{
		O[i] = B2[i];
		i++;
		len2--;
		for (j = len2 + i; j >= i; j--)
			B1[j] = B1[j - 1];
		B1[0] = 0;
	}
	j = i;
	for (i = j + len1; i >= j; i--)				//高位相同
	{
		O[i] += B1[i] + B2[i];					//相加
		if (i != 0)
		{
			if (O[i] % 10 >= 2)					//二进制修正
				O[i] += 8;
			if ((O[i] / 10) % 10 >= 2)
				O[i] = O[i] + (((O[i] / 10) % 10) / 2) * 100 - (((O[i] / 10) % 10) / 2) * 20;
			if ((O[i] / 100) % 10 >= 2)
				O[i] = O[i] + (((O[i] / 100) % 10) / 2) * 1000 - (((O[i] / 100) % 10) / 2) * 200;
			if ((O[i] / 1000) % 10 >= 2)
				O[i] = O[i] + (((O[i] / 1000) % 10) / 2) * 10000 - (((O[i] / 1000) % 10) / 2) * 2000;
			if ((O[i] / 10000) % 10 >= 2)
				O[i] = O[i] + (((O[i] / 10000) % 10) / 2) * 100000 - (((O[i] / 10000) % 10) / 2) * 20000;
			if ((O[i] / 100000) % 10 >= 2)
				O[i] = O[i] + (((O[i] / 100000) % 10) / 2) * 1000000 - (((O[i] / 100000) % 10) / 2) * 200000;

			if (O[i] >= 1010)							//大于10修正
			{
				O[i - 1] += 1;
				if((O[i]/10)%10<1)
				{
					O[i] += 10;
					if ((O[i] / 100) % 10 >= 1)
						O[i] -= 100;
					else if ((O[i] / 1000) % 10 >= 1)
					{
						O[i] += 100;
						O[i] -= 1000;
					}
					else if ((O[i] / 10000) % 10 >= 1)
					{
						O[i] -= 10000;
						O[i] += 1000;
						O[i] += 100;
					}
				}
				else O[i] -= 10;
				if((O[i]/1000)%10<1)
				{
					O[i] + 1000;
					if ((O[i] / 10000) % 10 >= 1)
					{
						O[i] -= 10000;
					}
				}
				else O[i] -= 1000;
			}
		}

	}

	if (O[0] % 10 >= 2)							//最高位二进制修正
		O[0] += 8;
	if ((O[0] / 10) % 10 >= 2)
		O[0] = O[0] + (((O[0] / 10) % 10) / 2) * 100 - (((O[0] / 10) % 10) / 2) * 20;
	if ((O[0] / 100) % 10 >= 2)
		O[0] = O[0] + (((O[0] / 100) % 10) / 2) * 1000 - (((O[0] / 100) % 10) / 2) * 200;
	if ((O[0] / 1000) % 10 >= 2)
		O[0] = O[0] + (((O[0] / 1000) % 10) / 2) * 10000 - (((O[0] / 1000) % 10) / 2) * 2000;
	if ((O[0] / 10000) % 10 >= 2)
		O[0] = O[0] + (((O[0] / 10000) % 10) / 2) * 100000 - (((O[0] / 10000) % 10) / 2) * 20000;
	if (O[0]>=1010)									//最高位大于10修正
	{

			if ((O[0] / 10) % 10<1)
			{
				O[0] += 10;
				if ((O[0] / 100) % 10 >= 1)
					O[0] -= 100;
				else if ((O[0] / 1000) % 10 >= 1)
				{
					O[0] += 100;
					O[0] -= 1000;
				}
				else if ((O[0] / 10000) % 10 >= 1)
				{
					O[0] -= 10000;
					O[0] += 1000;
					O[0] += 100;
				}
			}
			else O[0] -= 10;
			if ((O[0] / 1000) % 10<1)
			{
				O[0] + 1000;
				if ((O[0] / 10000) % 10 >= 1)
				{
					O[0] -= 10000;
				}
			}
			else O[0] -= 1000;
		for (i = j + len1 + 1; i > 0; i--)
			O[i] = O[i - 1];
		O[i] = 1;
		j++;
	}
	return (j + len1);
}

word msub(word *B1, word *B2, word *O)
{
	char C1[1000];
	char C2[1000];
	int i = 0;
	int j;
	int temp, t;
	int f = 0;
	for(i=0;i<=999;i++)							//初始化
	{
		C1[i] = B1[i];
		C2[i] = B2[i];
		O[i] = 0;
	}

	i = 0;
	while (len1 > len2)							//被减数位数大于减数修正位数相同
	{
		O[i] = B1[i];
		i++;
		len1--;
		for (j = len2 + i; j >= i; j--)
			B2[j] = B2[j - 1];
		B2[0] = 0;
	}
	while (len1 < len2)							//减数位数大于被减数修正位数相同且判断结果为负数
	{
		O[i] = B2[i];
		i++;
		len2--;
		for (j = len1 + i; j >= i ; j--)
			B1[j] = B1[j - 1];
		B1[0] = 0;
		FLAG = 1;
	}
	j = i;

	if (FLAG == 1)								//结果为负数的数，用减数减去被减数，最终结果加负号
	{
		for (i = j + len1; i >= 0; i--)
		{
			O[i] = 0;
			temp = B2[i] % 10 - B1[i] % 10 - f;		//每个二进制位逐位相减
			f = 0;
			if (temp < 0)							//判断借位
			{	
				temp += 2;
				if (B2[i] % 100 >= 10)
					B2[i] -= 10;
				else if (B2[i] % 1000 >= 100)
				{
					B2[i] -= 100;
					B2[i] += 10;
				}
				else if (B2[i] % 10000 >= 1000)
				{
					B2[i] -= 1000;
					B2[i] += 100;
					B2[i] += 10;
				}
				else									//本位二进制位无法借位，则向上一位借位
				{
					f = 1;
					B2[i] += 1000;						//这里修正过，向上一位借位后，实际借1010
				}
			}
			O[i] += temp;
			temp = (B2[i] / 10) % 10 - (B1[i] / 10) % 10;
			if (temp < 0)
			{
				temp += 2;
				if (B2[i] % 1000 >= 100)
					B2[i] -= 100;

				else if (B2[i] % 10000 >= 1000)
				{
					B2[i] -= 1000;
					B2[i] += 100;
				}
				else
				{
					f = 1;
					B2[i] += 1000;							
					temp = 0;
				}
			}
			temp *= 10;
			O[i] += temp;
			temp = (B2[i] / 100) % 10 - (B1[i] / 100) % 10;
			if (temp < 0)
			{
				temp += 2;
				if (B2[i] % 10000 >= 1000)
					B2[i] -= 1000;
				else
				{
					f = 1;
					B2[i] += 10;
					if (B2[i] % 10 >= 2)
						B2[i] += 8;
					if ((B2[i] / 10) % 10 >= 2)
						B2[i] += 80;
					if ((B2[i] / 100) % 10 >= 2)
						B2[i] += 800;
					if ((B2[i] / 1000) >= 2)
						B2[i] += 8000;
					O[i] += 10;
				}
			}
			temp *= 100;
			O[i] += temp;
			if (O[i] % 10 >= 2)
				O[i] += 8;
			if ((O[i] / 10) % 10 >= 2)
				O[i] += 80;
			if ((O[i] / 100) % 10 >= 2)
				O[i] += 800;
			if ((O[i] / 1000) >= 2)
				O[i] += 8000;
			temp = (B2[i] / 1000) % 10 - (B1[i] / 1000) % 10;
			if (temp < 0)
			{
				temp += 1;
				B2[i] += 10;
				if (B2[i] % 10 >= 2)
					B2[i] += 8;
				if ((B2[i] / 10) % 10 >= 2)
					B2[i] += 80;
				if ((B2[i] / 100) % 10 >= 2)
					B2[i] += 800;
				if ((B2[i] / 1000) >= 2)
					B2[i] += 8000;
				f = 1;
				O[i] += 10;
			}
			temp *= 1000;
			O[i] += temp;
			if (O[i] % 10 >= 2)
				O[i] += 8;
			if ((O[i] / 10) % 10 >= 2)
				O[i] += 80;
			if ((O[i] / 100) % 10 >= 2)
				O[i] += 800;
			if ((O[i] / 1000) >= 2)
				O[i] += 8000;
		}
	}
	if (FLAG == 0)											//无法根据位数判断正负的情况
	{
		for (i = j + len1; i >= 0; i--)
		{
			if (i < j)
			{
				O[i] = 0;
			}
			temp = B1[i] % 10 - B2[i] % 10 - f;
			f = 0;
			if (temp < 0)
			{
				temp += 2;
				if (B1[i] % 100 >= 10)
				{
					B1[i] -= 10;
				}
				else if (B1[i] % 1000 >= 100)
				{
					B1[i] -= 100;
					B1[i] += 10;
				}
				else if (B1[i] % 10000 >= 1000)
				{
					B1[i] -= 1000;
					B1[i] += 100;
					B1[i] += 10;
				}
				else if (B1[i] % 100000 >= 10000)
				{
					B1[i] -= 10000;
					B1[i] += 1000;
					B1[i] += 100;
					B1[i] += 10;
				}
				else
				{
					f = 1;
					B1[i] += 1000;
				}
			}
			O[i] += temp;
			temp = (B1[i] / 10) % 10 - (B2[i] / 10) % 10;
			if (temp < 0)
			{
				temp += 2;
				if (B1[i] % 1000 >= 100)
				{
					B1[i] -= 100;
				}

				else if (B1[i] % 10000 >= 1000)
				{
					B1[i] -= 1000;
					B1[i] += 100;
				}
				else if (B1[i] % 100000 >= 10000)
				{
					B1[i] -= 10000;
					B1[i] += 1000;
					B1[i] += 100;
				}
				else
				{
					f = 1;
					temp = 0;
					B1[i] += 1000;
				}
			}
			temp *= 10;
			O[i] += temp;
			temp = (B1[i] / 100) % 10 - (B2[i] / 100) % 10;
			if (temp < 0)
			{
				temp += 2;
				if (B1[i] % 10000 >= 1000)
					B1[i] -= 1000;
				else if (B1[i] % 100000 >= 10000)
				{
					B1[i] -= 10000;
					B1[i] += 1000;
				}
				else
				{
					f = 1;
					B1[i] += 10;
					if (B1[i] % 10 >= 2)
						B1[i] += 8;
					if ((B1[i] / 10) % 10 >= 2)
						B1[i] += 80;
					if ((B1[i] / 100) % 10 >= 2)
						B1[i] += 800;
					if ((B1[i] / 1000) >= 2)
						B1[i] += 8000;
					O[i] += 10;
				}
			}
			temp *= 100;
			O[i] += temp;
			if (O[i] % 10 >= 2)
				O[i] += 8;
			if ((O[i] / 10) % 10 >= 2)
				O[i] += 80;
			if ((O[i] / 100) % 10 >= 2)
				O[i] += 800;
			if ((O[i] / 1000) >= 2)
				O[i] += 8000;

			temp = (B1[i] / 1000) % 10 - (B2[i] / 1000) % 10;
			if (temp < 0)
			{
				temp += 2;
				if (B1[i] % 100000 >= 10000)
				{
					B1[i] -= 10000;
				}
				else
				{
					f = 1;
					temp = 0;
					B1[i] += 10;
					if (B1[i] % 10 >= 2)
						B1[i] += 8;
					if ((B1[i] / 10) % 10 >= 2)
						B1[i] += 80;
					if ((B1[i] / 100) % 10 >= 2)
						B1[i] += 800;
					if ((B1[i] / 1000) >= 2)
						B1[i] += 8000;
					O[i] += 10;
				}
			}
			temp *= 1000;
			O[i] += temp;
			if (O[i] % 10 >= 2)
				O[i] += 8;
			if ((O[i] / 10) % 10 >= 2)
				O[i] += 80;
			if ((O[i] / 100) % 10 >= 2)
				O[i] += 800;
			if ((O[i] / 1000) >= 2)
				O[i] += 8000;
			temp = (B1[i] / 10000) % 10 - (B2[i] / 10000) % 10;
			temp *= 10000;
			O[i] += temp;
		}


	}
	if (f == 1)							//位数相同但被减数小于减数，则记为负数，重新用减数减去被减数
	{
		f = 0;
		FLAG = 1;

		for (i = j + len1; i >= 0; i--)
		{
	
			O[i] = 0;
			temp = C2[i] % 10 - C1[i] % 10 - f;
			f = 0;
			if (temp < 0)
			{
				temp += 2;
				if (C2[i] % 100 >= 10)
					C2[i] -= 10;
				else if (C2[i] % 1000 >= 100)
				{
					C2[i] -= 100;
					C2[i] += 10;
				}
				else if (C2[i] % 10000 >= 1000)
				{
					C2[i] -= 1000;
					C2[i] += 100;
					C2[i] += 10;
				}
				else
				{
					f = 1;
					C2[i] += 1000;
				}
			}
			O[i] += temp;
			temp = (C2[i] / 10) % 10 - (C1[i] / 10) % 10;
			if (temp < 0)
			{
				temp += 2;
				if (C2[i] % 1000 >= 100)
					C2[i] -= 100;

				else if (C2[i] % 10000 >= 1000)
				{
					C2[i] -= 1000;
					C2[i] += 100;
				}
				else
				{
					f = 1;
					temp = 0;
					C2[i] += 1000;
				}
			}
			temp *= 10;
			O[i] += temp;
			temp = (C2[i] / 100) % 10 - (C1[i] / 100) % 10;
			if (temp < 0)
			{
				temp += 2;
				if (C2[i] % 10000 >= 1000)
					C2[i] -= 1000;
				else
				{
					f = 1;
					C2[i] += 10;
					if (C2[i] % 10 >= 2)
						C2[i] += 8;
					if ((C2[i] / 10) % 10 >= 2)
						C2[i] += 80;
					if ((C2[i] / 100) % 10 >= 2)
						C2[i] += 800;
					if ((C2[i] / 1000) >= 2)
						C2[i] += 8000;
					O[i] += 10;
				}
			}
			temp *= 100;
			O[i] += temp;
			if (O[i] % 10 >= 2)
				O[i] += 8;
			if ((O[i] / 10) % 10 >= 2)
				O[i] += 80;
			if ((O[i] / 100) % 10 >= 2)
				O[i] += 800;
			if ((O[i] / 1000) >= 2)
				O[i] += 8000;
			temp = (C2[i] / 1000) % 10 - (C1[i] / 1000) % 10;
			if (temp < 0)
			{
				temp += 1;
				C2[i] += 10;
				if (C2[i] % 10 >= 2)
					C2[i] += 8;
				if ((C2[i] / 10) % 10 >= 2)
					C2[i] += 80;
				if ((C2[i] / 100) % 10 >= 2)
					C2[i] += 800;
				if ((C2[i] / 1000) >= 2)
					C2[i] += 8000;
				O[i] += 10;
				f = 1;
			}
			temp *= 1000;
			O[i] += temp;
			if (O[i] % 10 >= 2)
				O[i] += 8;
			if ((O[i] / 10) % 10 >= 2)
				O[i] += 80;
			if ((O[i] / 100) % 10 >= 2)
				O[i] += 800;
			if ((O[i] / 1000) >= 2)
				O[i] += 8000;
		}
	}
	t = 0;
	while (O[0] == 0)
	{
		for (i = 0; i <= j + len1 - t; i++)
			O[i] = O[i + 1];
		t++;
		if(t>100)
		{
			t = len1 + j;
			break;
		}
	}
	return (j + len1 - t);								//返回位数
}

word mmul(word *B1, word *B2, word *O)    
{
	int i, j;
	int l;
	l = len1 + len2;

	if (len1 <= len2)							//乘法是逐位相乘相加再修正
	{
		for (i = len2; i >= 0; i--)
			for (j = len1; j >= 0; j--)
			{
				O[i + j] += B1[j] * B2[i];
			}
	}
	if (len1 > len2)
	{
		for (i = len2; i >= 0; i--)
			for (j = len1; j >= 0; j--)
			{
				O[i + j] += B1[j] * B2[i];
			}
	}

	i = l;
	for (i = l; i >= 1; i--)
	{
		if (O[i] % 10 >= 2)
			O[i] += 8;
		if ((O[i] / 10) % 10 >= 2)
			O[i] = O[i] + (((O[i] / 10) % 10) / 2) * 100 - (((O[i] / 10) % 10) / 2 )*20;
		if ((O[i] / 100) % 10 >= 2)
			O[i] = O[i] + (((O[i] / 100) % 10) / 2) * 1000 - (((O[i] / 100) % 10) / 2) * 200;
		if ((O[i] / 1000) % 10 >= 2)
			O[i] = O[i] + (((O[i] / 1000) % 10) / 2) * 10000 - (((O[i] / 1000) % 10) / 2) * 2000;
		if ((O[i] / 10000) % 10 >= 2)
			O[i] = O[i] + (((O[i] / 10000) % 10) / 2) * 100000 - (((O[i] / 10000) % 10) / 2) * 20000;
		if ((O[i] / 100000) % 10 >= 2)
			O[i] = O[i] + (((O[i] / 100000) % 10) / 2) * 1000000 - (((O[i] / 100000) % 10) / 2) * 200000;
		if ((O[i] / 1000000) % 10 >= 2)
			O[i] = O[i] + (((O[i] / 1000000) % 10) / 2) * 10000000 - (((O[i] / 1000000) % 10) / 2) * 2000000;
		if (O[i] % 10000)
		{
			O[i - 1] += O[i] / 10000;
			O[i] -= (O[i] / 10000) * 10000;
		}
	}
	i = 0;
	if (O[i] % 10 >= 2)
		O[i] += 8;
	if ((O[i] / 10) % 10 >= 2)
		O[i] = O[i] + (((O[i] / 10) % 10) / 2) * 100 - (((O[i] / 10) % 10) / 2) * 20;
	if ((O[i] / 100) % 10 >= 2)
		O[i] = O[i] + (((O[i] / 100) % 10) / 2) * 1000 - (((O[i] / 100) % 10) / 2) * 200;
	if ((O[i] / 1000) % 10 >= 2)
		O[i] = O[i] + (((O[i] / 1000) % 10) / 2) * 10000 - (((O[i] / 1000) % 10) / 2) * 2000;
	if ((O[i] / 10000) % 10 >= 2)
		O[i] = O[i] + (((O[i] / 10000) % 10) / 2) * 100000 - (((O[i] / 10000) % 10) / 2) * 20000;
	if ((O[i] / 100000) % 10 >= 2)
		O[i] = O[i] + (((O[i] / 100000) % 10) / 2) * 1000000 - (((O[i] / 100000) % 10) / 2) * 200000;
	if ((O[i] / 1000000) % 10 >= 2)
		O[i] = O[i] + (((O[i] / 1000000) % 10) / 2) * 10000000 - (((O[i] / 1000000) % 10) / 2) * 2000000;
	if(O[0]/10000)
	{
		for (i = l + 1; i >= 1; i--)
			O[i] = O[i - 1];
		O[0] = O[1] / 10000;
		O[1] = O[1] - O[0] * 10000;
		FLAG = 1;
	}
	return l + FLAG;
}

word mdiv(word *B1, word *B2, word *O)
{
	int i, j, k, m;
	int LL;
	int le, lee;
	int q;
	int num = -1;
	int mod;
	word a1;
	word a2;
	word *aa1;
	word *aa2;
	word oo[10000] = { 0 };
	if(len1<len2)						//被除数小于除数直接返回0
	{
		O[0] = 0;
		return 0;
	}
	else if(len1==len2)					//同上
	{
		if (B1[0]<B2[0])
		{
			O[0] = 0;
			return 0;
		}
	}
	if(len2==0)							//除数只有一位的情况，被除数每次只取一位与除数做减法，余数加到下一位
	{
		for(i=0;i<=len1;i++)
		{

			if (B1[i] % 10 >= 2)
				B1[i] += 8;
			if ((B1[i] / 10) % 10 >= 2)
				B1[i] += 80;
			if ((B1[i] / 100) % 10 >= 2)
				B1[i] += 800;
			if ((B1[i] / 1000)%10 >= 2)
				B1[i] += 8000;
			a1 = B1[i];
			a2 = B2[0];
			len2 = len1;
			for (j = 0;; j++)
			{
				if (a1 >= a2)
				{
					LL = msub(&a1, &a2, oo);
					
					a1 = oo[0];
					oo[0] = 0;
				}
				else
				{	
					if (i < len1)
					{
						B1[i + 1] += a1 * 1010;
					}
					break;
				}
			}
			
			if (j != 0 || (j == 0 && num + 1 != 0))
			{
				O[++num] = j;
				if (O[num] % 10 >= 2)
					O[num] = O[num] + ((O[num] % 10) / 2) * 10 - ((O[num] % 10) / 2) * 2;
				if ((O[num] / 10) % 10 >= 2)
					O[num] = O[num] + (((O[num] / 10) % 10) / 2) * 100 - (((O[num] / 10) % 10) / 2) * 20;
				if ((O[num] / 100) % 10 >= 2)
					O[num] = O[num] + (((O[num] / 100) % 10) / 2) * 1000 - (((O[num] / 100) % 10) / 2) * 200;	
			}
		}
	}
	else if (len2 > 0)							//除数大于一位时，被除数从最高位开始，取和除数相同位数与除数做减法至余数小于除数，把最高位加到下一位，再取一位继续做减法
	{
		aa1 = (word*)malloc(sizeof(unsigned int)*len2);
		aa2 = (word*)malloc(sizeof(unsigned int)*len2);
		le = len2;
		lee = len1;
		for (i = 0; i <= lee - le; i++)
		{
			if (B1[i] % 10 >= 2)
				B1[i] = B1[i] + ((B1[i] % 10) / 2) * 10 - ((B1[i] % 10) / 2) * 2;
			if ((B1[i] / 10) % 10 >= 2)
				B1[i] = B1[i] + (((B1[i] / 10) % 10) / 2) * 100 - (((B1[i] / 10) % 10) / 2) * 20;
			if ((B1[i] / 100) % 10 >= 2)
				B1[i] = B1[i] + (((B1[i] / 100) % 10) / 2) * 1000 - (((B1[i] / 100) % 10) / 2) * 200;
			if ((B1[i] / 1000) % 10 >= 2)
				B1[i] = B1[i] + (((B1[i] / 1000) % 10) / 2) * 10000 - (((B1[i] / 1000) % 10) / 2) * 2000;
			if ((B1[i] / 10000) % 10 >= 2)
				B1[i] = B1[i] + (((B1[i] / 10000) % 10) / 2) * 100000 - (((B1[i] / 10000) % 10) / 2) * 20000;
			for (j = 0; j <= le; j++)
				aa2[j] = B2[j];
			for (j = i; j <= i + le; j++)
				aa1[j - i] = B1[j];

			len1=len2;
			for (j = 0;; j++)
			{
				LL = msub(aa1, aa2, oo);
				if (FLAG == 1)
				{
					FLAG = 0;
					for (int u = 0; u <= LL; u++)
					{
						if(B1[i]!=0)
						{
							B1[i + 1] += B1[i] * 1010;
							B1[i] = 0;
						}
					}
					break;
				}
				for (k = 0; k < le - LL; k++)
				{
					aa1[k] = 0;
				}
				for (; k <= le; k++)
					aa1[k] = oo[k - le + LL];
				for (k = 0; k <= 100; k++)
					oo[k] = 0;
				for (q = i; q <= i + le; q++)
					B1[q] = aa1[q - i];
			}
			if (j != 0 || (j == 0 && num + 1 != 0))
			{
				O[++num] = j;
				if (O[num] % 10 >= 2)
					O[num] = O[num] + ((O[num] % 10) / 2) * 10 - ((O[num] % 10) / 2) * 2;
				if ((O[num] / 10) % 10 >= 2)
					O[num] = O[num] + (((O[num] / 10) % 10) / 2) * 100 - (((O[num] / 10) % 10) / 2) * 20;
				if ((O[num] / 100) % 10 >= 2)
					O[num] = O[num] + (((O[num] / 100) % 10) / 2) * 1000 - (((O[num] / 100) % 10) / 2) * 200;
			}
			for(k=0;k<le-LL-1;k++)
			{
				O[++num] = 0;
				i++;
			}
		}
	}
	return num;
}


word mmod(word *B1, word *B2, word *O)    // 余数直接套用除法，只把余数返回
{
	int i, j, k, m;
	int LL;
	int le, lee;
	int q;
	int num = -1;
	int mod;
	word a1;
	word a2;
	word *aa1;
	word *aa2;
	word oo[10000] = { 0 };
	if (len1<len2)
	{
		for (i = 0; i <= len1; i++)
			O[i] = B1[i];
		return len1;
	}
	else if (len1 == len2)
	{
		if (B1[0]<B2[0])
		{
			for (i = 0; i <= len1; i++)
				O[i] = B1[i];
			
			return len1;
		}
	}
	if (len2 == 0)
	{
		for (i = 0; i <= len1; i++)
		{

			if (B1[i] % 10 >= 2)
				B1[i] += 8;
			if ((B1[i] / 10) % 10 >= 2)
				B1[i] += 80;
			if ((B1[i] / 100) % 10 >= 2)
				B1[i] += 800;
			if ((B1[i] / 1000) % 10 >= 2)
				B1[i] += 8000;
			a1 = B1[i];
			a2 = B2[0];
			len2 = len1;
			for (j = 0;; j++)
			{
				if (a1 >= a2)
				{
					LL = msub(&a1, &a2, oo);

					a1 = oo[0];
					oo[0] = 0;
				}
				else
				{
					if (i < len1)
					{
						B1[i + 1] += a1 * 1010;
					}
					break;
				}
			}

			if (j != 0 || (j == 0 && num + 1 != 0))
			{
				O[++num] = j;
				if (O[num] % 10 >= 2)
					O[num] = O[num] + ((O[num] % 10) / 2) * 10 - ((O[num] % 10) / 2) * 2;
				if ((O[num] / 10) % 10 >= 2)
					O[num] = O[num] + (((O[num] / 10) % 10) / 2) * 100 - (((O[num] / 10) % 10) / 2) * 20;
				if ((O[num] / 100) % 10 >= 2)
					O[num] = O[num] + (((O[num] / 100) % 10) / 2) * 1000 - (((O[num] / 100) % 10) / 2) * 200;
			}
		}
		O[0] = a1;
		num = 0;
	}
	else if (len2 > 0)
	{
		aa1 = (word*)malloc(sizeof(unsigned int)*len2);
		aa2 = (word*)malloc(sizeof(unsigned int)*len2);
		le = len2;
		lee = len1;
		for (i = 0; i <= lee - le; i++)
		{
			if (B1[i] % 10 >= 2)
				B1[i] = B1[i] + ((B1[i] % 10) / 2) * 10 - ((B1[i] % 10) / 2) * 2;
			if ((B1[i] / 10) % 10 >= 2)
				B1[i] = B1[i] + (((B1[i] / 10) % 10) / 2) * 100 - (((B1[i] / 10) % 10) / 2) * 20;
			if ((B1[i] / 100) % 10 >= 2)
				B1[i] = B1[i] + (((B1[i] / 100) % 10) / 2) * 1000 - (((B1[i] / 100) % 10) / 2) * 200;
			if ((B1[i] / 1000) % 10 >= 2)
				B1[i] = B1[i] + (((B1[i] / 1000) % 10) / 2) * 10000 - (((B1[i] / 1000) % 10) / 2) * 2000;
			if ((B1[i] / 10000) % 10 >= 2)
				B1[i] = B1[i] + (((B1[i] / 10000) % 10) / 2) * 100000 - (((B1[i] / 10000) % 10) / 2) * 20000;
			for (j = 0; j <= le; j++)
				aa2[j] = B2[j];
			for (j = i; j <= i + le; j++)
				aa1[j - i] = B1[j];

			len1 = len2;
			for (j = 0;; j++)
			{
				LL = msub(aa1, aa2, oo);
				if (FLAG == 1)
				{
					FLAG = 0;
					if (i < lee - le)
					{
						if (B1[i] != 0)
						{
							B1[i + 1] += B1[i] * 1010;
							B1[i] = 0;
						}
					}
					break;
				}
				for (k = 0; k < le - LL; k++)
				{
					aa1[k] = 0;
				}
				for (; k <= le; k++)
					aa1[k] = oo[k - le + LL];
				for (k = 0; k <= 100; k++)
					oo[k] = 0;
				for (q = i; q <= i + le; q++)
					B1[q] = aa1[q - i];

			}
			if (j != 0 || (j == 0 && num + 1 != 0))
			{
				O[++num] = j;
				if (O[num] % 10 >= 2)
					O[num] = O[num] + ((O[num] % 10) / 2) * 10 - ((O[num] % 10) / 2) * 2;
				if ((O[num] / 10) % 10 >= 2)
					O[num] = O[num] + (((O[num] / 10) % 10) / 2) * 100 - (((O[num] / 10) % 10) / 2) * 20;
				if ((O[num] / 100) % 10 >= 2)
					O[num] = O[num] + (((O[num] / 100) % 10) / 2) * 1000 - (((O[num] / 100) % 10) / 2) * 200;
			}
			for (k = 0; k<le - LL - 1; k++)
			{
				O[++num] = 0;
				i++;
			}
		}
		for (j = i; j <= i + le; j++)
			O[j - i] = B1[j];
	}
	return num;
}

word mtoa(word *O, char *p, word L)
{
	int temp = 0;
	for(int i=0;i<=L;i++)
	{
		temp = 0;
		temp += ((O[i] / 1000) % 10) * 8;
		temp += ((O[i] / 100) % 10) * 4;
		temp += ((O[i] / 10) % 10) * 2;
		temp += O[i] % 10;
		O[i] = 0;
		p[i] = '0' + temp;
	}
	return L;
}