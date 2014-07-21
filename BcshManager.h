#ifndef _BCSHMANAGER_H__
#define _BCSHMANAGER_H__
#define BUFFER_LENGTH	256

class BcshManger
{
public:
	BcshManger();
	~BcshManger();
	int setBrightness(int display, int brightness);//0~255
	int setContrast(int display,int contrast);//0~511
	int setSat_con(int display, int sta);//0~511
	int setHue(int display, int s1, int s2);
private:
	void init();
};
#endif
