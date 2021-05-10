#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

//#define _WITH_SERVER_CONNECT



using namespace std;



#define SERVERIP   "192.168.123.98"
#define SERVERPORT 9000
#define BUFSIZE    512


#pragma pack(push,1)
struct Player_world {

	XMFLOAT4X4 player_world;


	XMFLOAT4X4 player_Head;
	XMFLOAT4X4 player_rHand;
	XMFLOAT4X4 player_lHand;
	XMFLOAT4X4 player_rFoot;
	XMFLOAT4X4 player_lFoot;
	XMFLOAT4X4 player_Spine;

	UINT nowState = STATE_IDLE;
	
};
#pragma pack(pop)

#pragma pack(push,1)
struct Thread_id {

	int thread_num;

};
#pragma pack(pop)

#pragma pack(push,1)
struct collide {

	bool check_collide = false;

	bool rHand2rHand = false;
	bool rHand2lHand = false;
	bool rHand2Spine = false;
	bool rHand2Head = false;

	bool lHand2rHand = false;
	bool lHand2lHand = false;
	bool lHand2Spine = false;
	bool lHand2Head = false;

	bool rFoot2Spine = false;
	bool rFoot2Head = false;

	bool lFoot2Spine = false;
	bool lFoot2Head = false;

	/*bool headHitted = false;
	bool spineHitted = false;*/

	XMFLOAT3 collidePosition = XMFLOAT3(0.0f, 0.0f, 0.0f);			//충돌이 나는 곳의 좌표
};
#pragma pack(pop)

#pragma pack(push,1)
struct AttackAndDefend {

	bool leftHand = false;
	bool rightHand = false;
	bool jap = false;

	bool hitTorsoLeft = false;
	bool hitTorsoRight = false;
	bool hitTorsoStright = false;

	bool leftGuard = false;
	bool rightGuard = false;
	bool middleGuard = false;

	bool nuckDown = false;

	bool checkAni = false;
	

};
#pragma pack(pop)

#pragma pack(push,1)
struct HeadHitted {

	bool leftHeadHitted = false;
	bool rightHeadHitted = false;
	bool straightHtitted = false;

};
#pragma pack(pop)


#pragma pack(push,1)
struct PlayerHP {

	float playerHp;

};
#pragma pack(pop)
class Player;

class Scene;

class Object;


// hierarchicalGameObjects.data()[1].head->GetPosition();
// hierarchicalGameObjects.data()[1].head->SetPosition(XMFLOAT3());
//Object* head	// 머리
//Object* rHand	// 오른손
//Object* lHand	// 왼손
//Object* lFoot	// 왼발
//Object* rFoot	// 오른발
//Object* spine	// 척추 중심

class Server
{

public:
	Player* cplayer;
	Scene* cscene;
	Object* cobject;

	SOCKET sock;
	int retval = 0;
	char Save_Data[BUFSIZE];

	Player_world player;
	Player_world other_player;
	Player_world save_world;

	XMFLOAT3 player_position;
	XMFLOAT3 player_right;
	XMFLOAT3 player_up;
	XMFLOAT3 player_look;

	Thread_id thread_id;
	collide col;

	AttackAndDefend send_attackAnddefend;
	AttackAndDefend recv_attackAnddefend;
	HeadHitted headHitted;

	PlayerHP myHP;


	bool bScenario{ false };

	int send_count = 0;
	int recv_count = 0;

	Server();
	~Server();

	void Server_send();

	void Server_recv();

	void attackAndGuard_idle();

	void otherPlayerPositionSet();



};