#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "stdafx.h"
#include "Server.h"

#include "Object.h"
#include "Player.h"
#include "Scene.h"
#include "AnimationController.h"
#include "AnimationSet.h"


Server::Server()
{


}
Server::~Server() {

}
Server::Server(int i) {
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
		retval = send(sock, (char*)&thread_id, sizeof(thread_id), 0);

		player.player_world = cplayer->m_xmf4x4World;

		/*player.player_Head = cobject->head->m_xmf4x4World;
		player.player_rHand = cobject->rHand->m_xmf4x4World;
		player.player_lHand = cobject->lHand->m_xmf4x4World;
		player.player_rFoot = cobject->rFoot->m_xmf4x4World;
		player.player_lFoot = cobject->lFoot->m_xmf4x4World;
		player.player_Spine = cobject->spine->m_xmf4x4World;*/

		//player.playerHp = cplayer->hp;
		myHP.playerHp = cplayer->hp;

		/*bool retVal{};
		retVal = cplayer->m_pSkinnedAnimationController->m_pAnimationSets->m_pAnimationSets[cplayer->m_pSkinnedAnimationController->m_pAnimationTracks->m_nAnimationSet]->IsAnimate();

		send_attackAnddefend.ani_playing = retVal;*/


		retval = send(sock, (char*)&player, sizeof(player), 0);

		retval = send(sock, (char*)&send_attackAnddefend, sizeof(send_attackAnddefend), 0);

		retval = send(sock, (char*)&myHP, sizeof(myHP), 0);

	}
}

void Server::Server_recv()
{

	//준비완료 받기
	if (recv_count == 0) {
		retval = recv(sock, (char*)Save_Data, sizeof(Save_Data), 0);
		cout << Save_Data << "받기완료" << endl;

		cout << "thread_id = " << thread_id.thread_num << endl;
		retval = recv(sock, (char*)&thread_id, sizeof(thread_id), 0);
		cout << "thread_id = " << thread_id.thread_num << endl;
		++recv_count;
	}
	else {
		retval = recv(sock, (char*)&other_player, sizeof(other_player), 0);
		//retval = recv(sock, (char*)&col, sizeof(col), 0);
		retval = recv(sock, (char*)&recv_attackAnddefend, sizeof(recv_attackAnddefend), 0);
		//retval = recv(sock, (char*)&headHitted, sizeof(headHitted), 0);
		retval = recv(sock, (char*)&otherHP, sizeof(otherHP), 0);


		//cout << "other Player HP : "<<otherHP.playerHp << " " << endl;
		//HP설정
		//cplayer->hp = myHP.playerHp;
		cscene->hierarchicalGameObjects[1]->hp = otherHP.playerHp;

		//retval = recv(sock, (char*)&bScenario, sizeof(bScenario), 0);
		//cout << player.player_world._41 << " " << player.player_world._42 << " " << player.player_world._43 << endl;

		//save_world.player_world =  cscene->m_ppHierarchicalGameObjects[0]->m_xmf4x4World;

		otherPlayerPositionSet();


		//cout << player_position.x << " / " << player_position.y << " / " << player_position.z << endl;

		//상대 클라 위치설정
		cscene->hierarchicalGameObjects[1]->SetPosition(player_position.x, player_position.y, player_position.z);
		cscene->hierarchicalGameObjects[1]->SetRight(player_right.x, player_right.y, player_right.z);
		cscene->hierarchicalGameObjects[1]->SetUp(player_up.x, player_up.y, player_up.z);
		cscene->hierarchicalGameObjects[1]->SetLook(player_look.x, player_look.y, player_look.z);

		//cscene->m_ppHierarchicalGameObjects[0]->nowState = other_player.nowState;

		//상대 클라 애니메이션 실행
		//otherPlayerAnimation();

		/*retVal = cscene->m_ppHierarchicalGameObjects[0]->m_pSkinnedAnimationController->m_pAnimationSets->m_pAnimationSets[cscene->
			m_ppHierarchicalGameObjects[0]->m_pSkinnedAnimationController->m_pAnimationTracks->m_nAnimationSet]->IsAnimate();
		*/
	
		/*if (recv_attackAnddefend.ani_playing == true) {
			cscene->m_ppHierarchicalGameObjects[0]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, recv_attackAnddefend.ani_num);

		}*/
		if (recv_attackAnddefend.checkAni == true)
			cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, recv_attackAnddefend.ani_num, ANIMATION_TYPE_ONCE, true);
		

		//충돌처리확인
		/*if (col.check_collide) {
			cout << "COLLIDE! " << endl;
			cobject->isCollide = true;

		}
		else {
			cout << "NOT COLLIDE! " << endl;
			cobject->isCollide = false;

		}*/
		/*if (col.rHand2Head) {
			cplayer->rHand->isCollide = true;
			cout << "RIGHT HAND - HEAD COLLIDE! " << endl;
			cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_LEFT_A);

		}
		else
			cplayer->rHand->isCollide = false;

		if (col.lHand2Head) {
			cplayer->lHand->isCollide = true;
			cout << "LEFT HAND - HEAD COLLIDE! " << endl;
			cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_RIGHT_A);

		}
		else
			cplayer->lHand->isCollide = false;

		if (col.rHand2rHand || col.rHand2lHand) {
			cplayer->rHand->isCollide = true;
			cout << "RIGHT HAND - Guard " << endl;
		}
		else
			cplayer->rHand->isCollide = false;

		if (col.lHand2rHand || col.lHand2lHand) {
			cplayer->lHand->isCollide = true;
			cout << "LEFT HAND - Guard! " << endl;
		}


		cplayer->isHit = false;

		if (headHitted.leftHeadHitted) {
			cplayer->isHit = true;
			cplayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_LEFT_A);
			send_attackAnddefend.hitTorsoLeft = true;


			cplayer->head->isCollide = true;
		}
		else if (headHitted.rightHeadHitted) {
			cplayer->isHit = true;
			cplayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_HIT_TORSO_RIGHT_A);
			send_attackAnddefend.hitTorsoRight = true;


			cplayer->head->isCollide = false;
		}
		else if (headHitted.straightHtitted)
		{

		}*/

		/*if (col.rHand2Spine)
			cout << "SPINE COLLIDE! " << endl;*/
			//cout << "collide _ position - " << col.collidePosition.x << " " << col.collidePosition.y << " " << col.collidePosition.z << endl;
		//if (cplayer->hp <= 0.0f) {
		//	//cplayer->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_KNOCKDOWN);
		//	cplayer->isAlive = false;
		//	send_attackAnddefend.nuckDown = true;
		//}

	}


}

void Server::Server_make_thread() {

	thread t1{ &Server::Server_thread, this };
	t1.join();
}

void Server::Server_thread() {
	while (true) {

		if (checkSR == true) {
			Server_send();
			Server_recv();

			//공격과 방어 초기화
			//attackAndGuard_idle();
			checkSR = false;
		}


	}
}


void Server::attackAndGuard_idle() {
	

	//send_attackAnddefend.ani_num = ANIMATION_IDLE_COMBAT;
	send_attackAnddefend.checkAni = false;

}

void Server::otherPlayerPositionSet()
{
	player_position.x = other_player.player_world._41;
	player_position.y = other_player.player_world._42;
	player_position.z = other_player.player_world._43;

	player_right.x = other_player.player_world._11;
	player_right.y = other_player.player_world._12;
	player_right.z = other_player.player_world._13;

	player_up.x = other_player.player_world._21;
	player_up.y = other_player.player_world._22;
	player_up.z = other_player.player_world._23;

	player_look.x = other_player.player_world._31;
	player_look.y = other_player.player_world._32;
	player_look.z = other_player.player_world._33;
}


