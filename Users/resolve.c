#include <math.h>

// �����е�۵ĳ���
#define L1 10.0  // ��һ�γ���
#define L2 10.0  // �ڶ��γ���
#define L3 10.0  // �����γ���

// ����PI����
#define PI 3.14159265358979323846

// ���˶�ѧ���㺯��
void inverse_kinematics(double x, double y, double z, double *theta1, double *theta2, double *theta3) 
{
	double alpha,beta,gamma,beta1,beta2;
    // ����
	alpha = atan2(y,x);//��������ǣ������ƣ�
    *theta1 = atan2(y, x);

    // ����ĩ����x-yƽ���ϵ�ͶӰ����
    double Lp = sqrt(x * x + y * y);
	// ����ĩ����z���ϵĸ߶�
    double Lh = z - L1;
	// ������б�߾���
    double Ld = sqrt(Lp * Lp + Lh * Lh);
	if(Lh < 0)
	{
		beta1 = acos((L2 * L2 + Ld * Ld - L3 * L3)/(2 * L2 * Ld));
		beta2 = atan2(Lp,fabs(Lh));
		beta = PI - (beta1 + beta2);
	}
	else if(Lh == 0)
	{
		beta1 = acos((L2 * L2 + Ld * Ld - L3 * L3)/(2 * L2 * Ld));
		beta = PI / 2 - beta1;
	}
	else if(Lh > 0)
	{
		beta1 = acos((L2 * L2 + Ld * Ld - L3 * L3)/(2 * L2 * Ld));
		beta2 = atan2(fabs(Lh),Lp);
		beta = PI/2 - (beta1 + beta2);
	}
    // ʹ�����Ҷ������theta3
    double cos_gamma = (L2 * L2 + L3 * L3 - Ld * Ld) / (2 * L2 * L3);
    gamma = PI - acos(cos_gamma);

    // ���Ƕ�ת��Ϊ��
    *theta1 = alpha * 180 / PI;
    *theta2 = beta * 180 / PI;
    *theta3 = gamma * 180 / PI;
}
//���˶�ѧ���㺯��
void forward_kinematics(double* x, double* y, double* z, double theta1, double theta2, double theta3) 
{
	double alpha = theta3 - (PI/2 - theta2);
	double Lp = L2 * sin(theta2) + L3 * cos(alpha);
	*x = Lp * cos(theta1);
	*y = Lp * sin(theta1);
	*z = L1 + L2 * cos(theta2) - L3 * sin(alpha);
}

















