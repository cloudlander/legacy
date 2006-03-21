#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include "../wRec.h"
using namespace std;

int main()
{
	char buf[3];
	cout<<"Totally "<<GetDeviceCount()<<" devices"<<endl;
	cout<<"Press ENTER key to start recording";
	fgets(buf,3,stdin);
#if 0
	if(!mp3RecInit(-1,2,44100,16,128,FALSE,"test.mp3"))
		return 1;
#else
	if(!wavRecInit(-1,2,22050,16,"test.wav"))
		return 1;
#endif
	if(!RecStart(0))
		return 1;
	cout<<endl<<"Recording has started. Press ENTER key to stop recording...";
	getchar();
	if(!RecStop(0))
		return 1;
	return 0;
}