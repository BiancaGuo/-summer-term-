//�ӵ�(��ʱ��)�͵л�
//��������ӵ�
#include"Bullet_Flight.h"



LPBULLET CreateBullet(unsigned int x,unsigned int y)
{
	LPBULLET lpbullet = (LPBULLET)malloc(sizeof(BULLET));
	if (lpbullet == 0)
	{
		return (LPBULLET)0; 
	}
	lpbullet->x = x;
	lpbullet->y = y;
	lpbullet->dir = 0;
	lpbullet->power = 0;
	lpbullet->type = 0;
	lpbullet->y_step = 5;

	return lpbullet;
}

void DestroyBullet(LPBULLET lpbullet)
{
	if (lpbullet!= 0)
	free(lpbullet);
}


int MoveBullet(LPBULLET lpbullet)
{
	lpbullet->y -= lpbullet->y_step;
	lpbullet->x -= lpbullet->y_step*lpbullet->dir;
	if (lpbullet->y<0|| lpbullet->x<0)
	{
		return  BULLET_OUT;//�ɳ��߽磬out

	}
	return  BULLET_MOVE;

}



unsigned int GetBulletX(LPBULLET lpbullet)
{
	return lpbullet->x;
}

unsigned int GetBulletY(LPBULLET lpbullet)
{
	return lpbullet->y;
}