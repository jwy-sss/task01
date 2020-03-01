#include <iostream>
#include <string>
#include<sstream>
#include <fstream>//ifstream读文件，ofstream写文件，fstream读写文件
using namespace std;
void file_size_gener();
string stm10_vert_str2(streampos l);

int main(int argc, char* argv[])
{
	file_size_gener();
	ifstream i("file_size.bin", ios::binary);
	if (i)
	{
		string line;
		for (int j = 0; j < 10; j++)
		{
			getline(i, line);
			cout << line << endl;
		}
	}
	string t;
	cin >> t;
}

void file_size_gener()
{
	ofstream out("file_size", ios::binary);
	int count;
	for (count = 1;; count++)
	{
		stringstream st;
		st << count;
		string str = st.str();
		ifstream in("part_" + str);
		if (in) // 有该文件
		{
			in.seekg(0, ios::end); //设置文件指针到文件流的尾部
			streampos ps = in.tellg(); //读取文件指针的位置
			string s = stm10_vert_str2(ps);
			for (int j = s.length(); j < 70; j++)
			{
				s = s + "0";
			}
			out << s << endl;
			in.close(); //关闭文件流
		}
		else // 没有该文件
		{
			break;
		}
	}
	for (int k = count; k <= 70; k++)
	{
		for (int j = 0; j < 70; j++)
		{
			out << "0";
		}
		out << endl;
	}
	out.close();
}

string stm10_vert_str2(streampos l)
{
	string s;
	while (l / 2)
	{
		if (l % 2 == 0)		s = "0" + s;
		else	s = "1" + s;
		l = l / 2;
	}
	if (l % 2 == 0)		s = "0" + s;
	else	s = "1" + s;
	cout << s << endl;
	return s;
}