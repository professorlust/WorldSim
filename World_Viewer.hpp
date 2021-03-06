#pragma once

#ifndef GUILD_WORLD_VIEWER_HPP
#define GUILD_WORLD_VIEWER_HPP

#include <Device/Display/DisplayInterface.hpp>
#include <Device/Mouse/MouseInterface.hpp>
#include <Render/Renderer.hpp>

#include <WorldGenerator/Biome.hpp> // For rendering biomes (Wow this is terrible).

/* #include "World_Viewer.hpp"

  Code for rendering the player's view of the world.
  
	I have a dream that one day I will be able to understand this code.
	
*/

class WorldViewer: public DisplayInterface, public MouseInterface
{
	public:
	
	bool active;
	
	int tileSize; /* How big to render the tiles, in pixels. */
	
    /* The tile at the center of the screen.
      Note that the mantissa is used to center on local tiles.
      This must be a double due to the required level of precision.
    */
  double centerTileX, centerTileY;
  double localTileMantissa;
	
		/* Store the last position of the mouse, so we can figure out which tile the mouse is on. */
	short int lastMouseX, lastMouseY;
	
	World* world;
	World_Local* worldLocal;
	
	/* Rendering coordinates. */
	int mainViewX1, mainViewX2, mainViewY1, mainViewY2;
	int mainViewNX, mainViewNY;
	
	bool panning;
	float panX, panY; /* Stores the last mouse position when panning. Needs to be a double. */

	bool alternateTile;
	
		/* The current tile that the mouse is hovering over. Set to -1 if the mouse is not hovering over a tile. */
	int hoveredXTile, hoveredYTile;
	
	int tilesToSkip;
	
	bool territoryView;
  bool tilesetMode;
  bool subterraneanMode;
	
		// Temp: Local tile to render.
	int centerLocalX, centerLocalY;
  double pixelsPerLocalTile;
	
    // If true, game will randomly scroll around.
    // In the future, the title screen will probably be scrolling around a map.
  bool demoMode;
  double demoScroll;
  
WorldViewer()
{
  tileSize=8;
  centerTileX=0; centerTileY=0;
  world=0;
  mainViewX1=0; mainViewX2=0;
  mainViewY1=0; mainViewY2=0;
  mainViewNX=0; mainViewNY=0;
  active=false;
  
  panX=0; panY=0;
  
  panning=false;
  alternateTile=0;
  
  lastMouseX=0;
  lastMouseY=0;
  
  hoveredXTile=0;
  hoveredYTile=0;
  
  tilesToSkip=0;
  
  territoryView = false;
  
  centerLocalX=LOCAL_MAP_SIZE/2;
  centerLocalY=LOCAL_MAP_SIZE/2;
  
  tilesetMode = true;
  subterraneanMode = false;
  
  pixelsPerLocalTile = tileSize/LOCAL_MAP_SIZE;
  
  localTileMantissa = (double)1/(double)LOCAL_MAP_SIZE;
  
  demoMode = false;
  demoScroll = 0.1;
}
	
  // Center the view on the middle of the world. Good for initializing.
void centerView()
{
  if (world==0)
  {
    return;
  }
  centerTileX = world->nX/2;
  centerTileY = world->nY/2;
  
  centerTileX+=0.5;
  centerTileY+=0.5;
}
	
bool keyboardEvent( Keyboard* _keyboard )
{
	//std::cout<<"Keypress: "<<(int)_keyboard->lastKey<<".\n";
  
  //std::cout<<"pixelsPerLocalTile: "<<pixelsPerLocalTile<<".\n";

	/* Pass keyboard event, instead of handling individual keypressed here. */
	
	/* Keyboard handling for main view. */
	
	//if(keyboard->keyWasPressed&&playerControl)
	// if(keyboard->keyWasPressed)
	// {
    
      // If the player is controlling someone, arrows should indicate local tile movement.
    // if ( world->playerCharacter )
    // {
      // if(_keyboard->isPressed(Keyboard::RIGHT))
      // {
        // centerTileX += localTileMantissa;
      // }
      // else if(_keyboard->isPressed(Keyboard::LEFT))
      // {
        // centerTileX -= localTileMantissa;
      // }
      // else if(_keyboard->isPressed(Keyboard::UP))
      // {
        // centerTileY += localTileMantissa;
      // }
      // else if(_keyboard->isPressed(Keyboard::DOWN))
      // {
        // centerTileY -= localTileMantissa;
      // }
    // }
      // // If the player is not controlling someone, arrows should indicate global tile movement.
    // else
    // {
      // if(_keyboard->isPressed(Keyboard::RIGHT))
      // {
        // ++centerTileX;
      // }
      // else if(_keyboard->isPressed(Keyboard::LEFT))
      // {
        // --centerTileX;
      // }
      // else if(_keyboard->isPressed(Keyboard::UP))
      // {
        // ++centerTileY;
      // }
      // else if(_keyboard->isPressed(Keyboard::DOWN))
      // {
        // --centerTileY;
      // }
    // }
		/* Zoom the main map in by one step.  (Can use either top row or numpad)*/
		if(_keyboard->isPressed(Keyboard::EQUALS) || _keyboard->isPressed(Keyboard::PLUS))
		{
			if (_keyboard->keyWasPressed)
			{
				zoomIn();
			}
		}
		/* Zoom the main map out by one step. */
		if(_keyboard->isPressed(Keyboard::MINUS))
		{
			if (_keyboard->keyWasPressed)
			{
				zoomOut();
			}
		}
    
		if(_keyboard->isPressed(Keyboard::TAB))
		{
			if (_keyboard->keyWasPressed)
			{
				subterraneanMode = !subterraneanMode;
			}
		}
		// if(keyboard->isPressed(Keyboard::SPACEBAR))
		// {
		// }
		// if (keyboard->isPressed(Keyboard::Q)||keyboard->isPressed(Keyboard::q))
		// {
			// std::cout<<"I'm afraid I can't let you do that.\n";
		// }
		// if(keyboard->isPressed(Keyboard::ESCAPE))
		// {
			// queryMode=false;
			// buildSubMenu=false;
			// buildSubMenuChoice=0;
		// }
		if(_keyboard->isPressed(Keyboard::D)||_keyboard->isPressed(Keyboard::d)) /* Debug info. */
		{
			// std::cout<<"Debug Info:\n";
			
			// if (world==0)
			// {
				// std::cout<<"ERROR: World doesn't exist.\n";
			// }
			// else
			// {
			
				// std::cout<<"Num tribes: "<<world->vTribe.size()<<"\n";
				
				// for (int i=0;i<world->vTribe.size();++i)
				// {
					// Tribe* t = world->vTribe(i);
					// std::cout<<"All living members of tribe "<<t->name<<".\n";
					
					// for(int j=0;j<t->vCharacter.size();++j)
					// {
						// Character* c = t->vCharacter(j);
						// if(c->isAlive)
						// {
						// std::cout<<c->id<<": "<<c->getFullName()<<", age: "<<c->age<<". ";
						// if(c->isMale==true)
						// { std::cout<<"Male.\n"; }
						// else
						// { std::cout<<"Female.\n";
						// }
						// }
					// }
					
				// }
				// std::cout<<"World population: "<<world->getPopulation()<<".\n";
			// }
			
			// std::cout.flush();
			
			// _keyboard->keyUp(Keyboard::D);
			// _keyboard->keyUp(Keyboard::d);
		}
		
		
		if(_keyboard->isPressed(Keyboard::F)||_keyboard->isPressed(Keyboard::f)) /* Flush console. */
		{

			std::cout.flush();
			
			_keyboard->keyUp(Keyboard::F);
			_keyboard->keyUp(Keyboard::f);
		}

	return false;
}

void zoomIn()
{
	if ( tilesToSkip > 0 ) { --tilesToSkip; }
	else
	{
  tileSize*=2;
	if(tileSize>4000000)
	{ tileSize=4000000; }
	}
	//std::cout<<"Tilesize: "<<tileSize<<".\n";
	// if (tileSize < 32 && world->isSafe(hoveredXTile, hoveredYTile))
	// {
		// centerTileX=hoveredXTile;
		// centerTileY=hoveredYTile;
		// glutWarpPointer(mainViewX1+(mainViewNX/2),mainViewY1+(mainViewNY/2));
	// }
}
void zoomOut()
{
	tileSize/=2;
  
	if(tileSize<1)
	{ tileSize=1; ++tilesToSkip; }


}

void switchTarget(World_Local* _worldLocal)
{
	worldLocal=_worldLocal;
}

	
		/* Keep the map visible on the screen so the player can't accidentally move it off screen and lose it. */
	void normaliseCoordinates()
	{
		if ( centerTileX < 0 )
		{
			centerTileX=0;
		}
		if ( centerTileY < 0 )
		{
			centerTileY=0;
		}
		if ( centerTileX > world->nX )
		{
			centerTileX = world->nX;
		}
		if ( centerTileY > world->nY )
		{
			centerTileY = world->nY;
		}
	}
	
	bool mouseEvent ( Mouse* mouse )
	{
		if ( world==0 ) { return false; }
	
		lastMouseX=mouse->x;
		lastMouseY=mouse->y;
		
		if(panning==true)
		{
			/* The view can be panned by holding down the middle mouse button
				and moving the mouse to drag the map. */
			centerTileX+=(panX-mouse->x)/((float)tileSize/(tilesToSkip+1));
			centerTileY+=(panY-mouse->y)/((float)tileSize/(tilesToSkip+1));
			panX=mouse->x;
			panY=mouse->y;
		}
		
		if(mouse->isMiddleClick==false)
		{
			panning=false;
		}
		if ( mouse->isMiddleClick)
		{
			//std::cout<<"Middle click.\n";
			panning=true;
			//panning=!panning;
			
			/* Set initial pan coordinates. */
			panX=mouse->x;
			panY=mouse->y;
			
			//mouse->isMiddleClick=false;
		}
		
		if(mouse->isWheelDown)
		{
			zoomOut();

			mouse->isWheelDown=false;
		}
		if(mouse->isWheelUp)
		{
			zoomIn();
			
			mouse->isWheelUp=false;
		}
		
		normaliseCoordinates();
		
		if ( mouse->isLeftClick == true )
		{
			//if ( isSafe(hover
			//std::cout<<"Clicked on: "<<hoveredXTile<<", "<<hoveredYTile<<".\n";
			
			world->queryTile(hoveredXTile,hoveredYTile);
		}
		
		if (mouse->isRightClick)
		{
			//std::cout<<"Adding tile: "<<hoveredXTile<<", "<<hoveredYTile<<" to render.\n";
			//localX=hoveredXTile;
			//localY=hoveredYTile;
      world->generateLocal(hoveredXTile,hoveredYTile);
		}
		
		return false;
	}
	
	void setPanel( const int _x1, const int _y1, const int _x2, const int _y2)
	{
		//std::cout<<"Worldviewer has been resized.\n";
		//std::cout<<"Coords: "<<mainViewX1<<","<<mainViewY1<<","<<mainViewX2<<","<<mainViewY2<<".\n";
	//	std::cout<<"MVSP\n";
		mainViewX1 = _x1;
		mainViewY1 = _y1;
		mainViewX2 = _x2;
		mainViewY2 = _y2;
		
		mainViewNX = mainViewX2-mainViewX1;
		mainViewNY = mainViewY2-mainViewY1;
		
	}
	
	inline void setCenterTile (const double _centerTileX, const double _centerTileY, const int _subTileX=LOCAL_MAP_SIZE/2, const int _subTileY=LOCAL_MAP_SIZE/2)
	{
		centerTileX = _centerTileX;
		centerTileY = _centerTileY;
    
    centerTileX += (_subTileX * localTileMantissa)+localTileMantissa/2;
    centerTileY += (_subTileY * localTileMantissa)+localTileMantissa/2;
	}
	
		/* Convert tile coordinates to screen (pixel) coordinates. */
	void toScreenCoords(const int _tileX, const int _tileY, int * _pixelX, int * _pixelY)
	{
		
		float centerTileXDecimal = centerTileX - (int)centerTileX;
		float centerTileYDecimal = centerTileY - (int)centerTileY;
		
		float centerTileXPixels = centerTileXDecimal*tileSize;
		float centerTileYPixels = centerTileYDecimal*tileSize;
	

		//double pixelOffsetX = tileSize*centerTileXDecimal;
		//double pixelOffsetY = tileSize*centerTileYDecimal;

		const int iCenterTileX = centerTileX;
		

		const int tilesToDrawX = (mainViewNX/tileSize) + 1;
		const int tilesToDrawY = (mainViewNY/tileSize) + 1;
		
		int tileX=centerTileX;
		int pixelTileX = mainViewX1+(mainViewNX/2) - centerTileXPixels;
		while(pixelTileX>=mainViewX1)
		{
			--tileX;
			tileX-=tilesToSkip;
			pixelTileX-=tileSize;
			
		}
		const int revertTileX = tileX;
		
		int tileY=centerTileY;
		int pixelTileY = mainViewY1 + (mainViewNY/2) - centerTileYPixels;
		while(pixelTileY>=0)
		{
			--tileY;
			tileY-=tilesToSkip;
			pixelTileY-=tileSize;
		}
		
		
		int nExtraXTiles = _tileX - tileX;
		int nExtraYTiles = _tileY - tileY;
		
		if ( nExtraXTiles>=0 && nExtraYTiles>=0 )
		{
			*_pixelX = pixelTileX + (tileSize * nExtraXTiles);
			*_pixelY = pixelTileY + (tileSize * nExtraYTiles);
		}
		else
		{
			*_pixelX = -1000;
			*_pixelY = -1000;
		}
	}
	
	
		/* HERE I RENDER THE GRAPHICAL ICONS WHICH ARE PRESENT ON THE WORLD MAP, FOR EXAMPLE: CITIES AND ARMIES. */
		/* ONLY A SMALL NUMBER OF WORLD TILES WILL HAVE AN ICON ON THEM, SO I WILL USE A DIFFERENT TECHNIQUE. */
	void renderWorldIcons()
	{
		//return;
		//std::cout<<"Rendering world icons.\n";
	
		if (active == false)
		{ return; }
		if(world==0)
		{
			std::cout<<"ABORT: No world to render.\n";
			return;
		}
		
			/* 0223554692 - SO I GUESS THIS PART MODIFIES THE SCREEN SO I CAN SIMPLY DRAW FROM (0,0). */
		Renderer::saveViewPort();
		Renderer::resizeViewPort(mainViewX1,mainViewY1,mainViewX2,mainViewY2);
		
			/* CONFUSING SHIT WHICH IS HARD TO EXPLAIN. */
		
		float centerTileXDecimal = centerTileX - (int)centerTileX;
		float centerTileYDecimal = centerTileY - (int)centerTileY;
		
		float pixelOffsetX = tileSize*centerTileXDecimal;
		float pixelOffsetY = tileSize*centerTileYDecimal;

		const int iCenterTileX = centerTileX;
		
		const int tilesToDrawX = (mainViewNX/tileSize) + 1;
		const int tilesToDrawY = (mainViewNY/tileSize) + 1;
		
		int tileX=centerTileX;
		int pixelTile = mainViewNX/2;
		while(pixelTile>=0)
		{
			--tileX;
			pixelTile-=tileSize;
		}
		const int revertTileX = tileX;
		
		int tileY=centerTileY;
		pixelTile = mainViewNY/2;
		while(pixelTile>=0)
		{
			--tileY;
			pixelTile-=tileSize;
		}
		
			/* ALLOWS DRAWING OF TEXTURES. */
		Renderer::setTextureMode();
		
			// USING THE NEW 'WORLDOBJECTGLOBAL' INTERFACE.
		for (int i=0;i<world->vWorldObjectGlobal.size();++i)
		{
			WorldObjectGlobal* wog = world->vWorldObjectGlobal(i);
      
      
        // Check if we're supposed to render a local map on this tile.
        // If so, don't draw world icons over it.
      if ( tileSize > 4 && world->isSafe(tileX,tileY) )
      {
        
        for ( int i2=0;i2<world->vWorldLocal.size();++i2)
        {
          if ( world->vWorldLocal(i2)->globalX == wog->worldX && world->vWorldLocal(i2)->globalY == wog->worldY )
          {
            wog = 0;
            break;
          }
        }
        
      }
      if ( wog == 0 ) { continue; }
      
			int wogPixelX=-1;
			int wogPixelY=-1;

			toScreenCoords(wog->worldX,wog->worldY,&wogPixelX,&wogPixelY);
      

			
				// I NEED TO DO A CHECK TO SEE IF THE OBJECT IS VISIBLE WITHIN THE CURRENT VIEW.
			
			if ( wogPixelX != -1000 && wogPixelY != -1000 )
			{
				Renderer::placeTexture4(wogPixelX,wogPixelY,wogPixelX+tileSize,wogPixelY+tileSize,wog->currentTexture(),false);
			}
		}
			/* RESTORE THE VIEWPORT TO THE PREVIOUS SETTING (PROBABLY THE WHOLE PANEL). */
		Renderer::restoreViewPort();
		
	}
	
	void render()
	{
		
		if (active == false)
		{ return; }
		if(world==0)
		{
			std::cout<<"ABORT: No world to render.\n";
			return;
		}
    
    if ( demoMode )
    {
      if ( centerTileX < 10 && demoScroll < 0 )
      {
        demoScroll = demoScroll * -1;
      }
      else if ( centerTileX > world->nX - 10 && demoScroll > 0 )
      {
        demoScroll = demoScroll * -1;
      }
      
      centerTileX += demoScroll;
    }

			/* 0223554692
				SO I GUESS THIS PART MODIFIES THE SCREEN SO I CAN SIMPLY DRAW FROM (0,0).
				I THINK IT ALSO PREVENTS ACCIDENTALLY RENDERING OVER OTHER PARTS OF THE SCREEN.
			*/
		Renderer::saveViewPort();

		Renderer::resizeViewPort(mainViewX1,mainViewY1,mainViewX2,mainViewY2);
		
		float centerTileXDecimal = centerTileX - (int)centerTileX;
		float centerTileYDecimal = centerTileY - (int)centerTileY;
		
		float centerTileXPixels = centerTileXDecimal*tileSize;
		float centerTileYPixels = centerTileYDecimal*tileSize;
    
    pixelsPerLocalTile = ((double)tileSize/LOCAL_MAP_SIZE);

		const int iCenterTileX = centerTileX;
		

		const int tilesToDrawX = (mainViewNX/tileSize) + 1;
		const int tilesToDrawY = (mainViewNY/tileSize) + 1;
		
		int tileX=centerTileX;
		int pixelTileX = mainViewX1+(mainViewNX/2) - centerTileXPixels;
		while(pixelTileX>=mainViewX1)
		{
			--tileX;
			tileX-=tilesToSkip;
			pixelTileX-=tileSize;
			
		}
		const int revertTileX = tileX;
		
		int tileY=centerTileY;
		int pixelTileY = mainViewY1 + (mainViewNY/2) - centerTileYPixels;
		while(pixelTileY>=0)
		{
			--tileY;
			tileY-=tilesToSkip;
			pixelTileY-=tileSize;
		}

			/* UPDATE HOVERED TILE COORDS. */
			/* MAKE SURE MOUSE IS WITHIN THE RENDERING VIEW */
		if (lastMouseX >= mainViewX1 && lastMouseX <= mainViewX2 && lastMouseY >= mainViewY1 && lastMouseY <= mainViewY2 )
		{
			const int startingPX = pixelTileX;
			const int startingPY = pixelTileY;
			
			int diffX = (lastMouseX - startingPX)/tileSize;
			int diffY = (lastMouseY - startingPY)/tileSize;
			
				/* UPDATE THE COORDS OF THE HOVERED TILE. */
			hoveredXTile = tileX + diffX;
			hoveredYTile = tileY + diffY;
		}
		else
		{
			hoveredXTile=-1;
			hoveredYTile=-1;
		}
		
		if ( tilesetMode == false )
		{

			/* Pixel coords for leftmost tile. */
			Renderer::setColourMode();

			for (int currentY = pixelTileY; currentY<mainViewNY; currentY+=tileSize)
			{
				//break;
				glBegin(GL_QUAD_STRIP);

				
				/* NOTE: 022-171 added this initial case. The left side of the map wasn't drawing, so I put this here, and moved the other glColor3ub from above the vertexes to below them. This seems to have fixed the rendering. The tiles were always there, they just weren't rendering... */
				glColor3ub(0,0,0);
				unsigned char lastRed=0;
				unsigned char lastGreen=0;
				unsigned char lastBlue=0;
				
				//for (int currentX = -pixelOffsetX; currentX<mainViewNX+tileSize; currentX+=tileSize)
				for (int currentX = pixelTileX; currentX<mainViewNX+tileSize; currentX+=tileSize)
				{
				
					if(world->isSafe(tileX,tileY)==true)
					{
						
						glVertex2s(currentX,currentY);
						glVertex2s(currentX,currentY+tileSize);
						
							// Territory view
						if (territoryView)
						{
              const Tribe* dominantTribe = world->aWorldTile(tileX,tileY).getDominantInfluence();
              if (dominantTribe!=0)
              {
                glColor3ub(dominantTribe->colourRed,dominantTribe->colourGreen,dominantTribe->colourBlue);
              }
              else
              {
                glColor3ub(world->aTopoMap(tileX,tileY,0),world->aTopoMap(tileX,tileY,1),world->aTopoMap(tileX,tileY,2));
              }
            }
						else
						{
							if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==false)
							{
								//enderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_OCEAN_00, false);
								glColor3ub(TEX_WORLD_TERRAIN_OCEAN_00.averageRed,TEX_WORLD_TERRAIN_OCEAN_00.averageGreen,TEX_WORLD_TERRAIN_OCEAN_00.averageBlue);
							}
							//else if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==true)
							//{
							//	glColor3ub(TEX_WORLD_TERRAIN_GRASS_00.averageRed,TEX_WORLD_TERRAIN_GRASS_00.averageGreen,TEX_WORLD_TERRAIN_GRASS_00.averageBlue);
							//}
							
							
							else
							{
								glColor3ub(world->aTopoMap(tileX,tileY,0),world->aTopoMap(tileX,tileY,1),world->aTopoMap(tileX,tileY,2));
							}
							
						}


							// ONLY DRAW STUFF THE THE PLAYER HAS REVEALED.
							// FOG OF WAR GOES HERE.

						
							// Basic landmass view
						if ( world->aIsLand(tileX,tileY) )
						{
							//glColor3ub(50,200,50);
						}
						else
						{
							//glColor3ub(50,50,200);
						}
							//landmass ID view
						// if ( world->aIsLand(tileX,tileY) && world->aLandmassID(tileX,tileY) != -1 )
						// {
							// glColor3ub(50,200,50);
						// }
						// else if ( world->aIsLand(tileX,tileY) )
						// {
							// glColor3ub(200,50,50);
						// }
						// else
						// {
							// glColor3ub(50,50,200);
						// }
						

					}
					
					/* NOTE: 022-171. Final case here, otherwise last x doesn't render. */
					else if(world->isSafe(tileX-1,tileY)==true)
					{
						glVertex2s(currentX,currentY);
						glVertex2s(currentX,currentY+tileSize);
					}

					++tileX;
					tileX+=tilesToSkip;
				}
				glEnd();

				tileX=revertTileX;
				++tileY;
				tileY+=tilesToSkip;
			}
			
		}
		else
		{
			
			Renderer::setTextureMode();

			for (int currentY = pixelTileY; currentY<mainViewNY; currentY+=tileSize)
			{
				//glBegin(GL_QUAD_STRIP);

				
				/* NOTE: 022-171 added this initial case. The left side of the map wasn't drawing, so I put this here, and moved the other glColor3ub from above the vertexes to below them. This seems to have fixed the rendering. The tiles were always there, they just weren't rendering... */
				glColor3ub(255,255,255);
				//unsigned char lastRed=0;
				//unsigned char lastGreen=0;
				//unsigned char lastBlue=0;
				
				for (int currentX = pixelTileX; currentX<mainViewNX+tileSize; currentX+=tileSize)
				{
          
            // Check if we're supposed to render a local map on this tile.
          World_Local * localMap = 0;
          if ( tileSize >= 12 && world->isSafe(tileX,tileY) )
          {
            
            for ( int i=0;i<world->vWorldLocal.size();++i)
            {
              if ( world->vWorldLocal(i)->globalX == tileX && world->vWorldLocal(i)->globalY == tileY )
              {
                localMap = world->vWorldLocal(i);
                break;
              }
            }
            
          }
          
						// RENDER THE LOCAL TILE
					//if (tileSize > 4 && localX == tileX && localY == tileY && world->isSafe(tileX,tileY))
          if ( localMap != 0)
					{
						
						float currentSubY = currentY;
						float nextSubY = currentY + pixelsPerLocalTile;
						
						int subTileSize = pixelsPerLocalTile;
						if (subTileSize < 1) { subTileSize = 1; }
						
            // First we must seed the RNG with the seed for this local tile.
						Random r1;
						r1.seed (world->aSeed(world->localX,world->localY));
            
            const enumBiome localBaseBiome = world->aTerrain(world->localX,world->localY);
						
						for (int localYTile = 0; localYTile<LOCAL_MAP_SIZE;++localYTile)
						{
							
							if ( nextSubY>=mainViewY1 && currentSubY <= mainViewY2 && floor(currentSubY) != floor(nextSubY) )
							{
							
								float currentPixel = currentX;
								float nextPixel = currentX+pixelsPerLocalTile;
								for (int localXTile = 0; localXTile<LOCAL_MAP_SIZE;++localXTile)
								{
									if ( nextPixel>=mainViewX1 && currentPixel <= mainViewX2 && floor(currentPixel) != floor(nextPixel) )
									{
                    
                    if ( subterraneanMode )
                    {
                      LocalTile* localTile = &localMap->aSubterranean(localXTile,localYTile);
                      
                      Renderer::placeTexture4(currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), localTile->currentTexture(), false);
                    }
                    else
                    {
                      
                      LocalTile* localTile = &localMap->aLocalTile(localXTile,localYTile);
                      
                      unsigned char lightValue = localTile->height*15;
                      glColor3ub(180+lightValue,180+lightValue,180+lightValue);
                      Renderer::placeTexture4(currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), localTile->currentTexture(), false);
                      glColor3ub(255,255,255);
                      
                      
                      for(int i=0;i<localMap->aLocalTile(localXTile,localYTile).vObject.size();++i)
                      {
                        Renderer::placeTexture4(currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), localMap->aLocalTile(localXTile,localYTile).vObject(i)->currentTexture(), false);
                      }
                    }
                    
                    
    
										

									}
									else
									{
										//r1.oneIn(10);
									}
									
									currentPixel=nextPixel;
									nextPixel+=pixelsPerLocalTile;
								}
							}
							else
							{
								for (int localXTile = 0; localXTile<LOCAL_MAP_SIZE;++localXTile)
								{
                  //r1.oneIn(10);
								}
							}
							currentSubY=nextSubY;
							nextSubY+=pixelsPerLocalTile;
						}
						//std::cout<<"\n";
						//Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_00, false);
					}
					else
					{
						/* Textures are chosen here rather than from tile objects because it is highly dependent on neighboring tiles. It would be possible to delegate texture handling to tile objects, but would take too much memory maintaining pointers to neighbours. In future maybe worldtile can return hardcoded textures chosen by world object. */
						
						// if (world->isSafe(tileX,tileY) )
						// {
							// //Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, world->aWorldTile(tileX,tileY).currentTexture(), false);
						// }
            
            
				
						if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY))
						{
							// //Renderer::pair
							//Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_00, false);
							
							
							if (world->aSeed(tileX,tileY) % 4 == 0)
							{
                glColor4f(2.0f, 2.0f, 2.0f, 1.0f);
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_00, false);
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
							}
							else if (world->aSeed(tileX,tileY) % 4 == 1)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_01, false);
							}
							else if (world->aSeed(tileX,tileY) % 4 == 2)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_02, false);
							}
							else if (world->aSeed(tileX,tileY) % 4 == 3)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_03, false);
							}
							
							if ( world->isSafe(tileX+1,tileY) && world->isLand(tileX+1,tileY) ==false )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_00, false);
							}
							if ( world->isSafe(tileX-1,tileY) && world->isLand(tileX-1,tileY) ==false )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_02, false);
							}
							if ( world->isSafe(tileX,tileY+1) && world->isLand(tileX,tileY+1) ==false )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_03, false);
							}
							if ( world->isSafe(tileX,tileY-1) && world->isLand(tileX,tileY-1) ==false )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_01, false);
							}
							
							if ( world->isSafe(tileX+1,tileY) && world->isLand(tileX+1,tileY) ==false
							&& world->isSafe(tileX,tileY+1) && world->isLand(tileX,tileY+1) ==false)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_CORNER_00, false);
							}
							if ( world->isSafe(tileX+1,tileY) && world->isLand(tileX+1,tileY) ==false
							&& world->isSafe(tileX,tileY-1) && world->isLand(tileX,tileY-1) ==false)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_CORNER_01, false);
							}
							if ( world->isSafe(tileX-1,tileY) && world->isLand(tileX-1,tileY) ==false
							&& world->isSafe(tileX,tileY+1) && world->isLand(tileX,tileY+1) ==false)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_CORNER_03, false);
							}
							if ( world->isSafe(tileX-1,tileY) && world->isLand(tileX-1,tileY) ==false
							&& world->isSafe(tileX,tileY-1) && world->isLand(tileX,tileY-1) ==false)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_CORNER_02, false);
							}
							
							if ( world->isSafe(tileX+1,tileY) && world->isLand(tileX+1,tileY) ==false
							&& world->isSafe(tileX,tileY+1) && world->isLand(tileX,tileY+1) ==false
							&& world->isSafe(tileX-1,tileY) && world->isLand(tileX-1,tileY) ==false )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_PENINSULA_00, false);
							}
							
							if ( world->isSafe(tileX+1,tileY) && world->isLand(tileX+1,tileY) ==false
							&& world->isSafe(tileX,tileY-1) && world->isLand(tileX,tileY-1) ==false
							&& world->isSafe(tileX-1,tileY) && world->isLand(tileX-1,tileY) ==false )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_PENINSULA_02, false);
							}


							
							if ( world->aSeed(tileX,tileY) % 20 == 0 )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_ALCHEMY, false);
							}
							// glVertex2s(currentX,currentY);
							// glVertex2s(currentX,currentY+tileSize);
							
								// //ONLY DRAW STUFF THE THE PLAYER HAS REVEALED.
								// //FOG OF WAR GOES HERE.
							// glColor3ub(world->aTopoMap(tileX,tileY,0),world->aTopoMap(tileX,tileY,1),world->aTopoMap(tileX,tileY,2));
							
							// if ( world->aInfluence(tileX,tileY) != 0 )
							// {
								// Tribe* dominantTribe = world->getDominantInfluence(tileX,tileY);
								// if (dominantTribe!=0)
								// {
									// glColor3ub(dominantTribe->colourRed,dominantTribe->colourGreen,dominantTribe->colourBlue);
								// }
								// else
								// {
									// glColor3ub(world->aTopoMap(tileX,tileY,0),world->aTopoMap(tileX,tileY,1),world->aTopoMap(tileX,tileY,2));
								// }
							// }
							// else
							// {
								// glColor3ub(world->aTopoMap(tileX,tileY,0),world->aTopoMap(tileX,tileY,1),world->aTopoMap(tileX,tileY,2));
							// }
						}
						else if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==false)
						{
							Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_OCEAN_00, false);
						}
						
						if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==true && world->aTerrain(tileX,tileY) == FOREST)
						{
							Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_FOREST_TREES, false);
						}
						
						if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==true && world->aTerrain(tileX,tileY) == MOUNTAIN)
						{
							//Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_00, false);
              glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
							Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_MOUNTAIN_00, false);
              glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
						}

						if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==true && world->aTerrain(tileX,tileY) == DESERT)
						{
							Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_DESERT_00, false);
						}
						if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==true && world->aTerrain(tileX,tileY) == DESERT)
						{
							Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_DESERT_01, false);
						}
						
						
						
					
					
					}
					//TEX_WORLD_TERRAIN_FOREST_TREE
					
					/* NOTE: 022-171. Final case here, otherwise last x doesn't render. */
					// else if(world->aHeightMap.isSafe(tileX-1,tileY)==true)
					// {
						// glVertex2s(currentX,currentY);
						// glVertex2s(currentX,currentY+tileSize);
					// }

					++tileX;
					tileX+=tilesToSkip;
				}
				//glEnd();

				tileX=revertTileX;
				++tileY;
				tileY+=tilesToSkip;
			}
			
		// Texture mode
		//Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_01, false);
		
	}

		Renderer::restoreViewPort();
		

		
		
		/* Now render the icons on the world map. */
		renderWorldIcons();
    
    
			//Renderer::placeTexture4(mainViewX1+(mainViewNX/2)-32, mainViewY1+(mainViewNY/2)-32, mainViewX1+(mainViewNX/2)+32, mainViewY1+(mainViewNY/2)+32, &TEX_WORLD_TEST_00, false);
		normaliseCoordinates();
		
	}

	
};

#endif
