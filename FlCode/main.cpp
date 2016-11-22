#include "FlCode.h"
#include <sstream>
int main()
{
	JzStatus  st;
	InitJz( st);
	string zlx = "分离码是一种信息编解码技术，主要利用数学的不同进制转换来形成，结合码表和数学的进制转换，提出码位分离的编解码方法。 ";
	const char * ppp = zlx.c_str();
	std::vector<unsigned char>iSer;
	string zxj = EncodeJz(st, zlx.c_str(), zlx.size(), iSer);
	cout << "encode:" << zxj << endl;
	for (auto &x : iSer)
	{
		cout <<(unsigned int) x << " ";
	}
	cout << endl;
	vector<unsigned char> v = DecodeJz(st, zxj, zxj.size(), iSer);
	stringstream stream;
	for (auto &x : v)
	{
		stream << x;
	}
	cout << stream.str() << endl;
	return 1;
}