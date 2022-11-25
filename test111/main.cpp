#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class Yut
{
private:
	int player1[4]; //플레이어 말
	int player2[4];
	int board[25]; //보드 (21~25 == 골인)
	int dice[4]; //윷
	int* moveNum; //윷 저장
	int b_len; //보드배열 길이
	bool playing;
	bool turn;
public:
	Yut(); //초기화용
	void roll(); //윷 던지기
	void move(int player); //이동
	bool randT(); //시작 순서
	bool bonusT(); //추가턴 확인
	bool catchs(int num); //말 잡기or 업기
	bool goal(); //골인 확인
	bool win(); //승리 확인
	void showPlayer();
};

Yut::Yut()
{
	b_len = sizeof(board) / sizeof(int);
	for (int i = 0; i < 4; i++) { player1[i] = 0; } //말 리셋
	for (int i = 0; i < 4; i++) { player2[i] = 0; }
	for (int i = 0; i < b_len; i++) { board[i] = 0; } //보드 리셋
	for (int i = 0; i < 4; i++) { dice[i] = 0; }
	playing = true;
	turn = NULL;
}
void Yut::roll()
{
	int roll = 0;
	int i = 0;
	bool temp = true;
	
	moveNum = new int[i + 1];
	while (temp) {
		if (i > 0) {
			int* tempNum = new int[i + 1];
			for (int r = 0; r < i; r++)
				tempNum[r] = moveNum[r];
			delete[] moveNum;
			moveNum = tempNum;
			delete[] tempNum;
			tempNum = NULL;
		}
		for (int i = 0; i < 4; i++) {
			srand(time(NULL) + i); 
			dice[i] = rand() % 2; 
			roll += dice[i]; 
		}
		printf("결과 값 = ");
		switch (roll)
		{
		case 0:
		{ moveNum[i] = 5; printf("모"); break; }
		case 1:
			if (dice[0] == 1)
			{
				moveNum[i] = -1; printf("백도");
				temp = false;
			}
			else {
				moveNum[i] = 1; printf("도");
				temp = false;
			}
			break;
		case 2: {
			moveNum[i] = 2; printf("개");
			temp = false;
			break;
		}
		case 3: {
			moveNum[i] = 3; printf("걸");
			temp = false;
			break;
		}
		case 4: {
			moveNum[i] = 4; printf("윷"); }
			  break;
		}
		printf("\n");
		i++;
	}
}
void Yut::move(int player)
{
	int num;
	while (moveNum[0] != NULL) {
		printf("이동 가능한 윷 :");
		for (int i = 0; i < _msize(moveNum) / sizeof(int); i++)
			printf("%d / ", moveNum[i]);
		printf("이동 : ");
		scanf("%d", &num);
		int whereami = player1[player];
		int checknum = player1[player] + moveNum[num];
		printf("선택된 값은 %d입니다. 말의 현재 위치는 %d입니다. 이동후 예상 위치는 %d입니다.\n", moveNum[num], player1[player], checknum);
		if (checknum != 0 && board[checknum] != 0) // 이동시킬 자리에 무언가 다른정보가 있을 경우
		{
			if (catchs(checknum))
				printf("상대편 말을 잡았습니다.");
			else
				printf("말을 업었습니다.");
		}
		for (int i = 0; i < 4; i++) {			//업힌 상태인 경우 한번에 이동
			if ((whereami > 0) && (whereami == player1[i]))
				player1[i] = checknum;
		}
		if (player1[player] == 0)
			player1[player] = checknum;
		printf("%d번말이 %d번으로 이동 했습니다.\n", player, player1[player]);
		moveNum[num] = NULL;
		for (int i = num; i < (_msize(moveNum) / sizeof(int)) - 1; i++) {
			moveNum[i] = moveNum[i + 1];
			moveNum[i + 1] = NULL;
		}
		printf("이동완료 \n");
		if (player1[player] > 20)
			goal();
	}
}
bool Yut::randT()
{
	srand(time(NULL));
	int temp;
	if (playing) {
		temp = rand() % 2;
		if (temp == 0)
		{
			this->turn = true; printf("플레이어1이 먼저 시작합니다.");
		}
		else if (temp == 1) {
			this->turn = false; printf("플레이어2가 먼저 시작합니다.");
		}
	}
	return true;
}
bool Yut::bonusT() //쓸모 없을듯?
{
	return true;
}
bool Yut::catchs(int num) //잡은것인지 아닌지 체크
{
	for (int i = 0; i < 4; i++) {
		if (player1[i] == num) //업은 것일 경우
			return false;
		else if (player2[i] == num) { //잡은 경우
			player2[i] = 0;
			return true;
		}
		else
			return false;
	}
	return false;
}
bool Yut::goal()
{
	for (int i = 0; i < 4; i++) {
		if (player1[i] > 20) {
			player1[i] = -1;
			return true;
		}
		if (player2[i] > 20) {
			player2[i] = -1;
			return true;
		}
	}
	return false;
}
bool Yut::win()
{
	int temp= 0;
	for (int i = 0; i < 4; i++)
		temp -= player1[i];
	if (temp == -4)
		printf("승리하였습니다!");
	for (int i = 0; i < 4; i++) 
		temp -= player2[i];
	if (temp == -4)
		printf("패배하였습니다!");
	return false;
}
void Yut::showPlayer()
{
	printf("움직일 말을 고르십시오. (출발하지 않은 말:");
	for (int i = 0; i < 4; i++)
	{
		if (player1[i] == 0)
			printf("%d / ", i + 1);
	}
	printf(")\n");
	printf("(보드 위에 있는 말 : ");
	for (int i = 0; i < 4; i++)
	{
		if (player1[i] > 0 && player1[i] < 21)
			printf("%d = %d번 위치 / ", i + 1, player1[i]);
	}
	printf(")\n");
}


int main()
{

	Yut yut;
	while (1)
	{
		int player = 0;
		
		yut.roll();
		yut.showPlayer();
		scanf("%d", &player);

		printf("\n");
		yut.move(player);
	}
}



