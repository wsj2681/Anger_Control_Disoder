#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS




using namespace std;



#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512


#pragma pack(push,1)
struct Player_world {

	XMFLOAT4X4 player_world;

};
#pragma pack(pop)

class CPlayer;



class Sever
{

public:
	CPlayer* cplayer;

	SOCKET sock;
	int retval = 0;
	char Save_Data[BUFSIZE];

	Player_world player;


	int send_count = 0;
	int recv_count = 0;

	Sever();
	~Sever();

	void Sever_send();

	void Sever_recv();
	
	

};

