#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "stdafx.h"
#include "Server.h"

#include "Object.h"
#include "Player.h"
#include "Scene.h"

Server::Server()
{

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "WSAStartup Error" << endl;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN sever_sock_addr;

	::ZeroMemory(&sever_sock_addr, sizeof(SOCKADDR_IN));
	sever_sock_addr.sin_family = AF_INET;
	sever_sock_addr.sin_addr.S_un.S_addr = inet_addr(SERVERIP);
	sever_sock_addr.sin_port = ntohs(SERVERPORT);


	retval = connect(sock, (sockaddr*)&sever_sock_addr, sizeof(sever_sock_addr));


}

void Server::Server_send()
{
	//준비완료 보내기
	if (send_count == 0) {
		retval = send(sock, (char*)"Ready", sizeof("Ready"), 0);
		cout << "보내기완료" << endl;

		++send_count;
	}
	else {
		retval = send(sock, (char*)&id, sizeof(id), 0);

		player.player_world = cplayer->m_xmf4x4World;

		retval = send(sock, (char*)&player, sizeof(player), 0);


	}
}

void Server::Server_recv()
{
	//준비완료 받기
	if (recv_count == 0) {
		retval = recv(sock, (char*)Save_Data, sizeof BUFSIZE, 0);
		cout << Save_Data << "받기완료" << endl;
		retval = recv(sock, (char*)&id, sizeof(id), 0);

		++recv_count;
	}
	else {
		retval = recv(sock, (char*)&other_player, sizeof(other_player), 0);
		//cout << player.player_world._41 << " " << player.player_world._42 << " " << player.player_world._43 << endl;

		//save_world.player_world =  cscene->m_ppHierarchicalGameObjects[0]->m_xmf4x4World;
		
		player_position.x =  other_player.player_world._41;
		player_position.y =  other_player.player_world._42;
		player_position.z =  other_player.player_world._43;

		cscene->m_ppHierarchicalGameObjects[0]->SetPosition(player_position.x, player_position.y, player_position.z); 



		/*cplayer->m_xmf4x4World = player.player_world;

		player_position.x = player.player_world._41;
		player_position.y = player.player_world._42;
		player_position.z = player.player_world._43;

		cplayer->SetPosition(player_position);*/


		/*cplayer.m_xm

		m_xmf4x4Transform._11 = m_xmf3Right.x;
		m_xmf4x4Transform._12 = m_xmf3Right.y;
		m_xmf4x4Transform._13 = m_xmf3Right.z;

		m_xmf4x4Transform._21 = m_xmf3Up.x;
		m_xmf4x4Transform._22 = m_xmf3Up.y;
		m_xmf4x4Transform._23 = m_xmf3Up.z;

		m_xmf4x4Transform._31 = m_xmf3Look.x;
		m_xmf4x4Transform._32 = m_xmf3Look.y;
		m_xmf4x4Transform._33 = m_xmf3Look.z;

		m_xmf4x4Transform._41 = m_xmf3Position.x;
		m_xmf4x4Transform._42 = m_xmf3Position.y;
		m_xmf4x4Transform._43 = m_xmf3Position.z;*/




	}


}

