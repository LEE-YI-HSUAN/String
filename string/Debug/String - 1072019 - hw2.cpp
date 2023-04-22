#include "String.h" // include definition of class String
#include<iostream>
using std::cout;
using std::endl;

string::string()
	: bx(),
	mySize(0),
	myRes(15)
{
}

string::string(const char *const ptr, const size_type count)
	: bx(),
	mySize(0),
	myRes(15)
{
	mySize = count;
	size_type newMyRes = myRes * 3 / 2;

	if (mySize >= 16 ) {
		myRes = (mySize / 16) * 16 + 15;
		//需要配置新空間
		char * temp = new char[myRes];
		bx.ptr = temp;
		
		for (int i = 0; i < count; i++)
			*(temp + i) = ptr[i];
	}
	else {
		myRes = 15;
		for (int i = 0; i < count; i++)
			bx.buf[i] = ptr[i];
	}
}

string::string(const size_type count, const char ch)
	: bx(),
	mySize(0),
	myRes(15)
{
	mySize = count;
	size_type newMyRes = myRes * 3 / 2;

	if (mySize >= 16) {
		myRes = (mySize / 16) * 16 + 15;
		//需要配置新空間
		char * temp = new char[myRes];
		bx.ptr = temp;

		for (int i = 0; i < count; i++)
			*(temp + i) = ch;
	}
	else {
		myRes = 15;

		for (int i = 0; i < count; i++)
			bx.buf[i] = ch;
	}
}

string::string(const string &right)
	: bx(),
	mySize(0),
	myRes(15)
{
	//mySize = right.mySize;
	size_type newMyRes = myRes * 3 / 2;

	if(right.myRes >= 16){
		//需要新空間
		myRes = (right.mySize / 16) * 16 + 15;
		char * temp = new char[myRes];

		bx.ptr = temp;
		mySize = right.mySize;
		for (int i = 0; i < right.mySize; i++) {
			*(temp + i) = right.bx.ptr[i];
		}
	}
	else {
		myRes = 15;
		mySize = right.mySize;

		for (int i = 0; i < mySize; i++)
			bx.buf[i] = right.bx.buf[i];
	}
}

string::~string()
{
	if (myRes > 15)
		delete[] bx.ptr;
}

string::value_type* string::myPtr()
{
	if (myRes < 16)
		return bx.buf;
	else
		return bx.ptr;
}

const string::value_type* string::myPtr() const
{
	if (myRes < 16)
		return bx.buf;
	else
		return bx.ptr;
}

string& string::assign(const string &right)
{
	if (this != &right)
	{
		//如果新需要的大小(right.mySize) > 原有空間(myRes)的話新創一個空間
		if (right.mySize > myRes) {
			//配置(right.mySize / 16) * 16 + 15跟myRes * 3 / 2較大的那一個
			if ((right.mySize / 16) * 16 + 15 < myRes * 3 / 2) {
				myRes = myRes * 3 / 2;
			}
			else {
				myRes = (right.mySize / 16) * 16 + 15;
			}
			char * temp = new char[myRes];

			for (int i = 0; i < right.mySize; i++)
				*(temp + i) = right.bx.ptr[i];

			mySize = right.mySize;
			bx.ptr = temp;
		}
		else {
			//如果right.myRes==15的話要從right.bx.buf中拿值
			if (right.myRes == 15) {
				//如果原有空間也等於15話，直接覆蓋
				if (myRes == 15) {
					bx = right.bx;
				}
				else {
					//只覆蓋前15個值
					for (int i = 0; i < 16; i++)
						bx.ptr[i] = right.bx.buf[i];
				}
			}
			else {
				for (int i = 0; i < right.mySize; i++)
					bx.ptr[i] = right.bx.ptr[i];
			}

			mySize = right.mySize;
		}
	}
	return *this;
}

void string::clear()
{
	mySize = 0;
	myPtr()[0] = value_type();
}

string::iterator string::begin()
{
	return iterator(myPtr());
}

string::const_iterator string::begin() const
{
	return const_iterator(myPtr());
}

string::iterator string::end()
{
	return iterator(myPtr() + static_cast<difference_type>(mySize));
}

string::const_iterator string::end() const
{
	return const_iterator(myPtr() + static_cast<difference_type>(mySize));
}

string::reference string::element(const size_type off)
{
	return myPtr()[off];
}

string::const_reference string::element(const size_type off) const
{
	return myPtr()[off];
}

void string::push_back(char ch)
{
	resize(mySize + 1);
	myPtr()[mySize - 1] = ch;
}

void string::pop_back()
{
	if (mySize > 0)
		--mySize;
}

string::reference string::front()
{
	return myPtr()[0];
}

string::const_reference string::front() const
{
	return myPtr()[0];
}

string::reference string::back()
{
	return myPtr()[mySize - 1];
}

string::const_reference string::back() const
{
	return myPtr()[mySize - 1];
}

const char* string::c_str() const
{
	return myPtr();
}

string::size_type string::size() const
{
	return mySize;
}

void string::resize(const size_type newSize, const char ch)
{
	size_type newMyRes = myRes * 3 / 2;
	//如果新的大小<16且原空間<16的話，要在bx.buf中互相傳值
	if (newSize < 16 && myRes <= 15) {
		myRes = 15;
		
		for (int i = 0; i < mySize; i++)
			bx.buf[i] = bx.buf[i];

		//ch有值才會進來
		if (ch != '\0') {
			for (int i = mySize; i < myRes; i++)
				bx.buf[i] = ch;
		}

		mySize = newSize;
	}
	else {
		//如果新需要的大小(newSize) > 原有空間(myRes)的話，新創一個空間
		if (newSize > myRes) {
			//當新需要的空間 == 16時，需要新空間
			if (newSize == 16) {
				myRes = (newSize / 16) * 16 + 15;
				char * temp = new char[myRes];

				for (int i = 0; i < 16; i++)
					*(temp + i) = bx.buf[i];

				bx.ptr = temp;
				mySize = newSize;
			}
			else {
				myRes = newMyRes;
				if (newMyRes < (newSize / 16) * 16 + 15) {
					myRes = (newSize / 16) * 16 + 15;
				}

				char* temp = new char[myRes];
				for (int i = 0; i < mySize; i++)
					*(temp + i) = bx.ptr[i];

				if (ch != '\0') {
					for (int i = mySize; i < myRes; i++)
						*(temp + i) = ch;
				}

				mySize = newSize;
				bx.ptr = temp;
			}
		}
		else {
			for (int i = mySize; i < newSize; i++)
				*(bx.ptr + i) = ch;

			mySize = newSize;
		}
	}

}

string::size_type string::capacity() const
{
	return myRes;
}

bool string::empty() const
{
	return mySize == 0;
}