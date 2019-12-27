#include "UI_FinalInfo.h"
#include "j1Fonts.h"
#include "j1Player.h"
#include "j1Gui.h"
#include "j1EntityManager.h"
#include "j1Render.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "Brofiler\Brofiler.h"

FinalInfo::FinalInfo(int x, int y, j1Module* callback) : UI_element(x, y, FINALINFO, { 0,0,0,0 }, callback)
{
	_TTF_Font* big_texts_font = App->font->Load("fonts/BMYEONSUNG.ttf", 55);
	SDL_Color white_color = { 255, 255, 255, 0 };
	big_coin = App->gui->createImageFromAtlas(x + 465 * App->gui->UI_scale, y + 242 * App->gui->UI_scale, { 11, 457, 82, 82 });
	final_score = App->gui->createText("0", x + 515 * App->gui->UI_scale, y + 377 * App->gui->UI_scale, big_texts_font, white_color);

}


FinalInfo::~FinalInfo()
{}

void FinalInfo::BlitElement()
{
	BROFILER_CATEGORY("FinalInfo Blit", Profiler::Color::NavajoWhite);


	iPoint big_coinPos = big_coin->calculateAbsolutePosition();


	j1Player* player = (j1Player*)App->entityManager->getPlayer();
	p2SString score_str("SCORE: %d points", player->score);
	final_score->setText(score_str);
	final_score->BlitElement();

	
	if (player->coins != 0)
	{
		App->render->Blit(big_coin->texture, big_coinPos.x + 200, big_coinPos.y, &big_coin->section, false, App->gui->UI_scale);
	}

	p2SString coins_str("X %d coins", player->coins);
	/*p2SString coins_str("X %d coins", player->coins);
	coins_textPos.y + 100;
	coins_text->setText(coins_str);
	coins_text->BlitElement();*/


}