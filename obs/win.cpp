#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib,"Ws2_32.lib")
#include <ws2tcpip.h>
#define port 1024

//	Device Group
#define grp_dev 1

#include <obs-frontend-api.h>

HANDLE sigExit=NULL;

void worker(){
	sockaddr_in protocol;
	protocol.sin_family = AF_INET;
	protocol.sin_addr.s_addr = INADDR_BROADCAST;
	protocol.sin_port = htons(port);
	SOCKET sock = socket(protocol.sin_family, SOCK_DGRAM, IPPROTO_UDP);
	if (sock != INVALID_SOCKET) {
		char tmp = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &tmp, sizeof(tmp)) == 0) {
			unsigned char msg[2];
			msg[0] = grp_dev;
			do{
				
				//	Are we streaming and/or recording?
				msg[1] = 1 + ( obs_frontend_recording_active() || obs_frontend_streaming_active() );

				if (sendto(sock, (char*)msg, sizeof(msg) / sizeof(unsigned char), 0, (SOCKADDR*)&protocol, sizeof(protocol)) == SOCKET_ERROR) 
					break;
				
			} while (WaitForSingleObject(sigExit, 0x400) == WAIT_TIMEOUT);
		}
	closesocket(sock);}


}

HANDLE thread=NULL;

#include <obs-module.h>
OBS_DECLARE_MODULE()

bool obs_module_load() {
	if((sigExit=CreateEvent(NULL,false,false,NULL))!=NULL)
		if((thread=CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)worker,NULL,NULL,NULL))!=NULL)
			return true;
return false;}

void obs_module_unload() {
	if(sigExit!=NULL) SetEvent(sigExit);
	if (thread != NULL) {
		CloseHandle(thread);
		WaitForSingleObject(thread, INFINITE);
	}
	if(sigExit!=NULL) CloseHandle(sigExit);
}
