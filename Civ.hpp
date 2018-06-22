#pragma once
#ifndef GUILD_CIV_HPP
#define GUILD_CIV_HPP

class Character;
class World;

#include "GuildCalendar.hpp"


	// CIVMANAGER STORES DATA SHARED ACROSS ALL CIVS.

class Civ;
	
class CivManager
{
	public:
	ArrayS2 <Civ*> aOwnerShip;
};
CivManager civManager;

class Civ
{
	private:
		Vector <std::string> familyNamePool;
		

		
		
		//GuildCalendar* calendar; /* Use world pointer. */
		

		

	public:
	
		//static int s_nIDGenerator;
		
		
			// EACH TILE MAY ONLY BE OWNED BY ONE CIV, THEREFORE THIS ARRAY IS SHARED BETWEEN ALL CIVS.
		static ArrayS2 <Civ*> aOwnership;
	
			/* The civ is at war with civs in this vector. */
		Vector <Civ*> vAtWar;
			/* The civ can only contact civs in this vector. */
		Vector <Civ*> vAware;
	
			// TRUE IF A CIV CAN SEE THIS TILE, FALSE OTHERWISE.
		ArrayS2 <bool> aVisible;
		
			// EACH TILE MAY ONLY BE OWNED BY ONE CIV, THEREFORE THIS ARRAY IS SHARED BETWEEN ALL CIVS.
		//static ArrayS2 <Civ*> aOwnership;
	
		/* ABSTRACT TEST STUFF */
			int nFood;
		
	
		Vector <Citizen*> vCitizen;
		Vector <City*> vCity;
		
		Vector <Army*> vArmy;
		Vector <Navy*> vNavy;
	
		std::string name;
		int money;
		World* world;
		int nSoldiers;
		
		int colourRed;
		int colourGreen;
		int colourBlue;
		
		Civ();
		
			// MAKE THE CIV AWARE OF THE EXISTENCE OF THE PASSED CIV.
		void addContact(Civ*);
		
		void manageArmy();
		
			/* LINK THE CIV TO A WORLD. ALSO LOAD UP CITIZEN FAMILIES. */
		void init(World*);
		void collectTaxes();
		void logicTick();
		
		
			// Place a city near the civ, in an 'optimal' location.
		City* foundCity();
		
		void setColour (const int,const int,const int);
		
			/* Returns false if the spawn failed. */
		bool spawn();
		
		
			/* SIMULATE X TURNS OF THE CIV. */
		void incrementTicks ( int /* nTicks */ );
	
			/* UPDATE FOG OF WAR ARRAY, AND ALSO CHECK FOR NEW CONTACTS. */
		void updateFogOfWar();
			// RETURN THE NUMBER OF NEIGHBORING FOG TILES. (0-8).
		int neighboringFog ( int _x, int _y );
			// RETURN THE NEIGHBORING TILE WITH THE MOST FOG. IF ALL OR SOME TILES ARE EQUALLY FOGGY, RETURN A RANDOM ONE OF THE FOGGIEST ONES.
		void foggiestNeighbor (int _x, int _y, int* _rx, int* _ry, bool _landOnly);
		
			// Set values to a tile which is a priority for exploration.
		void getExploreTile(int* _rx, int* _ry, bool _land);
		
		int getPopulation();
		
		void garbageCollect();
		
			/* DEPRECATED */
		void aiDay(); /* Simulate Civ for one day of game world time. */
		void aiSecond(); /* Simulate Civ for one second of game world time. */
		
			// RETURN TRUE IF THE CIV HAS CONTACT WITH THIS CIV.
		bool knowsAbout(Civ*);
			// RETURN TRUE IF THE CIV IS AT WAR WITH THE PASSED CIV.
		bool atWarWith(Civ*);
		
			// RETURN TRUE IF THE TILE IS NOT FOGGY.
		bool canSee(const int _x, const int _y);
	
};

//int Civ::s_nIDGenerator = 1;

	// DECLARE STATIC STUFFS.
ArrayS2 <Civ*> Civ::aOwnership;

//Civ::aOwnership;

//static ArrayS2 <Civ*> aOwnership;

#endif