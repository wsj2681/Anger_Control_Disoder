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
class Object;



class Server
{

public:
	CPlayer* cplayer;
	
	SOCKET sock;
	int retval = 0;
	char Save_Data[BUFSIZE];

	Player_world player;
	XMFLOAT3 player_position;


	int send_count = 0;
	int recv_count = 0;

	Server();
	~Server();

	void Server_send();

	void Server_recv();



};

