#include "Image.h"
#include<iostream>
#include<string>
#include<fstream>
#include<cstdlib>
using namespace std;
//Pixel::
Pixel::Pixel() : wid(0), hit(0), pix(NULL) {}
Pixel::Pixel(long long w, long long h) : wid(w), hit(h)
{
	pix = new unsigned char*[w];
	for (long long i = 0; i < w; i++)
		pix[i] = new unsigned char[h];
}
Pixel::Pixel(const Pixel &p) : wid(p.wid), hit(p.hit)
{
	pix = new unsigned char*[p.wid];
	for (long long i = 0; i < p.wid; i++)
	{
		pix[i] = new unsigned char[p.hit];
		for (long long j = 0; j < p.hit; j++)
		{
			pix[i][j] = p.pix[i][j];
		}
	}
}
Pixel::~Pixel()
{
	for (long long i = 0; i < wid; i++)
		delete[] pix[i];
	delete[] pix;
}
Pixel& Pixel::operator=(const Pixel &p)
{
	if (this != &p)
		for (long long i = 0; i < wid; i++)
			delete[] pix[i];
	delete[] pix;
	wid = p.wid;
	hit - p.hit;
	pix = new unsigned char*[p.wid];
	for (long long i = 0; i < p.wid; i++)
	{
		pix[i] = new unsigned char[p.hit];
		for (long long j = 0; j < p.hit; j++)
			pix[i][j] = p.pix[i][j];
	}
	return *this;
}
unsigned char& Pixel::operator()(long long w, long long h)
{
	return pix[w][h];
}
const unsigned char& Pixel::operator()(long long w, long long h) const
{
	return pix[w][h];
}

//Image::
void Image::filters(int n)
{
	Pixel nblue(width, height);
	Pixel ngreen(width, height);
	Pixel nred(width, height);
	for (long long i = 0; i < width; i++)
		for (long long j = 0; j < height; j++)
		{
			nblue(i, j) = blue(i, j);
			ngreen(i, j) = green(i, j);
			nred(i, j) = red(i, j);
			if (n == 8)
			{
				red(i, j) = 255 - nred(i, j);
				green(i, j) = 255 - ngreen(i, j);
				blue(i, j) = 255 - nblue(i, j);
			}
			if (n == 1 || n == 4 || n == 6 || n == 7)
				red(i, j) = (nred(i, j) + ngreen(i, j) + nblue(i, j)) / 3;
			if (n == 1 || n == 2 || n == 5 || n == 6)
				green(i, j) = (nred(i, j) + ngreen(i, j) + nblue(i, j)) / 3;
			if (n == 1 || n == 2 || n == 3 || n == 7)
				blue(i, j) = (nred(i, j) + ngreen(i, j) + nblue(i, j)) / 3;
			if (n == 13 || n == 9 || n == 12)
			{
				if((static_cast<int>(i / 256)) % 2 == 1)
					red(i, j) = i % width;
				else if ((static_cast<int>(i / 256)) % 2 == 0)
					red(i, j) = 255 - (i % width);
			}
			if (n == 13 || n == 14 || n == 10)
			{
				if ((static_cast<int>(i / 256)) % 2 == 1)
					green(i, j) = i % width;
				else if ((static_cast<int>(i / 256)) % 2 == 0)
					green(i, j) = 255 - (i % width);
			}
			if (n == 9 || n == 10 || n == 11)
			{
				if ((static_cast<int>(i / 256)) % 2 == 1)
					blue(i, j) = i % width;
				else if ((static_cast<int>(i / 256)) % 2 == 0)
					blue(i, j) = 255 - (i % width);
			}
			if (n == 15 || n == 16 || n == 17)
				red(i, j) = 0;
			if (n == 20 || n == 16 || n == 18)
				green(i, j) = 0;
			if (n == 19 || n == 17 || n == 18)
				blue(i, j) = 0;
		}
}
void Image::dim(int n)
{
	if(n < 1 && n > 10)
		throw 1;
	for (long long i = 0; i < width; i++)
		for (long long j = 0; j < height; j++)
		{
			red(i, j) = red(i, j) / n;
			green(i, j) = green(i, j) / n;
			blue(i, j) = blue(i, j) / n;
		}
}
//PPM::
void PPM::ignoreWSCom(ifstream &img)
{
	char c;
	while (true)
	{
		img >> ws;
		c = img.peek();
		if (c == '#')
		{
			img.ignore(900000, '\n');
		}
		else if (c >= '0' && c <= '9')
		{
			return;
		}
		else
		{
			img.ignore(1);
		}
	}
}
void PPM::rotation(int n)
{
	if (n == 2 || n == 4 || n == 6)
	{
		Pixel nblue(width, height);
		Pixel ngreen(width, height);
		Pixel nred(width, height);
		for (long long i = 0; i < width; i++)
			for (long long j = 0; j < height; j++)
			{
				nblue(i, j) = blue(i, j);
				ngreen(i, j) = green(i, j);
				nred(i, j) = red(i, j);
			}
		if (n == 2) //180
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue(width - i - 1, height - j - 1);
					green(i, j) = ngreen(width - i - 1, height - j - 1);
					red(i, j) = nred(width - i - 1, height - j - 1);
				}
		else if (n == 4)	//Ref0
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue(i, height - j - 1);
					green(i, j) = ngreen(i, height - j - 1);
					red(i, j) = nred(i, height - j - 1);
				}
		else if (n == 6) //Ref180
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue(width - i - 1, j);
					green(i, j) = ngreen(width - i - 1, j);
					red(i, j) = nred(width - i - 1, j);
				}
	}
	else if (n == 1 || n == 3 || n == 5 || n == 7)
	{
		unsigned char *nblue1 = new unsigned char[width * height];
		unsigned char *ngreen1 = new unsigned char[width * height];
		unsigned char *nred1 = new unsigned char[width * height];
		long long k = 0;
		for (long long i = 0; i < width; i++)
			for (long long j = 0; j < height; j++)
			{
				nblue1[k] = blue(i, j);
				ngreen1[k] = green(i, j);
				nred1[k] = red(i, j);
				k++;
			}
		long long r = width;
		width = height;
		height = r;
		Pixel templ(width, height);
		blue = templ;
		green = templ;
		red = templ;
		if (n == 1)	//90
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue1[height * (j + 1) - i];
					green(i, j) = ngreen1[height * (j + 1) - i];
					red(i, j) = nred1[height * (j + 1) - i];
				}
		else if (n == 3) //270
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue1[height * (height - j) - (width - i - 1)];
					green(i, j) = ngreen1[height * (height - j) - (width - i - 1)];
					red(i, j) = nred1[height * (height - j) - (width - i - 1)];
				}
		else if (n == 5)	//Ref90
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue1[height * (j + 1) - (width - i - 1)];
					green(i, j) = ngreen1[height * (j + 1) - (width - i - 1)];
					red(i, j) = nred1[height * (j + 1) - (width - i - 1)];
				}
		else if (n == 7)	//Ref270
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue1[height * (height - j) - i];
					green(i, j) = ngreen1[height * (height - j) - i];
					red(i, j) = nred1[height * (height - j) - i];
				}
	}
}

//P3::
P3::P3(string s)
{
	type = Type::P3PPM;
	filename = s;
	ifstream img(filename.c_str());
	if (img.is_open())
	{
		img >> ppmType[0];
		img >> ppmType[1];
		ignoreWSCom(img);
		img >> width;
		ignoreWSCom(img);
		img >> height;
		ignoreWSCom(img);
		img >> shades;
		ignoreWSCom(img);
		Pixel templ(width, height);
		blue = templ;
		green = templ;
		red = templ;
		int temp;
		for (long long i = 0; i < width; i++)
			for (long long j = 0; j < height; j++)
			{
				img >> temp;
				red(i, j) = temp;
				img >> temp;
				green(i, j) = temp;
				img >> temp;
				blue(i, j) = temp;
			}
		img.close();
	}
	else
		throw 1;
}
void P3::write()
{
	ofstream img(("new" + filename).c_str());
	if (img.is_open())
	{
		img << ppmType[0];
		img << ppmType[1] << endl;
		img << width << endl;
		img << height << endl;
		img << shades << endl;
		for (long long i = 0; i < width; i++)
			for (long long j = 0; j < width; j++)
			{
				if (i != 0 && j % 15 == 0)
					img << endl;
				img << int(red(i, j)) << " " << int(green(i, j)) << " " << int(blue(i, j)) << " ";
			}
		img.close();
	}
	else
		throw 1;
	system(("new" + filename).c_str());
}

//P6::
P6::P6(string s)
{
	type = Type::P6PPM;
	filename = s;
	ifstream img(filename.c_str(), ios::binary);
	if (img.is_open())
	{
		img >> ppmType[0];
		img >> ppmType[1];
		ignoreWSCom(img);
		img >> width;
		ignoreWSCom(img);
		img >> height;
		ignoreWSCom(img);
		img >> shades;
		ignoreWSCom(img);
		img.close();
		Pixel templ(width, height);
		blue = templ;
		green = templ;
		red = templ;
		int temp;
		unsigned char *bt = new unsigned char;
		basic_ifstream<unsigned char> img1(filename.c_str(), ios::binary);
		img1.seekg(15, ios::beg);
		for (long long i = 0; i < width; i++)
			for (long long j = 0; j < height; j++)
			{
				img1.read(reinterpret_cast<unsigned char*>(bt), sizeof(*bt));
				temp = *bt;
				blue(i, j) = temp;
				img1.read(reinterpret_cast<unsigned char*>(bt), sizeof(*bt));
				temp = *bt;
				green(i, j) = temp;
				img1.read(reinterpret_cast<unsigned char*>(bt), sizeof(*bt));
				temp = *bt;
				red(i, j) = temp;
			}
		img1.close();
	}
	else
		throw 1;
}

void P6::write()
{
	ofstream img(("new" + filename).c_str(), ios::binary);
	if (img.is_open())
	{
		img << ppmType[0];
		img << ppmType[1] << endl;
		img << width << " " << height << endl;
		img << shades << endl;
		for (long long i = 0; i < width; i++)
			for (long long j = 0; j < height; j++)
			{
				img.write(reinterpret_cast<char*>(&blue(i, j)), sizeof(blue(i, j)));
				img.write(reinterpret_cast<char*>(&green(i, j)), sizeof(red(i, j)));
				img.write(reinterpret_cast<char*>(&red(i, j)), sizeof(red(i, j)));
			}
		img.close();
	}
	else
		throw 1;
	system(("new" + filename).c_str());
}

//BMP::
BMP::BMP(string s)
{
	type = Type::Bitmap;
	filename = s;
	ifstream img(filename.c_str(), ios::binary);
	if (img.is_open())
	{
		img.seekg(18, ios::beg);
		int* in = new int;
		img.read(reinterpret_cast<char*>(in), sizeof(*in));
		width = *in;
		img.read(reinterpret_cast<char*>(in), sizeof(*in));
		height = *in;
		img.close();
		height++;
		Pixel templ(width, height);
		blue = templ;
		green = templ;
		red = templ;
		unsigned char *bt = new unsigned char;
		basic_ifstream<unsigned char> img1(filename.c_str(), ios::binary);
		header = new unsigned char[54];
		int temp;
		for (long long i = 0; i < 54; i++)
		{
			img1.read(reinterpret_cast<unsigned char*>(bt), 1);
			temp = *bt;
			header[i] = temp;
		}
		img1.seekg(54, ios::beg);
		for (long long i = 0; i < width; i++)
			for (long long j = 0; j < height; j++)
			{
				img1.read(reinterpret_cast<unsigned char*>(bt), sizeof(*bt));
				temp = *bt;
				blue(i, j) = temp;
				img1.read(reinterpret_cast<unsigned char*>(bt), sizeof(*bt));
				temp = *bt;
				green(i, j) = temp;
				img1.read(reinterpret_cast<unsigned char*>(bt), sizeof(*bt));
				temp = *bt;
				red(i, j) = temp;
			}
		img1.close();
	}
	else
		throw 1;
}
BMP::~BMP()
{
	delete[] header;
}
void BMP::rotation(int n)
{
	if (n == 2 || n == 4 || n == 6)
	{
		Pixel nblue(width, height);
		Pixel ngreen(width, height);
		Pixel nred(width, height);
		for (long long i = 0; i < width; i++)
			for (long long j = 0; j < height; j++)
			{
				nblue(i, j) = blue(i, j);
				ngreen(i, j) = green(i, j);
				nred(i, j) = red(i, j);
			}
		if (n == 2) //180
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue(width - i - 1, height - j - 1);
					green(i, j) = ngreen(width - i - 1, height - j - 1);
					red(i, j) = nred(width - i - 1, height - j - 1);
				}
		else if (n == 4)	//Ref0
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue(i, height - j - 1);
					green(i, j) = ngreen(i, height - j - 1);
					red(i, j) = nred(i, height - j - 1);
				}
		else if (n == 6) //Ref180
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue(width - i - 1, j);
					green(i, j) = ngreen(width - i - 1, j);
					red(i, j) = nred(width - i - 1, j);
				}
	}
	else if (n == 1 || n == 3 || n == 5 || n == 7)
	{
		unsigned char *nblue1 = new unsigned char[width * height];
		unsigned char *ngreen1 = new unsigned char[width * height];
		unsigned char *nred1 = new unsigned char[width * height];
		long long k = 0;
		for (long long i = 0; i < width; i++)
			for (long long j = 0; j < height; j++)
			{
				nblue1[k] = blue(i, j);
				ngreen1[k] = green(i, j);
				nred1[k] = red(i, j);
				k++;
			}
		long long r = width;
		width = height;
		height = r;
		width--;
		height++;
		Pixel templ(width, height);
		blue = templ;
		green = templ;
		red = templ;
		if (n == 1) //90
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue1[height * (height - j) - (width - i - 1)];
					green(i, j) = ngreen1[height * (height - j) - (width - i - 1)];
					red(i, j) = nred1[height * (height - j) - (width - i - 1)];
				}
		else if (n == 3)	//270
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue1[height * (j + 1) - i];
					green(i, j) = ngreen1[height * (j + 1) - i];
					red(i, j) = nred1[height * (j + 1) - i];
				}
		else if (n == 5)	//REF90
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue1[height * (height - j) - i];
					green(i, j) = ngreen1[height * (height - j) - i];
					red(i, j) = nred1[height * (height - j) - i];
				}
		else if (n == 7)	//REF270
			for (long long i = 0; i < width; i++)
				for (long long j = 0; j < height; j++)
				{
					blue(i, j) = nblue1[height * (j + 1) - (width - i - 1)];
					green(i, j) = ngreen1[height * (j + 1) - (width - i - 1)];
					red(i, j) = nred1[height * (j + 1) - (width - i - 1)];
				}
	}
}
void BMP::write()
{
	ofstream img2(("new" + filename).c_str(), ios::binary);
	header[18] = width;
	header[22] = height - 1;
	for (long long i = 0; i < 54; i++)
		img2.write(reinterpret_cast<char*>(&header[i]), sizeof(header[i]));
	for (long long i = 0; i < width; i++)
		for (long long j = 0; j < height; j++)
		{
			img2.write(reinterpret_cast<char*>(&blue(i, j)), sizeof(blue(i, j)));
			img2.write(reinterpret_cast<char*>(&green(i, j)), sizeof(red(i, j)));
			img2.write(reinterpret_cast<char*>(&red(i, j)), sizeof(red(i, j)));
		}
	img2.close();
	system(("new" + filename).c_str());
}