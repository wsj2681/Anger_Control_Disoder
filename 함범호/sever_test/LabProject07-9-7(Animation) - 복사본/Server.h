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

	short thread_id;

};
#pragma pack(pop)

class CPlayer;

class CScene;



class Server
{

public:
	CPlayer* cplayer;
	CScene* cscene;

	SOCKET sock;
	int retval = 0;
	char Save_Data[BUFSIZE];

	Player_world player;
	Player_world other_player;
	Player_world save_world;

	XMFLOAT3 player_position;

	Thread_id id;

	int send_count = 0;
	int recv_count = 0;

	Server();
	~Server();

	void Server_send();

	void Server_recv();



};

