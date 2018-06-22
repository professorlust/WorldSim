#pragma once
#ifndef WORLDSIM_CHARACTER_CPP
#define WORLDSIM_CHARACTER_CPP


#include "Character.hpp"
#include "Tribe.hpp"

const int MAX_CHILDREN = 5;

//#include <Graphics/Texture/Texture.hpp>
class Texture;

Character::Character()
{
	firstName="";
	lastName="";
	epithet="";
	
		/* Give every character a unique number */
	static long unsigned int STATIC_ID = 0;
	id = STATIC_ID++;

	isMale = true;
	age = -1;
	daysCounter=0;
	secondsCounter=0;
	
	isAlive=true;
	isMarried=false;
	isPregnant=false;
	pregnantCounter=0;
	
	actionPoints = 0;
	money=0;
	
	father=0;
	mother=0;
	spouse = 0;
	
	strength=0;
	agility=0;
	charisma=0;
	intelligence=0;
	perception=0;
	endurance=0;
	courage=0;
	
	health=0;
	hunger=0;
	
	birthLocation.setXY(0,0);
	deathLocation.setXY(0,0);
	
	tribe = 0;
	
	enum enumCauseOfDeath { UNKNOWN=0, STARVATION=1, OLD_AGE=2 };
	
}

void Character::init()
{
	firstName = globalNameGen.generateName();
	lastName = globalNameGen.generateName();
	age=0;
	
	isMale = Random::flip();
	
	strength=Random::multiRoll(10,10,true);
	agility=Random::multiRoll(10,10,true);
	charisma=Random::multiRoll(10,10,true);
	intelligence=Random::multiRoll(10,10,true);
	perception=Random::multiRoll(10,10,true);
	endurance=Random::multiRoll(10,10,true);
	courage=Random::multiRoll(10,10,true);
	
	health=10;
	hunger=0;
	
	tribe=0;
	
	dateOfBirth.set(world.calendar);
}

std::string Character::getFullName() const
{
	return firstName + " " + lastName;
}

std::string Character::getBiography()
{
	
	std::string biography;
	
	biography = getFullName();
	
	if ( father == 0 || mother == 0 )
	{
		biography += " was divinely concieved.\n\n";
	}
	else
	{
		biography += " was the RELATION of " + father->getFullName() + " and " + mother->getFullName() + ".\n\n";
	}
	
	
	// The character parents
	// 
	
	
	return getFullName();
}

void Character::incrementTicks(int nTicks)
{
	secondsCounter+=nTicks;
	
	while(secondsCounter>=86400)
	{
		++daysCounter;
		secondsCounter-=86400;
	}
	//std::cout<<"Dayscounter: "<<daysCounter<<".\n";
	while(daysCounter >=360)
	{
		age++;
		daysCounter-=360;
		
		if ( hunger > 0 )
		{
			// Chance of dying at 100 hunger = 1 in 100.
			// Chance of dying at 1 hunger = 1 in 5000
			int starvationChance = (101-hunger)*10;
			
			if ( Random::oneIn(starvationChance) )
			{
				die();
			}
		}
	}
}

void Character::die()
{
	isAlive = false;
	dateOfDeath.set(world.calendar);
	
	if ( tribe != 0 )
	{
		deathLocation.setXY(tribe->worldX,tribe->worldY);
	}
	else
	{
		std::cout<<"no tribe...\n";
	}
}

bool Character::marry(Character* c)
{
	if (c==0) {return false;}
	
	isMarried = true;
	c->isMarried=true;
	spouse = c;
	c->spouse = this;
	
	if (isMale == true && c->isMale == false)
	{
		c->lastName = lastName;
	}
	else if (isMale == false && c->isMale == true)
	{
		lastName = c->lastName;
	}

	dateOfMarriage.set(world.calendar);
	c->dateOfMarriage.set(world.calendar);
	
	return true;
}

Vector <Character*> * Character::getDescendants(Vector <Character*> * vDescendants)
{
	if (vDescendants==0) {return 0;}
	
	/* GET ALL CHILDREN, THEN GET ALL CHILDREN'S CHILDREN. RETURN 0 IF NO CHILDREN. */
	if (vChildren.size() == 0)
	{
		return vDescendants;
	}
	
	//Vector <Character*> * vChildren = new Vector <Character*>;
	//auto vDescendants = new Vector <Character*>;
	
	// for (auto i=0;i<vChildren.size();++i)
	// {
	// }
	
	//for(auto const& value: vChildren.data) { }
	//for(auto const& v: vChildren) { std::cout<<"Person: "<<v->firstName<<".\n"; }
	
	
	// Push all character's children onto vector.
	for(auto const& v: vChildren) { vDescendants->push(v); v->getDescendants(vDescendants); }
	
	
	//Push all children's descendants onto vector.
	//for(auto const& v: vChildren) { v->getDescendants(vDescendants); }
	
	vDescendants->removeDuplicates();
	
	return vDescendants;
}

Vector <Character*> * Character::getRelatives()
{
	Vector <Character*> vCloseAncestors;
	
	//auto vCloseAncestors = new Vector <Character*>;
	auto vAncestors = new Vector <Character*>;
	
		// The character cannot marry any descendants of their grandparents. So first we get all 4 grandparents. If there are no grandparents, we work from parents. If there are no parents, we work from this.
	if ( father != 0)
	{
		if (father->father != 0)
		{
			vCloseAncestors.push(father->father);
		}
		if(father->mother != 0)
		{
			vCloseAncestors.push(father->mother);
		}
		if ( father->father == 0 && father->mother == 0)
		{
			vCloseAncestors.push(father);
		}
	}
	else
	{
		vCloseAncestors.push(this);
	}
	if ( mother != 0)
	{
		if (mother->father != 0)
		{
			vCloseAncestors.push(father->father);
		}
		if(mother->mother != 0)
		{
			vCloseAncestors.push(mother->mother);
		}
		if ( mother->father == 0 && mother->mother == 0)
		{
			vCloseAncestors.push(mother);
		}
	}
	else
	{
		vCloseAncestors.push(this);
	}
	
		// Get all descendants of all these relatives.
	for(auto const& v: vCloseAncestors) { vAncestors->push(v); v->getDescendants(vAncestors); }
	
	vAncestors->removeDuplicates();
	
	return vAncestors;
}

void Character::starve()
{
	//--health;	
	++hunger;
	
	if (hunger > 100)
	{
		hunger = 100;
	}
	
	//if ( health <= 0)
	//{
		//std::cout<<"Starved to death.\n";
	//	isAlive = false;
	//}
}

bool Character::canMarry(Character* c)
{
	//std::cout<<"Checking if can marry.\n";
	
	
		//Cannot marry null pointer or yourself or underage or same sex.
	if(c==0 || c==this || isAlive==false || c->isAlive==false || age<16 || c->age<16 || isMale==c->isMale || spouse !=0 || c->spouse!=0 ) { return false; }
	
	//auto vRelatives = getRelatives();
	//if ( vRelatives->contains(c) )
	//{
	//	delete vRelatives;
	//	return false;
	//}
	
	return true;
}

Character* Character::giveBirth()
{
	
	Character * babby = new Character;
	babby->init();
	babby->lastName = lastName;
		// Inherit tribe membership from mother I guess.
	babby->tribe = tribe;
	
	vChildren.push(babby);
	spouse->vChildren.push(babby);
	
	if(isMale)
	{
		babby->father=this;
		if ( spouse!=0 )
		{
			babby->mother=this->spouse;
		}
	}
	else
	{
		babby->mother=this;
		if ( spouse!=0 )
		{
			babby->father=this->spouse;
		}
	}
	
	return babby;
}

Texture* Character::currentTexture ()
{	
	return &TEX_NPC;
}

// COUT OPERATOR

std::ostream& operator<<(std::ostream& stream, const Character& c)
{
	return stream<<c.getFullName();
}

// TABLE INTERFACE

/* TABLE INTERFACE */

std::string Character::getColumn(std::string _column)
{
	if ( _column=="firstname" )
	{
		return firstName;
	}
	else if ( _column=="lastname" )
	{
		return lastName;
	}
	if ( _column=="age" )
	{
		return DataTools::toString(age);
	}
	// if ( _column=="coordinates" )
	// {
		// return DataTools::toString(worldX)+","+DataTools::toString(worldY);
	// }
	// if ( _column=="territory" )
	// {
		// return DataTools::toString(vTerritory.size());
	// }
	// // else if ( _column=="id" )
	// // {
		// // return DataTools::toString(id);
	// // }
	
	return "?";
}
std::string Character::getColumnType(std::string _column)
{
	if ( _column == "age" || _column == "territory" || _column == "food" )
	{
		return "number";
	}
	return "string";
}

#endif