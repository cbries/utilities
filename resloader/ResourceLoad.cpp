/*
 * Copyright (C) 2012 Christian Benjamin Ries
 * Christian Benjamin Ries <mail@christianbenjaminries.de>
 * 
 * Date: 25th November 2012
 * License: I don't care!
 */
// C++
#include <iostream>
#include <string>

// Resource
#include "ResourceBlock.h"

Resource::Block blockSingleSprites  ("SingleSprites",   Resource::VeryHigh);
Resource::Block blockGame           ("Game",            Resource::High);
Resource::Block blockFeature        ("Feature",         Resource::Low);
Resource::Block blockRedBlack       ("RedBlack",        Resource::Low);

void setupBlocks();

/** @brief
 *************************************************************/
int main(int argc, char **argv)
{
    //
    // Query the planned loading mode.
    // 
    bool blockThreading = false;
    bool resourceThreading = false;

    for(int i=1; i<argc; i++) 
    {
        if(!strcmp(argv[i], "-h"))
        {
            std::cout << "-bt  Enables block threading" << std::endl;
            std::cout << "-rt  Enables resource threading" << std::endl;
        }
        if(!strcmp(argv[i], "-bt")) blockThreading = true;
        if(!strcmp(argv[i], "-rt")) resourceThreading = true;
    }

    //
    // Set-up of all Blocks
    // 

    setupBlocks();

    Resource::Schedule::AddBlockRef(&blockSingleSprites);
    Resource::Schedule::AddBlockRef(&blockGame);
    Resource::Schedule::AddBlockRef(&blockFeature);
    Resource::Schedule::AddBlockRef(&blockRedBlack);

  clock_t t1 = clock();

    //
    // Start loading of all Blocks based on a 
    // simple scheduling algorithm, i.e. prioritization.
    //
    try 
    {
        for(int i=0; /* empty */; i++)
        {
            // Test for prioritize Block manually, i.e.
            // change the loading order from "Game -> Feature -> RedBlack"
            // to "RedBlack -> Game -> Feature". Enables changing of the
            // order when prioritisation schouldn't be taken into account.
            //-if(i == 1) 
            //-{
            //-    Resource::Schedule::BringForward(&blockRedBlack);
            //-}

            Resource::VectorBlock::iterator it = Resource::Schedule::Next(); 
          
            Resource::Block *b = ((Resource::Block*)(*it));

            std::cout << " {x} Loading '" << b->Name() << "'..." << std::endl;
          
            b->Load(blockThreading, resourceThreading);
        }
    } catch(std::string & msg) 
    {
        std::cout << " {x} Done: " << msg << std::endl;
    }

    //
    // Waits for finishing of all Blocks and gather duration values.
    // 
    Resource::Schedule::Join();

  clock_t t2 = clock();

    Resource::MapDurations durations;
    Resource::Schedule::GatherDurations(durations);

    //
    // Prints the duration values of all Blocks.
    //
    Resource::MapDurations::iterator it = durations.begin();
    for(; it != durations.end(); it++)
    {
        Resource::Block *b = (Resource::Block*) it->first;

        std::cout << " {x} Block " << b->Name() << ": "
                  << b->Bytes() << " bytes in " << it->second << " msec " << std::endl;
    }

    std::cout << " {x} Total: " << Resource::__swatch(t1, t2) << " msec" << std::endl;
}

/** @brief
 *************************************************************/
void setupBlocks()
{
    blockSingleSprites.AddResource("PackageSingleSprites", Resource::Resource("gfx/b1/01.png"));
    blockSingleSprites.AddResource("PackageSingleSprites", Resource::Resource("gfx/b1/02.png"));
    blockSingleSprites.AddResource("PackageSingleSprites", Resource::Resource("gfx/b1/03.png"));
    blockSingleSprites.AddResource("PackageSingleSprites", Resource::Resource("gfx/b1/04.png"));

    blockGame.AddResource("PackageGame", Resource::Resource("gfx/b2/01.png"));
    blockGame.AddResource("PackageGame", Resource::Resource("gfx/b2/02.png"));
    blockGame.AddResource("PackageGame", Resource::Resource("gfx/b2/03.png"));
    blockGame.AddResource("PackageGame", Resource::Resource("gfx/b2/04.png"));
    blockGame.AddResource("PackageGame", Resource::Resource("gfx/b2/05.png"));
    blockGame.AddResource("PackageGame", Resource::Resource("gfx/b2/06.png"));

    blockFeature.AddResource("PackageFeature", Resource::Resource("gfx/b3/01.png"));
    blockFeature.AddResource("PackageFeature", Resource::Resource("gfx/b3/02.png"));
    blockFeature.AddResource("PackageFeature", Resource::Resource("gfx/b3/03.png"));
    blockFeature.AddResource("PackageFeature", Resource::Resource("gfx/b3/04.png"));
    blockFeature.AddResource("PackageFeature", Resource::Resource("gfx/b3/05.png"));
    blockFeature.AddResource("PackageFeature", Resource::Resource("gfx/b3/06.png"));
    blockFeature.AddResource("PackageFeature", Resource::Resource("gfx/b3/07.png"));

    blockRedBlack.AddResource("PackageRedBlack", Resource::Resource("gfx/b4/01.png"));
    blockRedBlack.AddResource("PackageRedBlack", Resource::Resource("gfx/b4/02.png"));
}

