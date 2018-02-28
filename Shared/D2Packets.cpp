#include "D2Packets.hpp"

#ifdef D2EXE
#include "../Game/Diablo2.hpp"
#elif defined(D2CLIENT)
#include "../Client/D2Client.hpp"
#elif defined(D2GAME)
#include "../Server/D2Game.hpp"
#else
#include "../Common/D2Common.hpp"
#endif

/*
 *	Read a packet from the client, on the server.
 *	@author	eezstreet
 */
size_t D2Packet::ReadServer(char* buffer, size_t bufferSize)
{
	size_t dwAmount = 1;
	if (!bufferSize)
	{
		return 0;
	}

	nPacketType = buffer[0];
	switch (nPacketType)
	{
		case D2CPACKET_QUESTDATA:
		case D2CPACKET_RESURRECT:
		case D2CPACKET_BUSYOFF:
		case D2CPACKET_STAMINAON:
		case D2CPACKET_STAMINAOFF:
		case D2CPACKET_SWAPWEAPONS:
		case D2CPACKET_LEAVEGAME:
		case D2CPACKET_JOINGAME:
			break; // sizeof = 1
	}

	return dwAmount;
}

/*
 *	Read a packet from the server, on the client.
 *	@author	eezstreet
 */
size_t D2Packet::ReadClient(char* buffer, size_t bufferSize)
{
	size_t dwAmount = 1;
	if (!bufferSize)
	{
		return 0;
	}

	nPacketType = buffer[0];
	switch (nPacketType)
	{
		case D2SPACKET_GAMELOADING:
		case D2SPACKET_LOADSUCCESS:
		case D2SPACKET_LOADCOMPLETE:
		case D2SPACKET_UNLOADCOMPLETE:
		case D2SPACKET_GAMEXIT:
		case D2SPACKET_STARTMERCLIST:
		case D2SPACKET_UNKNOWN6E:
		case D2SPACKET_UNKNOWN6F:
		case D2SPACKET_UNKNOWN70:
		case D2SPACKET_UNKNOWN71:
		case D2SPACKET_UNKNOWN72:
		case D2SPACKET_SWITCHWEAPONS:
		case D2SPACKET_CONNECTIONTERM:
			break;
		case D2SPACKET_COMPRESSIONINFO:
			packetData.ServerCompressionInfo.nCompressionType = buffer[dwAmount++];
			break;
		case D2SPACKET_SAVESTATUS:
			packetData.ServerSaveStatus.nSaveStatus = buffer[dwAmount++];
			break;
	}
	
	return dwAmount;
}

/*
 *	Write a packet to the client.
 *	@author	eezstreet
 */
size_t D2Packet::WriteServer(char* buffer, size_t bufferSize)
{
	size_t dwAmount = 1;
	if (!bufferSize)
	{
		return 0;
	}

	buffer[0] = nPacketType;
	switch (nPacketType)
	{
		case D2SPACKET_GAMELOADING:
		case D2SPACKET_LOADSUCCESS:
		case D2SPACKET_LOADCOMPLETE:
		case D2SPACKET_UNLOADCOMPLETE:
		case D2SPACKET_GAMEXIT:
		case D2SPACKET_STARTMERCLIST:
		case D2SPACKET_UNKNOWN6E:
		case D2SPACKET_UNKNOWN6F:
		case D2SPACKET_UNKNOWN70:
		case D2SPACKET_UNKNOWN71:
		case D2SPACKET_UNKNOWN72:
		case D2SPACKET_SWITCHWEAPONS:
		case D2SPACKET_CONNECTIONTERM:
			break;
		case D2SPACKET_COMPRESSIONINFO:
			buffer[dwAmount++] = packetData.ServerCompressionInfo.nCompressionType;
			break;
		case D2SPACKET_SAVESTATUS:
			buffer[dwAmount++] = packetData.ServerSaveStatus.nSaveStatus;
			break;
	}

	return dwAmount;
}

/*
 *	Write a packet to the server.
 *	@author	eezstreet
 */
const BYTE __joinName_Constant[16] =
{
	0x00, 0x00, 0x00, 0xFA,
	0xD9, 0xE2, 0xA3, 0x86,
	0x03, 0xB2, 0x01, 0x0C,
	0x02, 0x00, 0x00, 0x70,
};

size_t D2Packet::WriteClient(char* buffer, size_t bufferSize)
{
	size_t dwPacketDataSize = 0;
	size_t dwAmount = 1;
	if (!bufferSize)
	{
		return 0;
	}

	buffer[0] = nPacketType;
	switch (nPacketType)
	{
	case D2CPACKET_QUESTDATA:
	case D2CPACKET_RESURRECT:
	case D2CPACKET_BUSYOFF:
	case D2CPACKET_STAMINAON:
	case D2CPACKET_STAMINAOFF:
	case D2CPACKET_SWAPWEAPONS:
	case D2CPACKET_LEAVEGAME:
	case D2CPACKET_JOINGAME:
		break; // sizeof = 1
	case D2CPACKET_JOINLOCAL:
		memcpy(buffer + dwAmount, packetData.ClientLocalJoinRequest.szGameName, 16);
		dwAmount += 16;
		buffer[dwAmount++] = packetData.ClientLocalJoinRequest.nGameMode;
		buffer[dwAmount++] = packetData.ClientLocalJoinRequest.nClass;
		buffer[dwAmount++] = packetData.ClientLocalJoinRequest.nTemplate;
		buffer[dwAmount++] = packetData.ClientLocalJoinRequest.nDifficulty;
		memcpy(buffer + dwAmount, packetData.ClientLocalJoinRequest.szCharName, 16);
		dwAmount += 16;
		*(WORD*)(buffer + dwAmount) = packetData.ClientLocalJoinRequest.nUnknown1;
		dwAmount += 2;
		*(DWORD*)(buffer + dwAmount) = 0x00100004;
		dwAmount += 4;
		buffer[dwAmount++] = packetData.ClientLocalJoinRequest.nUnknown3;
		buffer[dwAmount++] = packetData.ClientLocalJoinRequest.nUnknown4;
		buffer[dwAmount++] = packetData.ClientLocalJoinRequest.nLocale;
		break;
	case D2CPACKET_JOINREMOTE:
		*(DWORD*)(buffer + dwAmount) = packetData.ClientRemoteJoinRequest.unk1;
		dwAmount += 4;
		*(WORD*)(buffer + dwAmount) = packetData.ClientRemoteJoinRequest.unk2;
		dwAmount += 2;
		buffer[dwAmount++] = packetData.ClientRemoteJoinRequest.unk3;
		*(DWORD*)(buffer + dwAmount) = packetData.ClientRemoteJoinRequest.dwVersion;
		dwAmount += 4;
		buffer[dwAmount++] = packetData.ClientRemoteJoinRequest.nLocale;
		D2Lib::strncpyz(buffer + dwAmount, packetData.ClientRemoteJoinRequest.szCharName, 16);
		dwAmount += 16;
		break;
	}

	return dwAmount;
}