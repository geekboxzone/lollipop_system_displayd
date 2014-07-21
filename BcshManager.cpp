#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <signal.h>
#include <sys/wait.h>
#include <cutils/properties.h>
#include <sys/stat.h>
#include <math.h>

#define LOG_TAG "BcshManager"
#include <cutils/log.h>


#include <sysutils/SocketClient.h>
#include "BcshManager.h"

#define BCSH_SYSFS_NODE	"/sys/class/graphics/fb0/bcsh"
#define BCSH_TYPE_BRIGHT "brightness "
#define BCSH_TYPE_CONTRAST "contrast"
#define BCSH_TYPE_STACON "sat_con"
#define BCSH_TYPE_HUE "hue"

BcshManger::BcshManger()
{
	FILE *fd = NULL;
	char buf[BUFFER_LENGTH];
	memset(buf, 0, BUFFER_LENGTH);
	strcpy(buf, BCSH_SYSFS_NODE);
	char tmp[16] = "open";
	fd = fopen(buf, "wb");
	if(fd != NULL)
	{
		fwrite(tmp,strlen(tmp), 1,fd);
		fclose(fd);
	}
	init();

}
void BcshManger::init()
{
	char property[PROPERTY_VALUE_MAX];
	char tmp[128];
	memset(property,0,PROPERTY_VALUE_MAX);
	memset(tmp,0,128);
	FILE *fd = NULL;
	char buf[BUFFER_LENGTH];
	memset(buf, 0, BUFFER_LENGTH);
	strcpy(buf, BCSH_SYSFS_NODE);
	
    if(property_get("persist.sys.bcsh.brightness", property, 0) > 0){
		sprintf(tmp,"brightness %d",atoi(property));
		
		fd = fopen(buf, "wb");
		if(fd != NULL)
		{
			fwrite(tmp,strlen(tmp), 1,fd);
			fclose(fd);
		}
    }
	memset(tmp,0,128);
	memset(property,0,PROPERTY_VALUE_MAX);
	if(property_get("persist.sys.bcsh.contrast", property, 0) > 0){
		sprintf(tmp,"contrast %d",atoi(property));
		fd = fopen(buf, "wb");
		if(fd != NULL)
		{
			fwrite(tmp,strlen(tmp), 1,fd);
			fclose(fd);
		}
	}

	memset(tmp,0,128);
	memset(property,0,PROPERTY_VALUE_MAX);
	if(property_get("persist.sys.bcsh.satcon", property, 0)>0){
		sprintf(tmp,"satcon %d",atoi(property));
		fd = fopen(buf, "wb");
		if(fd != NULL)
		{
			fwrite(tmp,strlen(tmp), 1,fd);
			fclose(fd);
		}
	}
	memset(tmp,0,128);
	memset(property,0,PROPERTY_VALUE_MAX);
	
	if(property_get("persist.sys.bcsh.hue", property, 0) > 0){
		int sin_hue =0;
		int cos_hue = 0;
		if(atoi(property) >=0 && atoi(property) <= 30){
			sin_hue = (int)(sin((float)atoi(property)/10-30)*256) + 0x100;
			cos_hue = (int)(cos((float)atoi(property)/10-30)*256);
		}else{
			sin_hue = (int)(sin((float)atoi(property)/10-30)*256);
			cos_hue = (int)(cos((float)atoi(property)/10-30)*256);
		}
		sprintf(tmp,"hue %d %d",sin_hue,cos_hue);
		fd = fopen(buf, "wb");
		if(fd != NULL)
		{
			fwrite(tmp,strlen(tmp), 1,fd);
			fclose(fd);
		}
	}
}
int BcshManger::setBrightness(int display,int brightness)
{
	FILE *fd = NULL;
	char buf[BUFFER_LENGTH];
	char tmp[128];
	char prop[16];
	memset(tmp,0,128);
	
	memset(buf, 0, BUFFER_LENGTH);
	strcpy(buf, BCSH_SYSFS_NODE);

	//strcpy(tmp,BCSH_TYPE_BRIGHT);
	sprintf(tmp,"brightness %d",brightness);
	fd = fopen(buf, "wb");
	if(fd == NULL)
		return -1;

	fwrite(tmp,strlen(tmp), 1,fd);
	fclose(fd);
	sprintf(prop,"%d",brightness);
	property_set("persist.sys.bcsh.brightness",prop);
	return 0;
}

int BcshManger::setContrast(int display,int contrast)
{
	FILE *fd = NULL;
	char buf[BUFFER_LENGTH];
	char tmp[128];
	char prop[16];
	memset(prop,0,16);
	memset(tmp,0,128);
	
	memset(buf, 0, BUFFER_LENGTH);
	strcpy(buf, BCSH_SYSFS_NODE);

	//strcpy(tmp,BCSH_TYPE_BRIGHT);
	sprintf(tmp,"contrast %d",contrast);
	fd = fopen(buf, "wb");
	if(fd == NULL)
		return -1;
	fwrite(tmp,strlen(tmp),1, fd);
	fclose(fd);
	sprintf(prop,"%d",contrast);
	property_set("persist.sys.bcsh.contrast",prop);
	return 0;
}

int BcshManger::setSat_con(int display,int sta)
{
	FILE *fd = NULL;
	char buf[BUFFER_LENGTH];
	char tmp[128];
	char prop[16];
	memset(prop,0,16);
	memset(tmp,0,128);
	
	memset(buf, 0, BUFFER_LENGTH);
	strcpy(buf, BCSH_SYSFS_NODE);

	//strcpy(tmp,BCSH_TYPE_BRIGHT);
	sprintf(tmp,"sat_con %d",sta);
	fd = fopen(buf, "wb");
	if(fd == NULL)
		return -1;
	fwrite(tmp,strlen(tmp),1, fd);
	fclose(fd);
	sprintf(prop,"%d",sta);
	property_set("persist.sys.bcsh.satcon",prop);
	return 0;
}

int BcshManger::setHue(int display,int s1,int s2)
{
	FILE *fd = NULL;
	char buf[BUFFER_LENGTH];
	char tmp[128];
	memset(tmp,0,128);
	
	memset(buf, 0, BUFFER_LENGTH);
	strcpy(buf, BCSH_SYSFS_NODE);

	//strcpy(tmp,BCSH_TYPE_BRIGHT);
	sprintf(tmp,"hue %d %d",s1,s2);
	fd = fopen(buf, "wb");
	if(fd == NULL)
		return -1;
	fwrite(tmp,strlen(tmp),1, fd);
	fclose(fd);
	return 0;
}

