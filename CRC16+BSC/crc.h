#ifndef C_R_C_H
#define C_R_C_H

const int blockLength = 1024 / 8;//��Ϣλ
//����һЩ����
const int A = 48271;
const long M = 2147483647;
const int Q = M / A;
const int R = M % A;

class CRC16 {
private:
	long int state;//Ĭ������
	unsigned char num[blockLength + 2];
	unsigned char crc[2]; //�洢16λУ��λ
	unsigned char crc_dec[blockLength + 2];//�����ŵ������
	bool is_change;
	bool is_error;
public:
	CRC16(int a);//��ȡ��Ϣλ����������ַ���
	void get_crc();
	void add_crc();
	double Uniform();
	void after_bsc_channel(double prob);
	void check_error();
	void free();
	bool is_errors();//��Ϣ�Ƿ����
	bool is_changes();//bit�Ƿ����
	unsigned char* val2() {
	return crc_dec;
	}
	unsigned char* val1() {
		return num;
	}
};


#endif // !C_R_C_H

