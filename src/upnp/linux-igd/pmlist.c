/******************************************************************************
*  Copyright (c) 2002 Linux UPnP Internet Gateway Device Project              *    
*  All rights reserved.                                                       *
*                                                                             *   
*  This file is part of The Linux UPnP Internet Gateway Device (IGD).         *
*                                                                             *
*  The Linux UPnP IGD is free software; you can redistribute it and/or modify *
*  it under the terms of the GNU General Public License as published by       *
*  the Free Software Foundation; either version 2 of the License, or          *
*  (at your option) any later version.                                        *
*                                                                             *    
*  The Linux UPnP IGD is distributed in the hope that it will be useful,      *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of             *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
*  GNU General Public License for more details.                               *
*                                                                             *   
*  You should have received a copy of the GNU General Public License          * 
*  along with Foobar; if not, write to the Free Software                      *
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  *
*                                                                             *  
*                                                                             *  
******************************************************************************/

#include "pmlist.h"
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void PortMapList_Construct(struct PortMapList *pmlist)
{
}

void PortMapList_Destruct(struct PortMapList *pmlist)
{
	struct PortMap *itr;

	while(pmlist->m_pmap){
                PortMap_delPortForward(pmlist->m_pmap->m_PortMappingProtocol, pmlist->m_pmap->m_ExternalIP,
                                pmlist->m_pmap->m_ExternalPort, pmlist->m_pmap->m_InternalClient,
                                pmlist->m_pmap->m_InternalPort);
		itr=pmlist->m_pmap;
		NODE_DEL(pmlist->m_pmap, itr);
		PortMap_Destruct(itr);
		free(itr);
	}
}

int PortMapAdd(struct PortMapList *pmlist, char *RemoteHost, char *Proto, char *ExtIP, int ExtPort,
		char *IntIP, int IntPort, int Enabled, char *Desc, int LeaseDuration)
{

	int fd_socket, fd_proto;
	struct sockaddr_in addr;
	struct PortMap *temp;
	
	if ((strcmp(Proto,"TCP") == 0) || (strcmp(Proto,"tcp") == 0))
		fd_proto = SOCK_STREAM;
	else
		fd_proto = SOCK_DGRAM;
	
	if ((fd_socket = socket(AF_INET,fd_proto, 0)) == -1)
		printf("Socket Error\n");
//		syslog(LOG_DEBUG, "Socket Error");
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(ExtPort);
	if (bind(fd_socket, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		close(fd_socket);
//		syslog(LOG_DEBUG,"Error binding socket");
		return (718);
	}
	close (fd_socket);
	
//	temp = new PortMap;
	temp = (struct PortMap*)malloc(sizeof(struct PortMap));
	PortMap_Construct(temp);
	
	if (RemoteHost)
        {
		temp->m_RemoteHost=strdup(RemoteHost);
//                temp->m_RemoteHost = new char[strlen(RemoteHost)+1];
//                strcpy(temp->m_RemoteHost, RemoteHost);
        }
        else temp->m_RemoteHost = NULL;

        if (Proto)
	{
		temp->m_PortMappingProtocol=strdup(Proto);
//		temp->m_PortMappingProtocol = new char[strlen(Proto)+1];
//		strcpy(temp->m_PortMappingProtocol, Proto);
	}
	else temp->m_PortMappingProtocol = NULL;

        if (ExtIP)
        {
		temp->m_ExternalIP=strdup(ExtIP);
//                temp->m_ExternalIP = new char[strlen(ExtIP)+1];
//                strcpy(temp->m_ExternalIP, ExtIP);
        }
        else temp->m_ExternalIP = NULL;

        temp->m_ExternalPort = ExtPort;

        if (IntIP)
        {
		temp->m_InternalClient=strdup(IntIP);
//                temp->m_InternalClient = new char[strlen(IntIP)+1];
//                strcpy(temp->m_InternalClient, IntIP);
        }
        else temp->m_InternalClient = NULL;

        temp->m_InternalPort = IntPort;

        if (Desc)
        {
		temp->m_PortMappingDescription=strdup(Desc);
//                temp->m_PortMappingDescription = new char[strlen(Desc)+1];
//                strcpy(temp->m_PortMappingDescription, Desc);
        }
        else temp->m_PortMappingDescription = NULL;

        temp->m_PortMappingLeaseDuration = LeaseDuration;

//	m_pmap.push_back(temp);
	NODE_ENQUEUE(pmlist->m_pmap, temp);
	
	PortMap_addPortForward(Proto, ExtIP, ExtPort, IntIP, IntPort, Enabled, Desc);
	
	return (1);
}


int PortMapDelete(struct PortMapList *pmlist, char *Proto, int ExtPort)
{
	struct PortMap *itr;
	
//	for (list<PortMap *>::iterator itr = m_pmap.begin(); itr != m_pmap.end(); itr++)
	/* Change this to use a double link list */
	for (itr = pmlist->m_pmap; itr; itr=itr->next)
	{
		if (( strcmp(itr->m_PortMappingProtocol,Proto) == 0 ) && (itr->m_ExternalPort == ExtPort))
		{
			PortMap_delPortForward(itr->m_PortMappingProtocol, itr->m_ExternalIP,
					itr->m_ExternalPort, itr->m_InternalClient,
					itr->m_InternalPort);
			
//			delete itr;
//			m_pmap.erase(itr);
			NODE_DEL(pmlist->m_pmap, itr);
			PortMap_Destruct(itr);
			free(itr);
			return (1);
		}
	}
	return 0;
}


int PortMap_addPortForward(char *Proto, char *ExtIP, int ExtPort, 
	char *IntIP,int IntPort, int Enabled, char *Desc)
{
	char command[255];
	char *errstr;
	int retval;

	sprintf(command, IPTABLES " -t nat -A PREROUTING -p %s -d %s --dport %d -j DNAT --to %s:%d", Proto, ExtIP, ExtPort, IntIP, IntPort);
//syslog(LOG_ERR,"add port forward |%s|", command);
	retval = system(command);
	if (retval) {
		errstr = strerror(retval);
//		syslog(LOG_DEBUG, "Problem with cmd: %s: %s\n", command, errstr);
	}
	sprintf(command, IPTABLES " -I FORWARD -p %s -d %s --dport %d -j ACCEPT", Proto, IntIP, IntPort);
//syslog(LOG_ERR,"cont             |%s|", command);
//	system(command);
	retval = system(command);
	if (retval) {
		errstr = strerror(retval);
//		syslog(LOG_DEBUG, "Problem with cmd: %s: %s\n", command, errstr);
	}

	return (1);
}

int PortMap_delPortForward(char *Proto, char *ExtIP, int ExtPort, 
	char* IntIP, int IntPort)
{
	char command[255];
	char *errstr;
	int retval;
	

	sprintf(command, IPTABLES  " -t nat -D PREROUTING -p %s -d %s --dport %d -j DNAT --to %s:%d", Proto, ExtIP, ExtPort, IntIP, IntPort);
//syslog(LOG_ERR,"del port forward |%s|", command);
	retval = system(command);	
	if (retval) {
		errstr = strerror(retval);
//		syslog(LOG_DEBUG, "Problem with cmd: %s: %s\n", command, errstr);
	}
	sprintf(command, IPTABLES  " -D FORWARD -p %s -d %s --dport %d -j ACCEPT", Proto, IntIP, IntPort);
//syslog(LOG_ERR,"cont             |%s|", command);
	retval = system(command);	
	if (retval) {
		errstr = strerror(retval);
//		syslog(LOG_DEBUG, "Problem with cmd: %s: %s\n", command, errstr);
	}
	return (1);
}

