#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

//#define _WITH_SERVER_CONNECT



using namespace std;



#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512


#pragma pack(push,1)
struct Player_world {

	XMFLOAT4X4 player_world;

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

};
#pragma pack(pop)

#pragma pack(push,1)
struct AttackAndDefend {

	bool leftHand = false;
	bool rightHand = false;
	bool foot = false;

	bool leftGuard = false;
	bool rightGuard = false;
	bool middleGuard = false;

	bool checkAni = false;

};
#pragma pack(pop)

class Player;

class Scene;

class Object;


// hierarchicalGameObjects.data()[1].head->GetPosition();
// hierarchicalGameObjects.data()[1].head->SetPosition(XMFLOAT3());
//Object* head	// ¸Ó¸®
//Object* rHand	// ¿À¸¥¼Õ
//Object* lHand	// ¿Þ¼Õ
//Object* lFoot	// ¿Þ¹ß
//Object* rFoot	// ¿À¸¥¹ß
//Object* spine	// Ã´Ãß Áß½É

class Server
{

public:
	Player* cplayer;
	Scene* cscene;
	Object* other_object;

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

