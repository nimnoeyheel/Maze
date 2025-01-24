#include "Player.h"
#include "Engine/Engine.h"
#include "Game/Game.h"
#include "Level/GameLevel.h"
#include "Projectile.h"
#include "Enemy.h"
#include <vector>
#include "Ground.h"

Player::Player(const Vector2& position, GameLevel* level)
	: DrawableActor("P"), refLevel(level)
{
	// 위치 설정
	this->position = position;

	// 색상 설정
	color = Color::White;
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime);
	currentTime += deltaTime;
	this->deltaTime = deltaTime;

	// ESC 종료
	if (Engine::Get().GetKeyDown(VK_ESCAPE))
	{
		// 메뉴 토글
		Game::Get().ToggleMenu();
	}

	Vector2 direction(0,0);

	// 상하좌우 이동
	if (Engine::Get().GetKey(VK_LEFT))
	{
		direction = Vector2(-1, 0);
		lastDirection = direction;
	}
	if (Engine::Get().GetKey(VK_RIGHT))
	{
		direction = Vector2(1, 0);
		lastDirection = direction;
	}
	if (Engine::Get().GetKey(VK_UP))
	{
		direction = Vector2(0,-1);
		lastDirection = direction;
	}
	if (Engine::Get().GetKey(VK_DOWN))
	{
		direction = Vector2(0, 1);
		lastDirection = direction;
	}

	// 공격 처리
	if(Engine::Get().GetKeyDown(VK_SPACE) && lastDirection != Vector2(0,0))
	{
		Attack();
	}

	// 발사체 업데이트
	UpdateProjectiles();

	// 이동 방향이 설정되었으면 이동 처리
	if(direction != Vector2(0,0))
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		float scaleX = static_cast<float>(screenWidth) / refLevel->mapWidth;
		float scaleY = static_cast<float>(screenHeight) / refLevel->mapHeight;
		float HalfX = 350 / 12 / 2;
		float HalfY = 350 / 20 / 2;
		float mapX = refLevel->consoleX / scaleX + HalfX;
		float mapY = refLevel->consoleY / scaleY + HalfY;

		Vector2 newPosition = Vector2(static_cast<int>(mapX),static_cast<int>(mapY)) + direction;
		
		if(refLevel->CanPlayerMove(newPosition))
		{
			SetPosition(newPosition);
			refLevel->MoveConsole(direction.x * 5,direction.y * 5);
		}
	}
}

void Player::Attack()
{
	Vector2 projectilePos = position;

	//float timeInterval = 1.0f; // 각 발사체 발사 간격
	//float currentTime = 0.0f;

	for(int i = 1; i <= 10; i++)
	{
		projectilePos = position + Vector2(lastDirection.x * i, lastDirection.y * i);
		Projectile* projectile = new Projectile(projectilePos);

		// 발사체에 초기 시간 설정
		//projectile->SetStartTime(currentTime);
		
		projectiles.push_back(projectile);

		// 다음 발사체 발사 시점
		//currentTime += timeInterval;
	}
}

void Player::UpdateProjectiles()
{
	const float delay = 0.2f;

	//for(size_t i = 0; i < projectiles.size(); ++i)
	//{
	//	Projectile* projectile = projectiles[i];
	//	
	//	// 발사체의 렌더링 시작 시간을 설정
	//	if(projectile->GetStartTime() > 0) // 초기값 -1이면 설정
	//	{
	//		projectile->SetStartTime(currentTime + i * delay);
	//	}

	//	// 발사체 업데이트
	//	bool shouldDestroy = projectile->UpdateProjectile(this->deltaTime,currentTime);

	//	if(shouldDestroy)
	//	{
	//		delete projectile;
	//		projectiles.erase(projectiles.begin()+i);
	//		--i;
	//	}
	//}

	for(auto it = projectiles.begin(); it != projectiles.end();)
	{
		Projectile* projectile = *it;


		//bool shouldDestroy = projectile->UpdateProjectile(this->deltaTime, currentTime);

		/*if(shouldDestroy)
		{
			delete projectile;
			it = projectiles.erase(it);
			continue;
		}*/

		Vector2 projectilePos = projectile->Position();

		// Enemy와 충돌 체크
		bool bIsCollided = false;
		for(int y = 0; y < refLevel->mapHeight; y++)
		{
 			for(int x = 0; x < refLevel->mapWidth; x++)
			{
				Actor* actor = refLevel->mapData[y][x];
				if(Enemy* enemy = dynamic_cast<Enemy*>(actor))
				{
					if(enemy->Position() == projectilePos)
					{
						// 잡은 에너미 수 누적
						TakeEnemise();

						// Enemy 제거
						auto enemyIt = std::find(refLevel->enemies.begin(),refLevel->enemies.end(),enemy);
						if(enemyIt != refLevel->enemies.end())
						{
							delete *enemyIt;
							*enemyIt = nullptr;
							refLevel->enemies.erase(enemyIt);
						}

						// 맵 데이터에서도 제거하고 새 ground 객체 추가
						refLevel->mapData[y][x] = nullptr;
						Ground* ground = new Ground(Vector2(projectilePos.x,projectilePos.y));
						refLevel->mapData[y][x] = ground;
						
						bIsCollided = true;
						break;
					}
				}
			}
			if(bIsCollided) break;
		}
		if(bIsCollided)
		{
			delete projectile;
			it = projectiles.erase(it);
		}
		else
		{
			++it;
		}
	}
}
