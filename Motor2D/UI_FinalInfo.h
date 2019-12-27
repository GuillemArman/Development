#ifndef __UI_FINALINFO
#define __UI_FINALINFO

#include "UI_element.h"

class Image;

class FinalInfo : public UI_element
{
public:
	FinalInfo(int x, int y, j1Module* callback);
	~FinalInfo();

	void BlitElement();

public:

	Image* big_coin = nullptr;
	Text* final_score = nullptr;
	Text* total_coins = nullptr;
	Text* total_time = nullptr;	
};


#endif // !__UI_FINALINFO