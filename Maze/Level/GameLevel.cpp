#include "GameLevel.h"
#include "ClearLevel.h"
#include "FailedLevel.h"

#include "Engine/Engine.h"
#include "Engine/Timer.h"

#include "Actor/Player.h"
#include "Actor/Goal.h"
#include "Actor/Enemy.h"
#include "Actor/Wall.h"
#include "Actor/Ground.h"
#include "Actor/Seed.h"
#include "Actor/Projectile.h"

#include <string>
#include <fstream>
#include <algorithm>

GameLevel::GameLevel(int stageNum,const std::string& fileName,int startX,int startY,int width,int height)
	: stageNum(stageNum),consoleX(startX),consoleY(startY),consoleWidth(width),consoleHeight(height)
{
	// 커서 감추기
	Engine::Get().SetCursorType(CursorType::NoCursor);

	// 맵 파일 읽기
	std::ifstream file(fileName);
	if(!file.is_open())
	{
		std::cerr << "Failed to open map file: " << fileName << std::endl;
		__debugbreak();
	}

	int yPosition = 0; // y 좌표 초기화
	std::string line;

	// 파일 줄 단위로 읽기
	while(std::getline(file,line))
	{
		std::vector<Actor*> row; // 각 줄의 액터 저장
		for(int xPosition = 0; xPosition < static_cast<int>(line.size()); ++xPosition)
		{
			char mapChar = line[xPosition];

			// 문자에 따라 액터 생성
			if(mapChar == '1') {
				Wall* wall = new Wall(Vector2(xPosition,yPosition));
				row.push_back(wall);
			} else if(mapChar == '.') {
				Ground* ground = new Ground(Vector2(xPosition,yPosition));
				row.push_back(ground);
			} else if(mapChar == 'S') {
				Seed* seed = new Seed(Vector2(xPosition,yPosition));
				row.push_back(seed);
			} else if(mapChar == 'E') {
				Enemy* enemy = new Enemy(Vector2(xPosition,yPosition),this);
				row.push_back(enemy);
				enemies.push_back(enemy);
			} else if(mapChar == 'G') {
				Goal* goal = new Goal(Vector2(xPosition,yPosition));
				row.push_back(goal);
				goals.push_back(goal);
			} else {
				row.push_back(nullptr); // 빈 공간
			}
		}
		mapData.push_back(row);
		++yPosition; // 다음 줄로 이동
	}

	// 맵 크기 설정
	mapHeight = static_cast<int>(mapData.size());
	mapWidth = mapData.empty() ? 0 : static_cast<int>(mapData[0].size());

	// 플레이어 생성 위치 설정
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	float scaleX = static_cast<float>(screenWidth) / mapWidth;
	float scaleY = static_cast<float>(screenHeight) / mapHeight;
	float HalfX = 350 / 12 / 2;
	float HalfY = 350 / 20 / 2;
	float mapX = consoleX / scaleX + HalfX;
	float mapY = consoleY / scaleY + HalfY;

	player = new Player(Vector2(static_cast<int>(mapX),static_cast<int>(mapY)),this);

	file.close();
}

void GameLevel::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// 플레이어 업데이트
	if(player) player->Update(deltaTime);

	timeLimit -= deltaTime;

	// 스테이지 클리어 처리
	if(isGameClear)
	{
		// 타이머 클래스의 객체로 코드 정리
		static Timer timer(0.1);
		timer.Update(deltaTime);
		if(!timer.IsTimeOut()) return;

		if(stageNum <= 3)
		{
			int totalScore = player->GetHP() + player->GetSeeds() + player->GetTakeEnemy() + static_cast<int>(timeLimit);

			// 클리어 레벨 로드
			Engine::Get().LoadLevel(new ClearLevel(stageNum,totalScore,static_cast<int>(60-timeLimit)));
		}
	}

	// 게임 오버 처리
	if(isGameOver)
	{
		if(stageNum <= 3)
		{
			// 실패 레벨 로드
			Engine::Get().LoadLevel(new FailedLevel(stageNum));
		}
	}
}

void GameLevel::Draw()
{
	//// 맵 크기와 스크린 비율 계산
	float scaleX = static_cast<float>(GetSystemMetrics(SM_CXSCREEN)) / mapWidth;
	float scaleY = static_cast<float>(GetSystemMetrics(SM_CYSCREEN)) / mapHeight;

	// 적 이동 및 mapData 업데이트
	for(auto* enemy : enemies)
	{
		if(!enemy) continue;

		// 적의 현재 위치
		Vector2 currentPos = enemy->Position();
		Vector2 newPos = currentPos;

		// 적의 방향에 따라 이동하려는 위치 계산
		switch(enemy->GetDirection())
		{
		case Enemy::Direction::Up:
			newPos.y -= 1;
			break;
		case Enemy::Direction::Down:
			newPos.y += 1;
			break;
		case Enemy::Direction::Left:
			newPos.x -= 1;
			break;
		case Enemy::Direction::Right:
			newPos.x += 1;
			break;
		default:
			break;
		}

		// 맵 범위 확인
		if(newPos.x < 0 || newPos.y < 0 || newPos.x >= mapWidth || newPos.y >= mapHeight)
			continue;

		// 이동하려는 위치의 Actor 확인
		Actor* targetActor = mapData[newPos.y][newPos.x];
		if(dynamic_cast<Ground*>(targetActor))
		{
			// 현재 위치와 새 위치의 Actor를 Swap
			std::swap(mapData[currentPos.y][currentPos.x],mapData[newPos.y][newPos.x]);

			// 적의 위치 갱신
			enemy->SetPosition(newPos);
		} else
		{
			// 이동할 수 없으면 방향을 반대로 변경
			enemy->ReverseDirection();
		}
	}

	// 화면에 보일 부분만 백 버퍼에 렌더링
	for(int y = 0; y < consoleHeight; ++y)
	{
		float mapY = consoleY / scaleY + y; // 맵의 Y 좌표 계산
		if(static_cast<int>(mapY)< 0 || static_cast<int>(mapY) >= mapHeight) continue;  // mapData 범위 초과 시 무시

		for(int x = 0; x < consoleWidth; ++x)
		{
			float mapX = consoleX / scaleX + x ; // 맵의 X 좌표 계산
			if(static_cast<int>(mapX) < 0 || static_cast<int>(mapX) >= mapWidth) continue;  // mapData 범위 초과 시 무시

			// 맵 데이터에서 Actor 가져오기
			DrawableActor* actor = dynamic_cast<DrawableActor*>(mapData[static_cast<int>(mapY)][static_cast<int>(mapX)]);
			if(actor)
			{
				Engine::Get().Draw(Vector2(x,y),actor->GetSymbol(),actor->GetColor());

				auto& projectiles = player->GetProjectiles();
						projectiles.erase(std::remove_if(projectiles.begin(),projectiles.end(),[&](Projectile* projectile)
						{
							if(dynamic_cast<Wall*>(actor) && actor->Position() == projectile->Position())
							{
								delete projectile;
								return true;
							}
							return false;
						}),
			projectiles.end()
						);

				/*for(auto it = projectiles.begin(); it != projectiles.end();)
				{
					if(dynamic_cast<Wall*>(actor) && actor->Position() == (*it)->Position())
					{
						// Wall에 충돌한 발사체 제거
						delete *it;
						it = projectiles.erase(it);
						break;
					}

					if(!dynamic_cast<Seed*>(actor) && !dynamic_cast<Wall*>(actor) && actor->Position() == (*it)->Position())
					{
						Engine::Get().Draw(Vector2(x,y),(*it)->GetSymbol(),(*it)->GetColor());
						++it;
					} else
					{
						++it;
					}
				}*/

				/*for(auto* projectile : player->GetProjectiles())
				{
					if(dynamic_cast<Wall*>(actor) && actor->Position() == projectile->Position())
					{
						break;
					}
					
					if(!dynamic_cast<Seed*>(actor) && !dynamic_cast<Wall*>(actor) && actor->Position() == projectile->Position())
					{
						Engine::Get().Draw(Vector2(x,y),projectile->GetSymbol(),projectile->GetColor());
					}
				}*/

				if(actor->Position() == player->Position())
				{
					Engine::Get().Draw(Vector2(x,y),player->GetSymbol(),player->GetColor());
				}
			}

		}
	}

	// 텍스트 출력 시작 위치
	int startX = 0,startY = 0;

	// Life, Seeds, Time 출력
	int totalSeconds = timeLimit;
	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;
	std::string time = "Time : ";
	std::string life = "Life : " + std::to_string(player->GetHP());
	std::string seeds = "Seeds : " + std::to_string(player->GetSeeds());
	std::string enemies = "Enemies : " + std::to_string(player->GetTakeEnemy());

	if(minutes > 0)
	{
		time += std::to_string(minutes) + "min " + std::to_string(seconds) + "sec";
	} else
	{
		time += std::to_string(seconds) + "sec";
	}

	Engine::Get().Draw(Vector2(startX,startY),life.c_str(),Color::Aqua);
	Engine::Get().Draw(Vector2(startX,startY+1),seeds.c_str(),Color::Aqua);
	Engine::Get().Draw(Vector2(startX,startY+2),enemies.c_str(),Color::Aqua);
	Engine::Get().Draw(Vector2(startX,startY+3),time.c_str(),Color::Aqua);

}

bool GameLevel::CanPlayerMove(const Vector2& position)
{
	// 게임이 클리어 또는 오버된 경우 바로 종료
	if(CheckGameClear()) return false;
	if(CheckGameOver()) return false;

	int x = position.x;
	int y = position.y;

	if(x<0 || y<0 || y >=mapHeight || x>= mapWidth) return false;

	Actor* targetActor = mapData[y][x];

	if(!targetActor) return true;

	if(dynamic_cast<Wall*>(targetActor)) return false;

	if(dynamic_cast<Ground*>(targetActor) || dynamic_cast<Goal*>(targetActor)) return true;

	if(dynamic_cast<Enemy*>(targetActor))
	{
		player->TakeDamage();
	}

	if(dynamic_cast<Seed*>(targetActor))
	{
		// 배열에서 targetActor를 찾아 제거
		auto it = std::find(actors.begin(),actors.end(),targetActor);
		if(it != actors.end())
		{
			delete *it;
			*it = nullptr;
			actors.erase(it);
		}

		// 맵 데이터에서도 제거
		mapData[y][x] = nullptr;
		player->TakeSeed();
		return true;
	}

	return false;
}

bool GameLevel::CanEnemyMove(const Vector2 & position)
{
	// 게임이 클리어 또는 오버된 경우 바로 종료
	if(CheckGameClear()) return false;
	if(CheckGameOver()) return false;

	int x = position.x;
	int y = position.y;

	if(x<0 || y<0 || y >=mapHeight || x>= mapWidth) return false;

	Actor* targetActor = mapData[y][x];

	if(dynamic_cast<Wall*>(targetActor))
	{
		return false;
	}

	if(dynamic_cast<Player*>(targetActor)
		|| dynamic_cast<Goal*>(targetActor)
		|| dynamic_cast<Seed*>(targetActor)
		|| dynamic_cast<Ground*>(targetActor)) return true;

	return false;
}

bool GameLevel::EnemyOverlapWall(const Vector2 & position)
{
	int x = position.x;
	int y = position.y;

	Actor* targetActor = mapData[y][x];

	if(dynamic_cast<Wall*>(targetActor))
	{
		return true;
	}

	return false;
}

bool GameLevel::CheckGameClear()
{
	for(auto* goal : goals)
	{
		if(player->Position() == goal->Position())
		{
			isGameClear = true;
			return true;
		}
	}
	
	return false;
}

bool GameLevel::CheckGameOver()
{
	if(player->GetHP() <= 0 || timeLimit <= 0)
	{
		isGameOver = true;
		return true;
	}

	return false;
}

void GameLevel::MoveConsole(int dx,int dy)
{
	consoleX += dx;
	consoleY += dy;

	if(consoleX < 0) consoleX = 0;
	if(consoleY < 0) consoleY = 0;

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if(consoleX + consoleWidth > screenWidth) consoleX = screenWidth - consoleWidth;
	if(consoleY + consoleHeight > screenHeight) consoleY = screenHeight - consoleHeight;

	// 콘솔 창 이동
	HWND consoleWindow = GetConsoleWindow();
	if(consoleWindow == NULL) return;
	MoveWindow(consoleWindow,consoleX,consoleY,consoleWidth,consoleHeight,TRUE);
}
