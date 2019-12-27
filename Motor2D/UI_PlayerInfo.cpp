#include "UI_PlayerInfo.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "p2DynArray.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "j1Player.h"
#include "j1EntityManager.h"
#include "Brofiler\Brofiler.h"

PlayerInfo::PlayerInfo(int x, int y, j1Module* callback) : UI_element(x, y, PLAYERINFO, { 0,0,0,0 }, callback)
{
	_TTF_Font* mid_texts_font = App->font->Load("fonts/BMYEONSUNG.ttf", 36);
	SDL_Color black_color = { 0, 0, 0, 255 };
	
	god_live = App->gui->createImageFromAtlas(x + 60, y + 3, { 880, 290, 50, 50 });
	
	j1Player* player = (j1Player*)App->entityManager->getPlayer();

	coin = App->gui->createImageFromAtlas(x + 152, y + 3, { 895, 189, 41, 41 });
	live = App->gui->createImageFromAtlas(x + 60, y + 0, { 880, 235, 70, 50 });
	empty_live = App->gui->createImageFromAtlas(x + 62, y + 3, { 431, 449, 51, 48 });
	score = App->gui->createText("000", x + 230, y + 3, mid_texts_font, black_color);
	coin_txt = App->gui->createText("0", x + 180, y + 3, mid_texts_font, black_color);
}


PlayerInfo::~PlayerInfo()
{
}

void PlayerInfo::BlitElement()
{
	BROFILER_CATEGORY("PlayerInfo Blit", Profiler::Color::Fuchsia);

	
	iPoint coinPos = coin->calculateAbsolutePosition();
	iPoint godlivePos = god_live->calculateAbsolutePosition();
	iPoint livePos = live->calculateAbsolutePosition();

	j1Player* player = (j1Player*)App->entityManager->getPlayer();
	p2SString score_str("SCORE: %03d", player->score);
	score->setText(score_str);
	score->BlitElement();

	p2SString coin_str("x %d", player->num_coins);
	coin_txt->setText(coin_str);
	coin_txt->BlitElement();


	for (int i = 0; i < 3; i++)
	{
		if (App->entityManager->player_god_mode)
			break;

		if (i < player->lives)
			App->render->Blit(live->texture, livePos.x + (i * 28), livePos.y, &live->section, false, App->gui->UI_scale);
		else
			App->render->Blit(empty_live->texture, livePos.x + (i * 28), livePos.y, &empty_live->section, false, App->gui->UI_scale);
	}

	if (App->entityManager->player_god_mode)
	{
		App->render->Blit(god_live->texture, godlivePos.x, godlivePos.y, &god_live->section, false, App->gui->UI_scale);
	}

	App->render->Blit(coin->texture,coinPos.x,coinPos.y, &coin->section, false, App->gui->UI_scale);
}