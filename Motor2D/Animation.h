#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 25

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;
	/*pugi::xml_document player_animations_file;*/
	pugi::xml_document	file;
	

public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
		pugi::xml_document	animations_file;
	}

	

	void Animation_XML(p2SString name)
	{
		pugi::xml_parse_result result = file.load_file("player_animations.xml");
		if (result != NULL)
		{
			pugi::xml_node animation = file.child("test").child(name.GetString());
			speed = animation.attribute("speed").as_float();

			loop = animation.attribute("loop").as_bool();


			for (pugi::xml_node frames = animation.child("frame"); frames; frames = frames.next_sibling("frame"))
			{
				PushBack({ frames.attribute("x").as_int(), frames.attribute("y").as_int(), frames.attribute("width").as_int(), frames.attribute("height").as_int() });
			}

		}

	}

	

	
	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		loops = 0;
		current_frame = 0.0f;
	}


};

#endif // !_ANIMATION_H_