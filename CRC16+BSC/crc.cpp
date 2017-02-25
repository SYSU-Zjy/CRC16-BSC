#include"crc.h"
#include<iostream>
#include<fstream>
#include<time.h>
#include<stdlib.h>

using namespace std;

//���ı�����
//CRC16::CRC16() {
//	ifstream fin ("test.txt");
//	if (!fin.is_open()) {
//		cout << "error";
//		exit(-1);
//	}
//	fin.getline((char*)num, blockLength);
//	cout << num << endl;
//}

CRC16::CRC16(int a) {
	int i;
	for (i = 0; i<blockLength-1; ++i)
	{
		switch ((a % 3))
		{
		case 1:
			num[i] = 'A' + a % 26;
			break;
		case 2:
			num[i] = 'a' + a % 26;
			break;
		default:
			num[i] = '0' + a % 10;
			break;
		}
	}
	num[i] = '\0';
	is_change = false;//��ʼ��Ϊfalse
	is_error = false;
	state = a;
}

void CRC16::get_crc() {
	unsigned short tmp = (unsigned short)(0xFFFF);//��ʼ��16���Ĵ���Ϊ1
	for (int i = 0; i<blockLength; i++)
	{
		tmp ^= (unsigned short)(num[i]);
		for (int j = 0; j<8; j++)
		{
			if (tmp & 1)
			{
				tmp >>= 1;
				tmp ^= 0xA001;//0x8005�ķ�������ʽ
			}
			else
			{
				tmp >>= 1;
			}
		}
	}
	crc[1] = tmp&0x00FF;//CRC��λ
	crc[0] = (tmp >> 8) & 0xFF;//CRC��λ
}

void CRC16::add_crc() {//��CRC����ں����ֽ�
	num[blockLength] = crc[1];//��λ��ǰ
	num[blockLength + 1] = crc[0];//��λ�ں����ﱻ���˺ܾ�
}

/**************************************************************************
������Uniform()
���ܣ�����0-1����ȷֲ��������
***************************************************************************/

double CRC16::Uniform()
{
	double u;
	int tmpState = A * (state % Q) - R * (state / Q);
	if (tmpState >= 0)
		state = tmpState;
	else
		state = tmpState + M;
	u = state / (double)M;
	return u;
}

void CRC16::after_bsc_channel(double prob) {//����BSC�ŵ�
	for (int i = 0; i < blockLength + 2; i++) {
		unsigned char byte = 0;
		for (int j = 0; j < 8; j++) {
			unsigned char tmp = num[i] & (0x80 >> j);
			unsigned char check = Uniform() < prob;//��Ҫһ�� 
			unsigned char check1 = (check)<<(7-j);
			byte = (tmp^check1) | byte;
			if (check == 1)//0��1��1��0�����
				is_change = true;
		}
		crc_dec[i] = byte;
	}
}


void CRC16::check_error() {
	unsigned short tmp = (unsigned short)(0xFFFF);//��ʼ��16���Ĵ���Ϊ1
	for (int i = 0; i<blockLength+2; i++)
	{
		tmp ^= (unsigned short)(crc_dec[i]);
		for (int j = 0; j<8; j++)
		{
			if (tmp & 1)
			{
				tmp >>= 1;
				tmp ^= 0xA001;//0x8005�ķ�������ʽ
			}
			else
			{
				tmp >>= 1;
			}
		}
	}
	if (tmp == 0) //��ʽΪ0��У���޲��
		is_error = false;
	else
		is_error = true;
}

bool CRC16::is_errors() {
	return is_error;
}

bool CRC16::is_changes() {//bit�Ƿ����
	return is_change;
}

void CRC16::free() {
	
}
