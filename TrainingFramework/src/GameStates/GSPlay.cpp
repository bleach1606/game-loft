#include "GSPlay.h"
#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Text.h"

extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine

GSPlay::GSPlay()
{
}

GSPlay::~GSPlay()
{

}

void GSPlay::InitTank(int k, int f) {

	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("Hull_06");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	// xet huong
	direction = Direction_A;
	old_direction = Direction_W;
	if (k)
	{
		m_tank = std::make_shared<Sprite2D>(model, shader, texture);
		m_tank->Set2DPosition(ax, ay);
		m_tank->SetSize(64, 64);

		texture = ResourceManagers::GetInstance()->GetTexture("Gun_07_A");
		m_Weapon = std::make_shared<Sprite2D>(model, shader, texture);
		m_Weapon->Set2DPosition(ax, ay + f * 40);
		m_Weapon->SetSize(9, 28);
		return;
	}

	//show tank
	texture = ResourceManagers::GetInstance()->GetTexture("Hull_06_01");
	//shader = ResourceManagers::GetInstance()->GetShader("Hull_06");
	m_tank = std::make_shared<Sprite2D>(model, shader, texture);
	m_tank->Set2DPosition(ax, ay);
	m_tank->SetSize(64, 64);

	texture = ResourceManagers::GetInstance()->GetTexture("Gun_07_A_01");
	m_Weapon = std::make_shared<Sprite2D>(model, shader, texture);
	m_Weapon->Set2DPosition(ax + f * 40, ay);
	m_Weapon->SetSize(28, 9);
}

void GSPlay::SetDirectionTank() {
	for each (std::shared_ptr<Tank> tank in lst_tank) {
		if (tank->getDirectionk() % 2)
		{
			tank->SetTexture(ResourceManagers::GetInstance()->GetTexture("Hull_06"));
		}
		else {
			tank->SetTexture(ResourceManagers::GetInstance()->GetTexture("Hull_06_01"));
		}
	}
}

void GSPlay::Init()
{
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			mp[i][j] = 0;
	ax = ay = 400;
	currentTime = 0.0f;
	direction = Direction_Stop;

	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play");

	//BackGround
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_BackGround->SetSize(screenWidth, screenHeight);

	//text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_score = std::make_shared< Text>(shader, font, "score: 10", TEXT_COLOR::RED, 1.0);
	m_score->Set2DPosition(Vector2(5, 25));

	// view tank
	InitTank(1, -1);

	for (int i = 0; i < 5; i++) {
		auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
		auto texture = ResourceManagers::GetInstance()->GetTexture("Hull_06");
		auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
		std::shared_ptr<Tank> tank = std::make_shared<Tank>(model, shader, texture);
		tank->SetAx(ax + (i + 2) *40);
		tank->SetAy(ay - (i + 2) *40);
		tank->SetDirectionk(direction);
		tank->Set2DPosition(tank->GetAx(), tank->GetAy());
		tank->SetSize(64, 64);
		lst_tank.push_back(tank);
	}
}

void GSPlay::Exit()
{

}


void GSPlay::Pause()
{

}

void GSPlay::Resume()
{

}


void GSPlay::HandleEvents()
{

}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{

	if (bIsPressed)
	{
		if (key == 'W' || key == 'w' || key == 'S' || key == 's')
		{
			
			if (key == 'S' || key == 's') {
				InitTank(1, 1);
				old_direction = direction = Direction_S;
			}
			else {
				InitTank(1, -1);
				old_direction = direction = Direction_W;
			}
		}
		else if (key == 'A' || key == 'a' || key == 'D' || key == 'd')
		{
			
			if (key == 'A' || key == 'a') {
				InitTank(0, -1);
				old_direction = direction = Direction_A;
			}
			else {
				InitTank(0, 1);
				old_direction = direction = Direction_D;
			}
		}
		else if (key == 32)
		{
			auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
			auto texture = ResourceManagers::GetInstance()->GetTexture("bullet");
			auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
			std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(model, shader, texture);
			bullet->SetAx(ax);
			bullet->SetAy(ay);
			bullet->SetDirectionk(old_direction);

			switch (bullet->getDirectionk())
			{
			case Direction_A:
				bullet->SetAx(bullet->GetAx() - 40);
				break;
			case Direction_D:
				bullet->SetAx(bullet->GetAx() + 40);
				break;
			case Direction_W:
				bullet->SetAy(bullet->GetAy() - 40);
				break;
			case Direction_S:
				bullet->SetAy(bullet->GetAy() + 40);
				break;
			default:
				break;
			}
			
			bullet->Set2DPosition(bullet->GetAx(), bullet->GetAy());
			bullet->SetSize(128, 128);
			lst_bullet.push_back(bullet);
		}
	}
	else
	{
		if (key == 'W' || key == 'w' || key == 'S' || key == 's' || key == 'A' || key == 'a' || key == 'D' || key == 'd') 
		{
			direction = Direction_Stop;
		}

	}
	
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
}

void GSPlay::Update(float deltaTime)
{
	currentTime += deltaTime;
	if (currentTime > 0.06f)
	{
		currentTime -= 0.06f;
		switch (direction)
		{
			case Direction_A:
				ax -= 10;
				if (ax < 0 ) ax = 0;
				m_Weapon->Set2DPosition(ax-20, ay);
				break;
			case Direction_D:
				ax += 10;
				if (ax >  750) ax = 750;
				m_Weapon->Set2DPosition(ax + 20, ay);
				break;
			case Direction_W:
				ay -= 10;
				if (ay < 0) ay = 0;
				m_Weapon->Set2DPosition(ax, ay - 20);
				break;
			case Direction_S:
				ay += 10;
				if (ay > 750) ay = 750;
				m_Weapon->Set2DPosition(ax, ay + 20);
				break;
			default:
				break;
		}
		m_tank->Set2DPosition(ax, ay);
		auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
		auto texture = ResourceManagers::GetInstance()->GetTexture("Explosion_A");
		auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
		
		std::list<std::shared_ptr<Bum>> bums;
		for each (std::shared_ptr<Bum> bum in lst_bum) {
			bum->SetFram(bum->getFram() + 1);
			if (bum->getFram() < 8) {
				std::string s = "Explosion_A";
				s[10] += bum->getFram();
				bum->SetTexture(ResourceManagers::GetInstance()->GetTexture(s));
				bums.push_back(bum);
			}
		}

		lst_bum.clear();
		lst_bum = bums;
		std::list<std::shared_ptr<Bullet>> temp;
		for each (std::shared_ptr<Bullet> bullet in lst_bullet)
		{
			switch (bullet->getDirectionk())
			{
			case Direction_A:
				bullet->SetAx(bullet->GetAx() - 40);
				break;
			case Direction_D:
				bullet->SetAx(bullet->GetAx() + 40);
				break;
			case Direction_W:
				bullet->SetAy(bullet->GetAy() - 40);
				break;
			case Direction_S:
				bullet->SetAy(bullet->GetAy() + 40);
				break;
			default:
				break;
			}
			if ( 0 > bullet->GetAx() || bullet->GetAx() > 800 || 0 > bullet->GetAy() || bullet->GetAy() > 800)
			{
				//lst_bullet.erase()
				//bullet->~Bullet();
			}
			else {
				std::list<std::shared_ptr<Tank>> tankss;
				int ok = 0;
				for each (std::shared_ptr<Tank> tank in lst_tank) {
					if (ok) {
						tankss.push_back(tank);
						continue;
					}
					ok = tank->IsDie(bullet->GetAx(), bullet->GetAy(), 10);
					if (ok) {
						//tank->~Tank();
						
						std::shared_ptr<Bum> bum = std::make_shared<Bum>(model, shader, texture);
						// xet huong
						bum->SetFram(0);
						bum->Set2DPosition(tank->GetAx(), tank->GetAy());
						bum->SetSize(64, 64);
						bum->Draw();
						lst_bum.push_back(bum);
					}
					else {
						tankss.push_back(tank);
					}
				}
				lst_tank.clear();
				lst_tank = tankss;
				if(!ok) temp.push_back(bullet);
			}
			bullet->Set2DPosition(bullet->GetAx(), bullet->GetAy());
			//bullet->Draw();
		}
		
		lst_bullet.clear();
		lst_bullet = temp;
		
		for each (std::shared_ptr<Tank> tank in lst_tank) {
			tank->RandomDir();
			switch (tank->getDirectionk())
			{
			case Direction_A:
				tank->SetAx(tank->GetAx() - 10);
				if (tank->GetAx() < 20) tank->SetAx(20);
				break;
			case Direction_D:
				tank->SetAx(tank->GetAx() + 10);
				if ((tank->GetAx() > 700)) tank->SetAx(700);
				break;
			case Direction_W:
				tank->SetAy(tank->GetAy() - 10);
				if (tank->GetAy() < 20) tank->SetAy(20);
				break;
			case Direction_S:
				tank->SetAy(tank->GetAy() + 10);
				if (tank->GetAy() > 700) tank->SetAy(700);
				break;
			default:
				break;
			}
			tank->Set2DPosition(tank->GetAx(), tank->GetAy());
			
		}	

		SetDirectionTank();
	}
}


void GSPlay::Draw()
{
	m_BackGround->Draw();
	m_tank->Draw();
	m_Weapon->Draw();
	for each (std::shared_ptr<Bullet> bullet in lst_bullet) {
		bullet->Draw();
	}

	for each (std::shared_ptr<Tank> tank in lst_tank) {
		tank->Draw();
	}

	for each (std::shared_ptr<Bum> bum in lst_bum) {
		bum->Draw();
	}
}

void GSPlay::SetNewPostionForBullet()
{
}