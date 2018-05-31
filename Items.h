#pragma once
#include "sdk.h"

#define SIZEOF(arg) (sizeof(arg) / 4)

static const char* SelectionMode[] =
{
	"Closest",
	"Distance",
	"Crosshair",
	"Velocity",
	"Health"
};

static const char* BuyBot[] =
{
	"Disabled",
	"Rifles",
	"Scout",
	"Auto"
};

static const char* HitboxMode[] =
{
	"Head",
	"Neck",
	"Chest",
	"Stomach",
	"Pelvis",
	"All"
};

static const char* HitscanMode[] =
{
	"Disabled",
	"Low",
	"Medium",
	"High",
	"Custom"
};

static const char* MultipointMode[] =
{
	"Disabled",
	"Normal",
	"Full"
};

static const char* FakelagMovement[] =
{
	"On ground",
	"On jump"
};

static const char* FakelagMode[] =
{
	"Maximum",
	"Adaptive",
};

static const char* HeightCompensation[] =
{
	"Spread",
	"No-spread"
};

static const char* PreferBodyaim[] =
{
	"Disabled",
	"Deadly",
	"Lethal",
	"Always"
};

static const char* AntiaimbotPitch[] =
{
	"Disabled",
	"Down", 
	"Up",
	"Zero",
	"Jitter",
	"AntiBacktrack",
	"Fake-Down(!)",
	"Fake-Up(!)"
};

static const char* Skyboxmode[] =
{
	"Off",
	"Dawn",
	"Himalaya",
	"Mountains",
	"Jungle",
	"City",
	"Daylight",
	"Night",
	"Storm"
};

static const char* AntiaimbotYaw[] =
{
	"Disabled", "Static", "Jitter", "180 spin", "180z", "Spin", "Lowerbody"
};

static const char* AntiaimbotYawRun[] =
{
	"Disabled", "Static", "Jitter", "180 spin", "180z", "Spin", "Lowerbody"
};

static const char* AntiaimbotYawFake[] =
{
	"Disabled", "Static", "Opposite", "Jitter", "Spin", "180z", "Sideways", "Lowerbody", "180 Treehouse", "Senpaii",
};

static const char* Freestanding[] =
{
	"Disabled",
	"Auto direction",
	"Keybased",
	"Keybased Jitter"
	//"Freestand"
};

static const char* AtTarget[] =
{
	"Disabled",
	"Enabled"
	//"Average"
};

static const char* ModelsMode[] =
{
	"Disabled",
	"Flat",
	"Default"
};

static const char* glow[] =
{
	"Disabled",
	"1",
	"2",
	"3",
	"4"
};

static const char* Fakeghost[] =
{
	"Disabled",
	"Fake",
	"Lowerbody"
};


static const char* ThirdpersonAngles[] =
{
	"Fake",
	"Real",
	"Lowerbody"
};

static const char* Clantag[] =
{
	"Disabled",
	"aimware.net",
	"gamesense",
	"skeet.cc",
	"komihook",
	"Luminus"
};

static const char* DroppedWeapons[] =
{
	"Disabled",
	"Enabled"
	//"Glow"
};

static const char* Hitmarkerwavs[] =
{
	"Disabled",
	"normal",
	"bolbi",
	"metallic",
	"blob",
	"thats a one",
	"penguware",
};

static const char* aimware_color[] =
{
	"Red",
	"White",
};


static const char* Configs[] =
{
	"Auto",
	"Scout",
	"No Spread",
	"Legit"
};
