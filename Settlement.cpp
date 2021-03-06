#pragma once
#ifndef WORLDSIM_SETTLEMENT_CPP
#define WORLDSIM_SETTLEMENT_CPP

#include "Settlement.hpp"

Settlement::Settlement()
{
  
  name = "Settlement";
  world = 0;
  colourRed = 0;
  colourGreen = 0;
  colourBlue = 0;
  
  race = NONE;
  
}

Texture* Settlement::currentTexture()
{
	return &TEX_TEST_404;
}

  /* SIMULATE X TURNS OF THE SETTLEMENT. */
void Settlement::incrementTicks ( int /* nTicks */ )
{
  

  return;
}


/* TABLE INTERFACE */

std::string Settlement::getColumn(std::string _column)
{
	if ( _column=="name" )
	{
		return name;
	}
	if ( _column=="race" )
	{
		if (race == NONE)
    {
      return "None";
    }
		if (race == HUMAN)
    {
      return "Human";
    }
		if (race == DWARVEN)
    {
      return "Dwarven";
    }
		if (race == ELVEN)
    {
      return "Elven";
    }
    
	}
  
  if ( _column == "metal" )
  {
    return DataTools::toString(nMetalStockpile);
  }
	if ( _column=="food" )
	{
		//return DataTools::toString(nFood);
	}
	if ( _column=="population" )
	{
		return DataTools::toString(vCharacter.size());
	}
	if ( _column=="coordinates" )
	{
		//return DataTools::toString(worldX)+","+DataTools::toString(worldY);
	}
	if ( _column=="territory" )
	{
		//return DataTools::toString(vTerritory.size());
	}
	// else if ( _column=="id" )
	// {
		// return DataTools::toString(id);
	// }
	
	return "?";
}
std::string Settlement::getColumnType(std::string _column)
{
	if ( _column == "population" || _column == "territory" || _column == "food" || _column == "metal" )
	{
		return "number";
	}
	return "string";
}


#endif
  