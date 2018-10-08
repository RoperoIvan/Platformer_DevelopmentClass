#ifndef __J1FADETOBLACK_H
#define __J1FADETOBLACK_H


class j1FadeToBlack : public j1Module
{
public:

	j1FadeToBlack();
	~j1FadeToBlack();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	
};



#endif
