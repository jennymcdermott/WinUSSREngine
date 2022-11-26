#pragma once
/*
* This is not only to seperate out the TCP portion of the
* network handler for there may be different protocols (
* (i.e. RDP or UDP), let alone the reimplementation of
* reverse tcp. So this route was safer.
* 
* This file is just for convenience. 
* Well aware it's not technically necessary.
*/

#include "pch.h"
#include "TCPBase/FileTransferData.h"
#include "TCPBase/NetTypes.h"
#include "TCPBase/PacketType.h"
#include "TCPBase/PacketManager.h"
#include "TCPBase/PacketStructs.h"
#include "TCPBase/NetObject.h"
#include "TCPBase/Connections.h"