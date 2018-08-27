#pragma once
#ifndef WORLDSIM_WORLD_LOCAL_CPP
#define WORLDSIM_WORLD_LOCAL_CPP

/* World_Local.hpp
	#include "World_Local.cpp"

	Project: WorldSim
	Created: 10/12/2017, 0272632857.
	Updated: 10/12/2017, 0272632857.

	Description:
	The World stores local-level data, such as the position of local units.

	Notes:

	0272632857 - Added basic documentation.

*/



#include "World_Local.hpp"

World_Local::World_Local()
{
	globalX=0;
	globalY=0;
	
	nX=0;
	nY=0;
	active=false;
	generated=false;
}

void World_Local::init(int _x, int _y)
{
  globalX = _x;
  globalY = _y;
  
  aLocalTile.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
  aSubterranean.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
}

bool World_Local::generate()
{
  
  if ( world.isSafe(globalX,globalY) == false )
  { return false; }
  
  baseBiome = world.aWorldTile(globalX,globalY).biome;
  
  seed = world.aSeed(globalX,globalY);
  
  
		// GENERATE HEIGHTMAP
  DiamondSquareAlgorithmCustomRange dsa2;
	dsa2.maxValue=5;
  
  ArrayS2 <int> aLocalHeight;
  aLocalHeight.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0);
  
  // If we are generating a mountain, set a summit in the middle which is 3x3 tiles.
  if ( baseBiome == MOUNTAIN )
  {
    aLocalHeight(LOCAL_MAP_SIZE/2,LOCAL_MAP_SIZE/2) = 100;
    aLocalHeight((LOCAL_MAP_SIZE/2)+1,LOCAL_MAP_SIZE/2) = 100;
    aLocalHeight((LOCAL_MAP_SIZE/2)-1,LOCAL_MAP_SIZE/2) = 100;
    
    aLocalHeight(LOCAL_MAP_SIZE/2,(LOCAL_MAP_SIZE/2)-1) = 100;
    aLocalHeight((LOCAL_MAP_SIZE/2)+1,(LOCAL_MAP_SIZE/2)-1) = 100;
    aLocalHeight((LOCAL_MAP_SIZE/2)-1,(LOCAL_MAP_SIZE/2)-1) = 100;
    
    aLocalHeight(LOCAL_MAP_SIZE/2,(LOCAL_MAP_SIZE/2)+1) = 100;
    aLocalHeight((LOCAL_MAP_SIZE/2)+1,(LOCAL_MAP_SIZE/2)+1) = 100;
    aLocalHeight((LOCAL_MAP_SIZE/2)-1,(LOCAL_MAP_SIZE/2)+1) = 100;
    
    aLocalHeight.fillBorder(1);
    dsa2.maxValue=100;
    dsa2.generate(&aLocalHeight,0,0,0.9,10);
    
  for ( int _y=0;_y<LOCAL_MAP_SIZE;++_y)
  {
    for ( int _x=0;_x<LOCAL_MAP_SIZE;++_x)
    {
      aLocalHeight(_x,_y) =  aLocalHeight(_x,_y)/20;
    }
  }
    //aLocalHeight.setBorder(1);
  }
  else
  {
	////HEIGHTMAP TABLE FREESTEPS SMOOTHING
    dsa2.generate(&aLocalHeight,0,0,0.75,100);
  }
	
  
  // Take the seed for this world tile and expand it into a subseed for every local tile */
  random.seed(seed);
  
  for ( int _y=0;_y<LOCAL_MAP_SIZE;++_y)
  {
    for ( int _x=0;_x<LOCAL_MAP_SIZE;++_x)
    {
      aLocalTile(_x,_y).baseTerrain = baseBiome;
      aLocalTile(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
      aLocalTile(_x,_y).clearObjects();
      aLocalTile(_x,_y).height = aLocalHeight(_x,_y);
      
      
      aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
      aSubterranean(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
      aSubterranean(_x,_y).clearObjects();
      aSubterranean(_x,_y).height = -1;
      
      if (random.oneIn(100))
      {
        //put tree
        aLocalTile(_x,_y).addObject(new WorldObject_Tree);
      }
      
      else if ( baseBiome == MOUNTAIN )
      {
        if (random.oneIn(10))
        {
          auto * rockyBoi = new WorldObject_Rock;
          if ( random.oneIn(10) )
          {
            rockyBoi->nGold = 100;
          }
          
          aLocalTile(_x,_y).addObject(rockyBoi);
        }
        
      }
    }
  }
  
  //Generate global objects
  Vector <Tribe * > * vTribesHere = world.getTribesOn(globalX,globalY);
  
  for (int i=0;i<vTribesHere->size();++i)
  {
    Tribe * currentTribe = (*vTribesHere)(i);
    //RANDOMLY PLACE THE TRIBE CHARACTERS HERE
    Character * c = new Character;

    aLocalTile(0,0).addObject(c);
    
  }
  
	std::cout<<"Checking map data in path: "<<world.strSavePath<<"\n";
  
	// strSavePath = "savedata/"+name;
	
		// // For now, we will just delete any worlds with the same name.
	// //std::string systemCommmand = "exec rm -r "+strSavePath;
	// //system(systemCommmand.c_str());
	// FileManager::DeleteDirectory(strSavePath,true);
	

  std::string localMapPath = world.strSavePath + "/" + DataTools::toString(globalX) + "-" + DataTools::toString(globalY) + ".dat";
	
  std::cout<<"Savefile for this map is: "<<localMapPath<<"\n";
	// FileManager::createDirectory(strSavePath);
	
	if ( FileManager::directoryExists(world.strSavePath) )
  {
    if ( FileManager::fileExists(localMapPath) )
    {
    }
    else
    {
      //WRITE A FILE
      FileManager::createFile(localMapPath);
    }
	}
  else
  {
    std::cout<<"Error: Unable to access directory.\n";
  }
  
	//aIsLand.init(3000,3000,false);
	return false;
}

bool World_Local::saveToFile(std::string _path)
{
	return false;
}


#endif
