#include<iostream>
#include<string>
#include<fstream>
using namespace std;
enum Type
{
	P3PPM, P6PPM, Bitmap
};
class Pixel
{
private:
	long long wid;
	long long hit;
	unsigned char** pix;
public:
	Pixel();
	Pixel(long long, long long);
	Pixel(const Pixel&);
	~Pixel();
	Pixel& operator=(const Pixel&);
	unsigned char& operator()(long long, long long);
	const unsigned char& operator()(long long, long long) const;
};
class Image
{
protected:
	string filename;
	Type type;
	long long width;
	long long height;
	Pixel red;
	Pixel green;
	Pixel blue;
public:
	virtual void rotation(int) = 0;
	void filters(int);
	void dim(int);
	virtual void write() = 0;
};
class PPM : public Image
{
protected:
	char ppmType[2];
	int shades;
	void ignoreWSCom(ifstream &img);
public:
	void rotation(int);
};
class P3 : public PPM
{
private:
public:
	P3(string);
	void write();
};
class P6 : public PPM
{
private:
public:
	P6(string);
	void write();
};
class BMP : public Image
{
private:
	char sign[2];
	unsigned char* header;
public:
	BMP(string);
	~BMP();
	void rotation(int);
	void write();
};