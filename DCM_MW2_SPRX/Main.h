#include <wchar.h>
#include <sys/prx.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/ppu_thread.h>
#include <string.h>
#include <cstring>
#include <sys/sys_time.h>
#include <sys/time_util.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/process.h>
#include <sys/memory.h>
#include <sys/timer.h>
#include <sys/return_code.h>
#include <sys/prx.h>
#include <stddef.h>
#include <math.h>
#include <cmath>
#include <stdarg.h>
#include <cellstatus.h>
#include <typeinfo>
#include <vector>
#include <pthread.h>
#include <locale.h>
#include <cell/error.h>
#include <sys/paths.h>
#include <time.h>

//   -----------------------------   //
//		  INLUDED HEADER FILES       //
		#include "Button.h"
		#include "Function.h"
		#include "Hud.h"
		#include "PS3.h"
		#include "Verification.h"
//   -----------------------------   //

#define TOC ( 0x00724C38 )

sys_ppu_thread_t MainThread;
sys_ppu_thread_t thrd_Calls;

sys_ppu_thread_t create_thread(void (*entry)(uint64_t), int priority, size_t stacksize, const char* threadname,sys_ppu_thread_t id)
{	
	if(sys_ppu_thread_create(&id, entry, 0, priority , stacksize, 0, threadname) == CELL_OK)
	{	
	}
		
    return id;
}


bool ModLoaded;
bool MenuOpened[18];
int Scroll[18];
int MaxScroll[18];
const char* weapon;
char* CurrentMenu[18];
char* ParentMenu[18];
char* setMenuTitle[18];
const char* Players[18];
int SelectedClient;

huds::gameHudelem_s* BG[18];
huds::gameHudelem_s* MenuTitle[18];
huds::gameHudelem_s* MenuText[18][99];
huds::gameHudelem_s* Scrollbar[18];
huds::gameHudelem_s* InfoBox[18];
huds::gameHudelem_s* InfoText1[18];
huds::gameHudelem_s* InfoText2[18];
huds::gameHudelem_s* InfoText3[18];
huds::gameHudelem_s* InfoText4[18];

void KillClient(int client)
	{
		if(SelectedClient != host)
		{
		*(char*)(0x014E2220 + 0x3700 * client) = 0xC5;
		_toc::SV_GameSendServerCommand(SelectedClient, 1, "f \"The Host ^7[^1Killed You^7]\"");
		}
		else
			_toc::SV_GameSendServerCommand(client, 1, "f \"^1Can't Fuck With the Host\"");
	}
	void KickPlayer(int client)
	{
		if(SelectedClient != host)
		{
			_toc::ClientDisconnect(SelectedClient);
		}
		else
			_toc::SV_GameSendServerCommand(client, 1, "f \"^1Can't Fuck With the Host\"");
	}

void MenuHuds(int client)
{
	BG[client] = huds::setShader(client, "white", 200, 600, 400, 0, 0, 0, 0, 0, 150);
	Scrollbar[client] = huds::setShader(client, "white", 200, 16.5, 400, 50, 0, 0, 0, 0, 165);
	MenuTitle[client] = huds::setText(client, "", 4, 2, 500, 150, 5, 255, 255, 255, 0, 17, 255, 255, 255);
	InfoBox[client] = huds::setShader(client, "white", 190, 75, 150, 270, 0, 0, 0, 0, 0);
	InfoText1[client] = huds::setText(client, "PlautGaming's Black Ice SPRX!", 3, 1, 245, 650, 5, 255, 255, 255, 0, 17, 255, 255, 255);
	InfoText2[client] = huds::setText(client, "[{+actionslot 1}] / [{+actionslot 2}] to Scroll", 3, 1, 245, 680, 5, 255, 255, 255, 0);
	InfoText3[client] = huds::setText(client, "[{+gostand}] to Select Option", 3, 1, 245, 710, 5, 255, 255, 255, 0);
	InfoText4[client] = huds::setText(client, "[{+usereload}] to go Back / Exit Menu", 3, 1, 245, 740, 5, 255, 255, 255, 0);
	for(int x = 1; x < 21; x++)
		MenuText[client][x] = huds::setText(client, "", 4, 3, 500, 170 + (x * 50), 5, 255, 255, 255, 255, 0, 0, 0, 0);
}

void Scrolling(int client)
{
	for(int x = 1; x < 21; x++)
	{
		MenuText[client][x]->elem.fontScale = .9;
		MenuText[client][x]->elem.color.r = 255;
		MenuText[client][x]->elem.color.g = 255;
		MenuText[client][x]->elem.color.b = 255;
		MenuText[client][x]->elem.glowColor.a = 0;
	}
	float pos = 70.085 + (21.92 * Scroll[client]);
	huds::moveOverTime(Scrollbar[client], 0.20, 400, pos);
	huds::glowText(MenuText[client][Scroll[client]], 0.20, 17, 255, 255, 255);
	huds::scaleOverTimeText(MenuText[client][Scroll[client]], 0.20, 1.4);
}

void Close(int client)
{
	huds::fadeOverTime(BG[client], 0.50, 0, 0, 0, 0);
	huds::fadeOverTime(Scrollbar[client], 0.50, 0, 0, 0, 0);
	huds::fadeOverTime(InfoBox[client], 0.50, 0, 0, 0, 0);
	huds::fadeOverTime(InfoText1[client], 0.50, 0, 0, 0, 0);
	huds::fadeOverTime(InfoText2[client], 0.50, 0, 0, 0, 0);
	huds::fadeOverTime(InfoText3[client], 0.50, 0, 0, 0, 0);
	huds::fadeOverTime(InfoText4[client], 0.50, 0, 0, 0, 0);
	huds::fadeOverTime(MenuTitle[client], 0.50, 0, 0, 0, 0);
	for(int x = 1; x < 21; x++)
		huds::fadeOverTime(MenuText[client][x], 0.50, 0, 0, 0, 0);
}
void Open(int client)
{
	huds::fadeOverTime(BG[client], 0.50, 0, 0, 0, 140);
	huds::fadeOverTime(Scrollbar[client], 0.50, 0, 0, 0, 170);
	huds::fadeOverTime(InfoBox[client], 0.50, 0, 0, 0, 140);
	huds::fadeOverTime(InfoText1[client], 0.50, 255, 255, 255, 255);
	huds::fadeOverTime(InfoText2[client], 0.50, 255, 255, 255, 255);
	huds::fadeOverTime(InfoText3[client], 0.50, 255, 255, 255, 255);
	huds::fadeOverTime(InfoText4[client], 0.50, 255, 255, 255, 255);
	huds::fadeOverTime(MenuTitle[client], 0.50, 255, 255, 255, 255);
	for(int x = 1; x < 21; x++)
			huds::fadeOverTime(MenuText[client][x], 0.50, 255, 255, 255, 255);
}
void setMenuList(int client)
{
	if(CurrentMenu[client] == "main")
	{
		Scroll[client] = 1;
		MaxScroll[client] = 8;
		huds::ChangeText(MenuTitle[client], "Main Menu");
		huds::ChangeText(MenuText[client][1], "Main Mods");
		huds::ChangeText(MenuText[client][2], "Fun Menu");
		huds::ChangeText(MenuText[client][3], "Lobby Menu");
		huds::ChangeText(MenuText[client][4], "Message Menu");
		huds::ChangeText(MenuText[client][5], "All Players");
		huds::ChangeText(MenuText[client][6], "Weapons");
		huds::ChangeText(MenuText[client][7], "Account");
		huds::ChangeText(MenuText[client][8], "Players Menu");
	}
	else if(CurrentMenu[client] == "Main Mods")
	{
		if(AccessLevel[client] == Host, Admin)
		{
		Scroll[client] = 1;
		MaxScroll[client] = 8;
		huds::ChangeText(MenuTitle[client], "Main Mods");
		huds::ChangeText(MenuText[client][1], "God Mode");
		huds::ChangeText(MenuText[client][2], "No Clip");
		huds::ChangeText(MenuText[client][3], "Infinite Ammo");
		huds::ChangeText(MenuText[client][4], "Little Crosshair");
		huds::ChangeText(MenuText[client][5], "No Recoil");
		huds::ChangeText(MenuText[client][6], "Red Boxes");
		huds::ChangeText(MenuText[client][7], "Explosive Bullets");
		huds::ChangeText(MenuText[client][8], "Aim Assist");
		huds::ChangeText(MenuText[client][9], "");
		}
		else if(AccessLevel[client] == Vip, Verified)
		{
			_toc::SV_GameSendServerCommand(client, 1, "f \"^1You Need a Higher Access Level\"");
		}
	}
	else if(CurrentMenu[client] == "Fun Menu")
	{
		if(AccessLevel[client] == Host, Admin, Vip, Verified)
		{
		Scroll[client] = 1;
		MaxScroll[client] = 5;
		huds::ChangeText(MenuTitle[client], "Fun Menu");
		huds::ChangeText(MenuText[client][1], "Double Run Speed");
		huds::ChangeText(MenuText[client][2], "FOV");
		huds::ChangeText(MenuText[client][3], "Pro Skater");
		huds::ChangeText(MenuText[client][4], "Third Person");
		huds::ChangeText(MenuText[client][5], "Left Hand Gun");
		huds::ChangeText(MenuText[client][6], "");
		huds::ChangeText(MenuText[client][7], "");
		huds::ChangeText(MenuText[client][8], "");
		huds::ChangeText(MenuText[client][9], "");
		}
	}
	else if(CurrentMenu[client] == "Lobby Menu")
	{
		if(AccessLevel[client] == Host)
		{
		Scroll[client] = 1;
		MaxScroll[client] = 7;
		huds::ChangeText(MenuTitle[client], "Lobby Menu");
		huds::ChangeText(MenuText[client][1], "High Jump");
		huds::ChangeText(MenuText[client][2], "Super Speed");
		huds::ChangeText(MenuText[client][3], "Force Host");
		huds::ChangeText(MenuText[client][4], "Super Force Host");
		huds::ChangeText(MenuText[client][5], "Gravity");
		huds::ChangeText(MenuText[client][6], "TimeScale");
		huds::ChangeText(MenuText[client][7], "Super Melee");
		huds::ChangeText(MenuText[client][8], "");
		huds::ChangeText(MenuText[client][9], "");
		}
		else if(AccessLevel[client] == Admin, Vip, Verified)
		{
			_toc::SV_GameSendServerCommand(client, 1, "f \"^1Only the Host Can Access This Menu\"");
		}
	}
	else if(CurrentMenu[client] == "Message Menu")
	{
		if(AccessLevel[client] == Host, Admin, Vip, Verified)
		{
		Scroll[client] = 1;
		MaxScroll[client] = 4;
		huds::ChangeText(MenuTitle[client], "Message Menu");
		huds::ChangeText(MenuText[client][1], "Creators");
		huds::ChangeText(MenuText[client][2], "Fuck You Bitch");
		huds::ChangeText(MenuText[client][3], "TrickShot Last");
		huds::ChangeText(MenuText[client][4], "OMG that's Legitness!");
		huds::ChangeText(MenuText[client][5], "");
		huds::ChangeText(MenuText[client][6], "");
		huds::ChangeText(MenuText[client][7], "");
		huds::ChangeText(MenuText[client][8], "");
		huds::ChangeText(MenuText[client][9], "");
		}
	}
	else if(CurrentMenu[client] == "All Players")
	{
		if(AccessLevel[client] == Host)
		{
		Scroll[client] = 1;
		MaxScroll[client] = 6;
		huds::ChangeText(MenuTitle[client], "All Players");
		huds::ChangeText(MenuText[client][1], "GodMode");
		huds::ChangeText(MenuText[client][2], "No Clip");
		huds::ChangeText(MenuText[client][3], "Red Boxes");
		huds::ChangeText(MenuText[client][4], "Third Person");
		huds::ChangeText(MenuText[client][5], "FOV");
		huds::ChangeText(MenuText[client][6], "Infinite Ammo");
		huds::ChangeText(MenuText[client][7], "");
		huds::ChangeText(MenuText[client][8], "");
		huds::ChangeText(MenuText[client][9], "");
		huds::ChangeText(MenuText[client][10], "");
		}
		else if(AccessLevel[client] == Admin, Vip, Verified)
		{
			_toc::SV_GameSendServerCommand(client, 1, "f \"^1Only the Host Can Access This Menu\"");
		}
	}
	else if(CurrentMenu[client] == "Weapons")
	{
		if(AccessLevel[client] == Host, Admin, Vip)
		{
		Scroll[client] = 1;
		MaxScroll[client] = 5;
		huds::ChangeText(MenuTitle[client], "Weapons");
		huds::ChangeText(MenuText[client][1], "AC130");
		huds::ChangeText(MenuText[client][2], "Intervention");
		huds::ChangeText(MenuText[client][3], "Barrett");
		huds::ChangeText(MenuText[client][4], "Gold Deagle");
		huds::ChangeText(MenuText[client][5], "Default Weapon");
		huds::ChangeText(MenuText[client][6], "");
		huds::ChangeText(MenuText[client][7], "");
		huds::ChangeText(MenuText[client][8], "");
		huds::ChangeText(MenuText[client][9], "");
		huds::ChangeText(MenuText[client][10], "");
		}
		else if(AccessLevel[client] == Verified)
		{
			_toc::SV_GameSendServerCommand(client, 1, "f \"^1You Need a Higher Access Level\"");
		}
	}
	else if(CurrentMenu[client] == "Account")
	{
		if(AccessLevel[client] == Host, Admin, Vip)
		{
		Scroll[client] = 1;
		MaxScroll[client] = 4;
		huds::ChangeText(MenuTitle[client], "Account");
		huds::ChangeText(MenuText[client][1], "Level 70");
		huds::ChangeText(MenuText[client][2], "Unlock All");
		huds::ChangeText(MenuText[client][3], "Toggle Prestige");
		huds::ChangeText(MenuText[client][4], "^5B^7lack^5I^7ce ClanTag {BI}");
		huds::ChangeText(MenuText[client][5], "");
		huds::ChangeText(MenuText[client][6], "");
		huds::ChangeText(MenuText[client][7], "");
		huds::ChangeText(MenuText[client][8], "");
		huds::ChangeText(MenuText[client][9], "");
		huds::ChangeText(MenuText[client][10], "");
		}
		else if(AccessLevel[client] == Verified)
		{
			_toc::SV_GameSendServerCommand(client, 1, "f \"^1You Need a Higher Access Level\"");
		}
	}
	else if(CurrentMenu[client] == "Players Menu")
	{
		if(AccessLevel[client] == Host)
		{
		Scroll[client] = 1;
		huds::ChangeText(MenuTitle[client], "Players Menu");
		int j = 1;
		for( int i = 0; i < 18; i++)
		{
			Players[i] = (char*)0x014E5408 + (i * 0x3700);
			if(strcmp(Players[i], ""))
			{
					char dest[0x50];
					_toc::stdc_snprintf(dest, sizeof(dest), "[%s]%s", getAccessLevelStr(i), Players[i]);
					huds::ChangeText(MenuText[client][j], dest);
					MaxScroll[client] = j;
					j++;
			}
			for (int k = j; k < 21; k++)
			{
				huds::ChangeText(MenuText[client][k], "");
			}
		}
		}
		else if(AccessLevel[client] == Admin, Vip, Verified)
		{
			_toc::SV_GameSendServerCommand(client, 1, "f \"^1Only the Host Can Access This Menu\"");
		}
	}
	else if(CurrentMenu[client] == "client")
	{
		Scroll[client] = 1;
		MaxScroll[client] = 8;
		char temp[100];
		_toc::stdc_snprintf(temp, sizeof(temp), "Do What to %s?", (char*)0x014E5408 + (SelectedClient * 0x3700));
		huds::ChangeText(MenuTitle[client], temp);
		huds::ChangeText(MenuText[client][1], "Suicide");
		huds::ChangeText(MenuText[client][2], "Kick");
		huds::ChangeText(MenuText[client][3], "Remove Access");
		huds::ChangeText(MenuText[client][4], "Give Verified");
		huds::ChangeText(MenuText[client][5], "Give VIP");
		huds::ChangeText(MenuText[client][6], "Give Admin");
		huds::ChangeText(MenuText[client][7], "Level 70");
		huds::ChangeText(MenuText[client][8], "Unlock All");
		huds::ChangeText(MenuText[client][9], "");
		huds::ChangeText(MenuText[client][10], "");
		huds::ChangeText(MenuText[client][11], "");
		huds::ChangeText(MenuText[client][12], "");
		huds::ChangeText(MenuText[client][13], "");
		huds::ChangeText(MenuText[client][14], "");
		huds::ChangeText(MenuText[client][15], "");
		huds::ChangeText(MenuText[client][16], "");
		huds::ChangeText(MenuText[client][17], "");
		huds::ChangeText(MenuText[client][18], "");
		huds::ChangeText(MenuText[client][19], "");
		huds::ChangeText(MenuText[client][20], "");
	}
}

void loadMenu(int client, char* menu, char* parent)
{
	CurrentMenu[client] = menu;
	ParentMenu[client] = parent;
	setMenuList(client);
	Scrolling(client);
}

void setAccess(int Owner, int Target, Verification AccessLevel)
{
	if (isHost(Target))
			_toc::iPrintlnBold(Owner, "^2You cannot change the Accesslevel of the Host!");
	else
	{
			if (isAllowed(Target))
					Close(Target);
			AccessLevel = AccessLevel;
			char temp[0x50];
			_toc::stdc_snprintf(temp, sizeof(temp), "^2You have changed %s Accesslevel to: %s", getClientName(Target), AccessLevel);
			_toc::iPrintln(Owner, temp);
			_toc::stdc_snprintf(temp, sizeof(temp), "^2Your Accesslevel has been changed to %s by %s", AccessLevel, getClientName(Owner));
	}
}

void removeAccess(int Target, Verification Accesslevel)
{
	AccessLevel[Target] = Accesslevel;
}

void getMenuFuncList(int client)
{
	if(CurrentMenu[client] == "main")
	{
		if(Scroll[client]==1)
		{
			loadMenu(client, "Main Mods", "main");
		}
		else if(Scroll[client]==2)
		{
			loadMenu(client, "Fun Menu", "main");
		}
		else if(Scroll[client]==3)
		{
			loadMenu(client, "Lobby Menu", "main");
		}
		else if(Scroll[client]==4)
		{
			loadMenu(client, "Message Menu", "main");
		}
		else if(Scroll[client]==5)
		{
			loadMenu(client, "All Players", "main");
		}
		else if(Scroll[client]==6)
		{
			loadMenu(client, "Weapons", "main");
		}
		else if(Scroll[client]==7)
		{
			loadMenu(client, "Account", "main");
		}
		else if(Scroll[client]==8)
		{
			loadMenu(client, "Players Menu", "main");
		}
	}
	else if(CurrentMenu[client] == "Main Mods")
	{
		if(Scroll[client]==1)
		{
			GodMode(client);
		}
		else if(Scroll[client]==2)
		{
			NoClip(client);
		}
		else if(Scroll[client]==3)
		{
			UnlimitedAmmo(client);
		}
		else if(Scroll[client]==4)
		{
			LittleCrosshair(client);
		}
		else if(Scroll[client]==5)
		{
			NoRecoil(client);
		}
		else if(Scroll[client]==6)
		{
			RedBoxes(client);
		}
		else if(Scroll[client]==7)
		{
			ExplosiveBullets(client);
		}
		else if(Scroll[client]==8)
		{
			ToggleAim(client);
		}
	}
	else if(CurrentMenu[client] == "Fun Menu")
	{
		if(Scroll[client]==1)
		{
			DoubleRunSpeed(client);
		}
		else if(Scroll[client]==2)
		{
			FOV(client);
		}
		else if(Scroll[client]==3)
		{
			SkateMod(client);
		}
		else if(Scroll[client]==4)
		{
			ThirdPerson(client);
		}
		else if(Scroll[client]==5)
		{
			LeftGun(client);
		}
	}
	else if(CurrentMenu[client] == "Lobby Menu")
	{
		if(Scroll[client]==1)
		{
			SuperJump(client);
		}
		else if(Scroll[client]==2)
		{
			SuperSpeed(client);
		}
		else if(Scroll[client]==3)
		{
			ForceHost(client);
		}
		else if(Scroll[client]==4)
		{
			SuperForceHost(client);
		}
		else if(Scroll[client]==5)
		{
			Gravity(client);
		}
		else if(Scroll[client]==6)
		{
			Timescale(client);
		}
		else if(Scroll[client]==7)
		{
			SuperMelee(client);
		}
	}
	else if(CurrentMenu[client] == "Message Menu")
	{
		if(Scroll[client]==1)
		{
			Creators(0);
			Creators(1);
			Creators(2);
			Creators(3);
			Creators(4);
			Creators(5);
			Creators(6);
			Creators(7);
			Creators(8);
			Creators(9);
			Creators(10);
			Creators(11);
			Creators(12);
			Creators(13);
			Creators(14);
			Creators(15);
			Creators(16);
			Creators(17);
		}
		else if(Scroll[client]==2)
		{
			FuckYou(0);
			FuckYou(1);
			FuckYou(2);
			FuckYou(3);
			FuckYou(4);
			FuckYou(5);
			FuckYou(6);
			FuckYou(7);
			FuckYou(8);
			FuckYou(9);
			FuckYou(10);
			FuckYou(11);
			FuckYou(12);
			FuckYou(13);
			FuckYou(14);
			FuckYou(15);
			FuckYou(16);
			FuckYou(17);
		}
		else if(Scroll[client]==3)
		{
			TrickShot(0);
			TrickShot(1);
			TrickShot(2);
			TrickShot(3);
			TrickShot(4);
			TrickShot(5);
			TrickShot(6);
			TrickShot(7);
			TrickShot(8);
			TrickShot(9);
			TrickShot(10);
			TrickShot(11);
			TrickShot(12);
			TrickShot(13);
			TrickShot(14);
			TrickShot(15);
			TrickShot(16);
			TrickShot(17);
		}
		else if(Scroll[client]==4)
		{
			Legitness(0);
			Legitness(1);
			Legitness(2);
			Legitness(3);
			Legitness(4);
			Legitness(5);
			Legitness(6);
			Legitness(7);
			Legitness(8);
			Legitness(9);
			Legitness(10);
			Legitness(11);
			Legitness(12);
			Legitness(13);
			Legitness(14);
			Legitness(15);
			Legitness(16);
			Legitness(17);
		}
	}
	else if(CurrentMenu[client] == "All Players")
	{
		if(Scroll[client]==1)
		{
			GodMode(0);
			GodMode(1);
			GodMode(2);
			GodMode(3);
			GodMode(4);
			GodMode(5);
			GodMode(6);
			GodMode(7);
			GodMode(8);
			GodMode(9);
			GodMode(10);
			GodMode(11);
			GodMode(12);
			GodMode(13);
			GodMode(14);
			GodMode(15);
			GodMode(16);
			GodMode(17);
		}
		else if(Scroll[client]==2)
		{
			NoClip(0);
			NoClip(1);
			NoClip(2);
			NoClip(3);
			NoClip(4);
			NoClip(5);
			NoClip(6);
			NoClip(7);
			NoClip(8);
			NoClip(9);
			NoClip(10);
			NoClip(11);
			NoClip(12);
			NoClip(13);
			NoClip(14);
			NoClip(15);
			NoClip(16);
			NoClip(17);
		}
		else if(Scroll[client]==3)
		{
			RedBoxes(0);
			RedBoxes(1);
			RedBoxes(2);
			RedBoxes(3);
			RedBoxes(4);
			RedBoxes(5);
			RedBoxes(6);
			RedBoxes(7);
			RedBoxes(8);
			RedBoxes(9);
			RedBoxes(10);
			RedBoxes(11);
			RedBoxes(12);
			RedBoxes(13);
			RedBoxes(14);
			RedBoxes(15);
			RedBoxes(16);
			RedBoxes(17);
		}
		else if(Scroll[client]==4)
		{
			ThirdPerson(0);
			ThirdPerson(1);
			ThirdPerson(2);
			ThirdPerson(3);
			ThirdPerson(4);
			ThirdPerson(5);
			ThirdPerson(6);
			ThirdPerson(7);
			ThirdPerson(8);
			ThirdPerson(9);
			ThirdPerson(10);
			ThirdPerson(11);
			ThirdPerson(12);
			ThirdPerson(13);
			ThirdPerson(14);
			ThirdPerson(15);
			ThirdPerson(16);
			ThirdPerson(17);
		}
		else if(Scroll[client]==5)
		{
			FOV(0);
			FOV(1);
			FOV(2);
			FOV(3);
			FOV(4);
			FOV(5);
			FOV(6);
			FOV(7);
			FOV(8);
			FOV(9);
			FOV(10);
			FOV(11);
			FOV(12);
			FOV(13);
			FOV(14);
			FOV(15);
			FOV(16);
			FOV(17);
		}
		else if(Scroll[client]==6)
		{
			UnlimitedAmmo(0);
			UnlimitedAmmo(1);
			UnlimitedAmmo(2);
			UnlimitedAmmo(3);
			UnlimitedAmmo(4);
			UnlimitedAmmo(5);
			UnlimitedAmmo(6);
			UnlimitedAmmo(7);
			UnlimitedAmmo(8);
			UnlimitedAmmo(9);
			UnlimitedAmmo(10);
			UnlimitedAmmo(11);
			UnlimitedAmmo(12);
			UnlimitedAmmo(13);
			UnlimitedAmmo(14);
			UnlimitedAmmo(15);
			UnlimitedAmmo(16);
			UnlimitedAmmo(17);
		}
	}
	else if(CurrentMenu[client] == "Weapons")
	{
		if(Scroll[client]==1)
		{
			GiveAC130(client);
		}
		else if(Scroll[client]==2)
		{
			GiveI(client);
		}
		else if(Scroll[client]==3)
		{
			GiveB(client);
		}
		else if(Scroll[client]==4)
		{
			GiveG(client);
		}
		else if(Scroll[client]==5)
		{
			GiveD(client);
		}
	}
	else if(CurrentMenu[client] == "Account")
	{
		if(Scroll[client]==1)
		{
			level(client);
		}
		else if(Scroll[client]==2)
		{
			UnlockAll(client);
		}
		else if(Scroll[client]==3)
		{
			Prestige(client);
		}
		else if(Scroll[client]==4)
		{
			IceClan(client);
		}
	}
	else if(CurrentMenu[client] == "Players Menu")
	{
		if(Scroll[client]==1)
		{
			SelectedClient = 0;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==2)
		{
			SelectedClient = 1;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==3)
		{
			SelectedClient = 2;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==4)
		{
			SelectedClient = 3;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==5)
		{
			SelectedClient = 4;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==6)
		{
			SelectedClient = 5;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==7)
		{
			SelectedClient = 6;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==8)
		{
			SelectedClient = 7;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==9)
		{
			SelectedClient = 8;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==10)
		{
			SelectedClient = 9;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==11)
		{
			SelectedClient = 10;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==12)
		{
			SelectedClient = 11;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==13)
		{
			SelectedClient = 12;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==14)
		{
			SelectedClient = 13;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==15)
		{
			SelectedClient = 14;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==16)
		{
			SelectedClient = 15;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==17)
		{
			SelectedClient = 16;
			loadMenu(client, "client", "players");
		}
		else if(Scroll[client]==18)
		{
			SelectedClient = 17;
			loadMenu(client, "client", "players");
		}
	}
	else if(CurrentMenu[client] == "client")
	{
		if(Scroll[client]==1)
		{
			KillClient(SelectedClient);
		}
		else if(Scroll[client]==2)
		{
			KickPlayer(SelectedClient);
		}
		else if(Scroll[client]==3)
		{
			removeAccess(SelectedClient, None);
			KillClient(SelectedClient);
		}
		else if(Scroll[client]==4)
		{
			setAccess(client, SelectedClient, Verified);
			KillClient(SelectedClient);
		}
		else if(Scroll[client]==5)
		{
			setAccess(client, SelectedClient, Vip);
			KillClient(SelectedClient);
		}
		else if(Scroll[client]==6)
		{
			setAccess(client, SelectedClient, Admin);
			KillClient(SelectedClient);
		}
		else if(Scroll[client]==7)
		{
			level(SelectedClient);
		}
		else if(Scroll[client]==8)
		{
			UnlockAll(SelectedClient);
		}
	}
}

//======================================//
//                                      //
//                                      //
//=============MAIN=THREAD==============//
//                                      //
//                                      //
//======================================//



void onPlayerSpawned(int clientIndex)
{
	sleep(1500);
	char buf[100];
	_toc::stdc_snprintf(buf, 100, "Welcome [ %s ]", (char*)0x014E5408 + (clientIndex * 0x3700));
	huds::SetMessage(clientIndex, buf, "PlautGaming's Black Ice v1 SPRX!", 2048, 50, 7000, 1500, 255, 255, 255, 255, 17, 255, 255, 255);
	_toc::SV_GameSendServerCommand(clientIndex, 0, "o \"mp_level_up\"");

	char dest[100];
	_toc::stdc_snprintf(dest, 100, "^5PlautGaming's Black Ice v1 SPRX! Press [{+actionslot 1}] For Menu");
	_toc::iPrintln(clientIndex, dest);
}

void Menu_Thread(std::uint64_t args)
{
	for(;;)
	{
		for(int i = 0; i < 18; i++)
		{
			if(_toc::Dvar_GetBool("cl_ingame") == 1)
			{
				if(!ModLoaded)
				{
					*(int*)0x131689C = 1;//to use precached shaders
                    AccessLevel[i] = isHost(i) ? Host : None;
					if(isAllowed(i))
					{
						_toc::SV_GameSendServerCommand(i, 1, "v loc_warnings \"0\"");
						_toc::SV_GameSendServerCommand(i, 1, "v loc_warningsAsErrors \"0\"");
						onPlayerSpawned(i);
						MenuHuds(i);
						ModLoaded = true;
					}
				}
				else
				{
					if(!MenuOpened[i])
					{
						if(ButtonPressed(i, "+actionslot 1"))
						{
							sleep(200);
							Open(i);
							loadMenu(i,"main","Exit");
							MenuOpened[i] = true;
						}
					}
					else
					{
						if(ButtonPressed(i, "+actionslot 1"))
						{
							sleep(120);
							Scroll[i] --;
							if (Scroll[i] < 1)
							{
									Scroll[i] = MaxScroll[i];
							}
							Scrolling(i);
						}
						else if(ButtonPressed(i, "+actionslot 2"))
						{
							sleep(120);
							Scroll[i] ++;
							if (Scroll[i] > MaxScroll[i])
							{
									Scroll[i] = 1;
							}
							Scrolling(i);
						}
						else if(ButtonPressed(i, "+gostand"))
						{
							sleep(150);
							getMenuFuncList(i);
							Scrolling(i);
						}
						else if(ButtonPressed(i, "+usereload"))
						{
							sleep(150);
							if(ParentMenu[i]=="Exit")
							{
								Close(i);
								MenuOpened[i] = false;
							}
							else if(CurrentMenu[i]=="players")
							{
								loadMenu(i, "main", "Exit");
							}
							else
							{
								loadMenu(i,ParentMenu[i],"Exit");
							}
						}
					}
				}
				if(!isAlive(i) && isAllowed(i))
				{
					sys_timer_sleep(1);
					onPlayerSpawned(i);
				}
			}
			else
			{
				huds::Destroy;
				ModLoaded = false;
				removeAccess(i, None);
			}
		}
	}
}