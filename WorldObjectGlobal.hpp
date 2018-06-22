#pragma once
#ifndef GUILD_WORLDOBJECTGLOBAL_HPP
#define GUILD_WORLDOBJECTGLOBAL_HPP

/* WorldObjectGlobal.hpp
	#include "WorldObjectGlobal.hpp"

	Project: WorldSim
	Created: Unknown.
	Updated: 7/10/2017, 0271994297.

	Description:
	Interface for objects which have a presence on the world map.

	Notes:

	0271994297 - Updated basic documentation.
*/

#include <Interface/HasTexture.hpp>

class World;

class WorldObjectGlobal: public HasTexture
{
	private:
	
	std::string name;
	
	public:
	int worldX, worldY; /* Coordinates on the world map. */
	World* world; /* Link with the world. */
	
	WorldObjectGlobal();
	
	
	enum enumType { NOTHING=0, URBAN=1, ARMY=2, FLEET=3, NOMADIC_TRIBE=4 };
	int type;
	
	//std::string name;
	
		/* RETURN THE NAME OF THE OBJECT. EXAMPLES:
			CITY - ADELAIDE.
			ARMY - IV LEGION.
		*/
	virtual std::string getName() { return "?"; }
	
		/* RETURN MULTI-LINE INFO. TBD. */  
	virtual std::string getExtendedInfo() { return "?"; }
	
		// RETURN THE NUMBER OF TILES AWAY FROM THE OBJECT. THIS IS IN 'MOVES', INCLUDING DIAGONALS.
	int distanceTo (WorldObjectGlobal* _obj)
	{
		int xChange = worldX - _obj->worldX;
		int yChange = worldY - _obj->worldY;
		
		if ( xChange < 0 )
		{ xChange*=-1; }
		if ( yChange < 0 )
		{ yChange*=-1; }
	
		return (xChange+yChange);
	}
	
	virtual ~WorldObjectGlobal()
	{
	}
	
	
	// TextureInterface::
	//virtual Texture* currentTexture() { return 0; }
		
};

#endif