#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "stdafx.h"
#include "Server.h"

#include "Object.h"
#include "Player.h"
#include "Scene.h"
#include "AnimationController.h"
#include "AnimationSet.h"
#include "AnimationTrack.h"


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
	bool optval = true;

	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));

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
		UINT movingAniNum = 0;

		movingAniNum =  cplayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0);		
	

		if (movingAniNum == ANIMATION_MOVE_BACKWARD) {
			SendPlayerMove.back = true;
		}
		else if (movingAniNum == ANIMATION_MOVE_FORWARD) {
			SendPlayerMove.Front = true;
		}
		else if (movingAniNum == ANIMATION_MOVE_LEFT) {
			SendPlayerMove.Left = true;
		}
		else if (movingAniNum == ANIMATION_MOVE_RIGHT) {
			SendPlayerMove.Right = true;
		}


		retval = send(sock, (char*)&player, sizeof(player), 0);

		retval = send(sock, (char*)&send_attackAnddefend, sizeof(send_attackAnddefend), 0);

		retval = send(sock, (char*)&myHP, sizeof(myHP), 0);

		retval = send(sock, (char*)&double_check, sizeof(double_check), 0);
		
		retval = send(sock, (char*)&SendPlayerMove, sizeof(SendPlayerMove), 0);

		//retval = send(sock, (char*)&readyAndstrat, sizeof(readyAndstrat), 0);
		

		//if(send_attackAnddefend.checkAni == true)
			attackAndGuard_idle();
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
		oldPlayerPosition = other_player.player_world;
		positionRecv = true;
		//retval = recv(sock, (char*)&col, sizeof(col), 0);
		retval = recv(sock, (char*)&recv_attackAnddefend, sizeof(recv_attackAnddefend), 0);
		//retval = recv(sock, (char*)&headHitted, sizeof(headHitted), 0);

		if (thread_id.thread_num == 1) {
			retval = recv(sock, (char*)&otherHP, sizeof(otherHP), 0);
		}
		retval = recv(sock, (char*)&RecvOtherPlayerMove, sizeof(RecvOtherPlayerMove), 0);

		//retval = recv(sock, (char*)&readyAndstrat, sizeof(readyAndstrat), 0);

		if (RecvOtherPlayerMove.Start == true) {
			cout << "게임시작" << endl;
			SendPlayerMove.Ready = false;
			SendPlayerMove.Start = false;
		}

		//cout << "other Player HP : "<<otherHP.playerHp << " " << endl;
		//HP설정
		//cplayer->hp = myHP.playerHp;
		cscene->hierarchicalGameObjects[1]->hp = otherHP.playerHp;

		//retval = recv(sock, (char*)&bScenario, sizeof(bScenario), 0);
		//cout << player.player_world._41 << " " << player.player_world._42 << " " << player.player_world._43 << endl;

		//save_world.player_world =  cscene->m_ppHierarchicalGameObjects[0]->m_xmf4x4World;

		


		//cout << player_position.x << " / " << player_position.y << " / " << player_position.z << endl;
		
		//cplayer->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0);

		if (RecvOtherPlayerMove.back == true) {
			if (cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_MOVE_BACKWARD) {
				cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_MOVE_BACKWARD, ANIMATION_TYPE_ONCE, true);
				cout << "백이동 실행" << endl;
			}
			
		}
		else if (RecvOtherPlayerMove.Front == true) {
			if (cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_MOVE_FORWARD) {
				cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_MOVE_FORWARD, ANIMATION_TYPE_ONCE, true);
				cout << "전진이동 실행" << endl;
			}
			
		}
		else if (RecvOtherPlayerMove.Left == true) {
			if (cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_MOVE_LEFT) {
				cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_MOVE_LEFT, ANIMATION_TYPE_ONCE, true);
				cout << "왼쪽이동 실행" << endl;
			}
			
		}
		else if (RecvOtherPlayerMove.Right == true) {
			if (cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->GetNowTrackAnimationSet(0) != ANIMATION_MOVE_RIGHT) {
				cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, ANIMATION_MOVE_RIGHT, ANIMATION_TYPE_ONCE, true);
				cout << "오른쪽이동 실행" << endl;
			}
			
		}

		otherPlayerPositionSet();

		//상대 클라 위치설정
		/*cscene->hierarchicalGameObjects[1]->SetPosition(player_position.x, player_position.y, player_position.z);
		cscene->hierarchicalGameObjects[1]->SetRight(player_right.x, player_right.y, player_right.z);
		cscene->hierarchicalGameObjects[1]->SetUp(player_up.x, player_up.y, player_up.z);
		cscene->hierarchicalGameObjects[1]->SetLook(player_look.x, player_look.y, player_look.z);*/

		//cscene->m_ppHierarchicalGameObjects[0]->nowState = other_player.nowState;

		//상대 클라 애니메이션 실행
		//otherPlayerAnimation();

		bool retVal;
		retVal = cscene->hierarchicalGameObjects[0]->m_pSkinnedAnimationController->m_pAnimationSets->m_pAnimationSets[cscene->
			hierarchicalGameObjects[0]->m_pSkinnedAnimationController->m_pAnimationTracks->m_nAnimationSet]->IsAnimate();
		
	
		/*if (recv_attackAnddefend.ani_playing == true) {
			cscene->m_ppHierarchicalGameObjects[0]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, recv_attackAnddefend.ani_num);

		}*/

			

		//cout << "ani_check - " << recv_attackAnddefend.checkAni << " " << endl;

		//cout << "ani Num - " << recv_attackAnddefend.ani_num << " - ani_check - " << recv_attackAnddefend.checkAni << endl;

		if (recv_attackAnddefend.checkAni == true) {
			if (recv_attackAnddefend.ani_num == ANIMATION_MOVE_FORWARD || recv_attackAnddefend.ani_num == ANIMATION_MOVE_BACKWARD ||
				recv_attackAnddefend.ani_num == ANIMATION_MOVE_LEFT || recv_attackAnddefend.ani_num == ANIMATION_MOVE_RIGHT) {
				if (retVal == true) {
					cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, recv_attackAnddefend.ani_num, ANIMATION_TYPE_ONCE, true);
					other_ani_check = true;
					double_check.double_check = true;
				}
			
			}
			else {
				cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, recv_attackAnddefend.ani_num, ANIMATION_TYPE_ONCE, true);
				other_ani_check = true;

				double_check.double_check = true;
			}
		
		
		}

		/*if (recv_attackAnddefend.checkAni == true) {
			cscene->hierarchicalGameObjects[1]->m_pSkinnedAnimationController->SetTrackAnimationSet(0, recv_attackAnddefend.ani_num, ANIMATION_TYPE_ONCE, true);
			
			double_check.double_check = true;
		}*/


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
	double_check.double_check = false;
	SendPlayerMove.back = false;
	SendPlayerMove.Front = false;
	SendPlayerMove.Left = false;
	SendPlayerMove.Right = false;
	//readyAndstrat.Ready = false;
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


