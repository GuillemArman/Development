#include "j1Player.h"
#include "j1Collision.h"

j1Player::j1Player() :j1Module()
{
	name.create("player1");
}

j1Player::~j1Player()
{

}

bool j1Player::Start() {
	return true;
}

bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update() {

	return true;
}

bool j1Player::PostUpdate() {

	return true;
}

bool j1Player::Load(pugi::xml_node& data)
{
	//loading player pos from xml
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	return true;

}

bool j1Player::Save(pugi::xml_node& data) const
{
	//saving pos into xml

	pugi::xml_node player = data.append_child("position");
	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;
	return true;
}

//iPoint j1Player::GetPosition()
//{
//	return position;
//}