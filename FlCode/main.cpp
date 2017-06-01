#include "FlCode.h"
#include <sstream>
int main()
{
	JzStatus  st;
	InitJz( st);
	string zlx = "<!DOCTYPE html>\r\n<html lang = \"en\"";
	const char * ppp = zlx.c_str();
	std::vector<unsigned char>iSer;
	string zxj = EncodeJzByspace(st, zlx.c_str(), zlx.size());
	cout << "encode:" << zxj << endl;
	for (auto &x : iSer)
	{
		cout <<(unsigned int) x << " ";
	}
	cout << endl;
	vector<unsigned char> v = DecodeJzByspace(st, zxj, zxj.size());
	stringstream stream;
	for (auto &x : v)
	{
		stream << x;
	}
	cout << stream.str() << endl;
	return 1;
}