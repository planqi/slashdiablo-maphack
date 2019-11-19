#pragma once
#include <list>
#include <Windows.h>
#include <string>

namespace Drawing {
	// HookGroups allow use of the basic hooks(Line,Text,Box,Frame)
	//	in more advanced hooks(Input,UI,Button) that require
	//	multiple basic hooks.
	class Hook;
	class HookGroup {
		public:
			std::list<Hook*> Hooks;
			virtual unsigned int GetX() = 0;
			virtual unsigned int GetY() = 0;
			virtual unsigned int GetXSize() = 0;
			virtual unsigned int GetYSize() = 0;
			virtual bool IsActive() = 0;
	};

	enum HookVisibility {InGame,OutOfGame,Automap,Perm,Group};
	enum BoxTrans {BTThreeFourths, BTOneHalf, BTOneFourth, BTWhite, BTBlack, BTNormal, BTScreen, BTHighlight, BTFull};
	enum {None=0, Center=1, Right=2, Top=4};


	typedef std::list<Hook*> HookList;
	typedef std::list<Hook*>::iterator HookIterator;
	typedef bool (__cdecl *OnClick)(bool,Hook*,void*);

	class Hook {
		private:
			static HookList Hooks;//Holds a list of every basic hook used.
			HookVisibility visibility;//When we should show the hook.
			unsigned int x, y, z;//Hooks screen coordinates and the z-order.
			CRITICAL_SECTION crit;//Critical Section so we don't have race conditions.
			bool active;//Boolean to hold if we should draw the hook or not.
			int alignment;//Holds what type of alignment(if any) we should use.
			HookGroup* group;//Holds the group this hook is associated with.
			OnClick left;//Click callback handler for left clicking
			void* leftVoid;//Holds data to give back to the callback for things like knowing your proper class.
			OnClick right;//Click callback handler for right clicking
			void* rightVoid;//Holds data to give back to the callback for things like knowing your proper class.
		public:
			//Two Hook Initializations; one for basic hooks, one for grouped hooks.
			Hook(HookVisibility visibility, unsigned int x, unsigned int y);
			Hook(HookGroup* group, unsigned int x, unsigned int y);
			//~Hook();

			//Critical Section Helpers.
			void Lock();
			void Unlock();

			//Returns the x position of where the hook will be drawn.
			unsigned int GetX();

			//Returns the base x position not calculating in groups or alignment.
			unsigned int GetBaseX();

			//Sets the base x position, or offset from groups x position.
			void SetBaseX(unsigned int xPos);

			//Returns the width of the hook, determine by super-class.
			virtual unsigned int GetXSize() = 0;


			//Returns the y position of where the hook will be drawn.
			unsigned int GetY();

			//Returns the base y position not calculating in groups or alignment.
			unsigned int GetBaseY();

			//Sets the base y position, or offset from groups y position.
			void SetBaseY(unsigned int yPos);

			//Returns the height of the hook, determine by super-class.
			virtual unsigned int GetYSize() = 0;


			//Returns when the hook will be drawn compared to other hooks.
			int GetZOrder();

			//Sets when the hook will be drawn compared to the other hooks.
			void SetZOrder(int zPos);


			//Returns when the hook will be visible
			HookVisibility GetVisibility();

			//Sets when the hook will be visible
			void SetVisibility(HookVisibility newVisibility);


			//Returns if we are drawing the hook currently.
			bool IsActive();

			//Sets if we should be drawing the hook.
			void SetActive(bool newActive);


			//Returns how we are going to align the hook.
			int GetAlignment();

			//Sets how we are to align the hook.
			void SetAlignment(int newAlign);

			
			//Returns the hook's group.
			HookGroup* GetGroup();

			//Sets the hook's group.
			void SetGroup(HookGroup* newGroup);

			//Returns the callback handler for left clicks
			OnClick GetLeftClickHandler();

			//Return the callback void handler for left clicks
			void* GetLeftClickVoid();

			//Set the callback for left clicks
			void SetLeftCallback(OnClick leftHandler, void* voidVar);


			//Returns the callback handler for right clicks
			OnClick GetRightClickHandler();

			//Return the callback void handler for right clicks
			void* GetRightClickVoid();

			//Set the callback for right clicks
			void SetRightCallback(OnClick rightHandler, void* voidVar);

			//Determine if the given x/y set is within the hooks drawing area.
			bool InRange(unsigned int x, unsigned int y);

			//This is the function in super-class we actually draw the function.
			virtual void OnDraw() = 0;

			//Function gets called when someone clicks, return true to block the click.
			virtual bool OnLeftClick(bool up, unsigned int x, unsigned int y) { return false; };
			virtual bool OnRightClick(bool up, unsigned int x, unsigned int y) { return false; };

			//Function gets called when someone types, return true to block the input.
			virtual bool OnKey(bool up, BYTE key, LPARAM lParam) { return false; };


			//Static function to draw all the hooks with the given visibility.
			static void Draw(HookVisibility type);

			//Static function to check if we interacted with any hooks.
			static bool LeftClick(bool up, unsigned int x, unsigned int y);
			static bool RightClick(bool up, unsigned int x, unsigned int y);
			static bool KeyClick(bool bUp, BYTE bKey, LPARAM lParam);

			//Misc Hook Functions needed
			static unsigned int GetScreenHeight();
			static unsigned int GetScreenWidth();
			static void ScreenToAutomap(POINT* ptPos, int x, int y);
			static void AutomapToScreen(POINT* ptPos, int x, int y);

	};
};