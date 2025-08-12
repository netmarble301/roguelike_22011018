7/10 ImageManager 구현(GDI+), 랜덤 맵 생성 및 랜덤 오브젝트 배치
7/11 스마트포인터(unique_ptr)의 reset & nullptr구현, Monster & MonsterFactory 구현
7/12 GameLogic 과 GameManager 분리
7/16 MapData를 enum class으로 수정(MapDataType), monster 종류 구분하기 쉽게
7/23 monsterMove(몬스터 랜덤이동) 구현
7/27 A*알고리즘 구현(로그라이크 상하좌우만 이동 가능, 장애물(1) 때문에 일단 MapDataType형으로 제작, 나중에 시간나면 싱글톤으로 구현 범용적으로 사용 가능하게), A*알고리즘 이용해서 monsterMove 수정
7/28 searchPlayer(5x5범위 플레이어 탐색) & monsterChase(플레이어 추격) 구현, gameManager > gameLogic > roguelike_game 순서로 호출하도록
7/31 플레이어와 몬스터 서로 턴 넘어갈 때 살짝 타임스탑하기(일단 setTimer 사용)
8/7 monster & player 기본 공격 구현, 플레이어 사망 시 게임 종료 함수 구현
8/12 ImageManager 제대로 수정

목표: 직관적으로 구현(스테이터스 같이 공통된 부분 따로 묶어서 구현), 아이템(플레이어 체력 회복) 구현, 맵 안개(비트연산자 말고 다른 방법 있는지 확인), 
층수에 따라 오브젝트 출현 확률 변경?, 플레이어 스킬 구현, 게임 현재 상황을 게임로그에 출력하도록(필수x), 플레이어가 몬스터들에게 둘러싸였을 때 몬스터 한 명만 플레이어 공격하는 문제 해결하기,
