/*
 * Copyright (C) 2012
 * Christian Benjamin Ries <mail@christianbenjaminries.de>
 * 
 * Date: 25th November 2012
 * License: I don't care!
 */
// 
#include "ResourceBlock.h"

// Utilities
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// C++
#include <iostream>
#include <algorithm>

// Boost
#include <boost/thread.hpp> 
#include <boost/thread/future.hpp>

// external
#include <Magick++.h>

/** @brief
 *************************************************************/
namespace Resource
{


float __swatch(clock_t start, clock_t end)
{
	return (((float)end - (float)start) / 1000000.0F ) * 1000;
}


/*
 */


ResourceLoader::ResourceLoader()
  : mDuration(0.0f)
{
    // nothing to do
}


ResourceLoader::ResourceLoader(const ResourceLoader & loader)
{
    mDuration = loader.mDuration;
}


ResourceLoader::~ResourceLoader()
{
    // nothing to do
}


void ResourceLoader::Do(std::string resourceName) 
{
    try
    {
        mDuration = 0.0f;

        // Following implementation has to be
        // replaced by other loading techniques.

        // Magick++ is not initialised, i.e. following call is
        // removed within the main-function:
        //  > Magick::InitializeMagick(*argv);

        Magick::Image image;

        clock_t t1 = clock();
        image.read(resourceName);
        clock_t t2 = clock();

        mDuration = __swatch(t1, t2);

    } catch(Magick::Exception & error) {
        std::cout << " {m} Loading failed: " << error.what() << std::endl;
    }
}


float ResourceLoader::Duration()
{
    return mDuration;
}

/*
 */

Resource::Resource(const std::string & name, PRIORITY priority)
  : mName(name),
    mPriority(priority)
{
    // nothing to do
}


Resource::Resource(const Resource & resource)
{
    mName = resource.mName;
    mPriority = resource.mPriority;
}


Resource::~Resource()
{
    // nothing to do
}


/*
 */

Block::Block(const std::string & name, PRIORITY priority)
  : mName(name),
    mPriority(priority),
    mDone(false),
    mStarted(false),
    mWorkerThread(NULL),
    mDuration(0.0f)
{
    // nothing to do
}


Block::Block(const Block & block)
{
    mName = block.mName;
    mPriority = block.mPriority;
    mResources = block.mResources;
    mDone = block.mDone;
    mStarted = block.mStarted;
    mWorkerThread = block.mWorkerThread;
    mDuration = block.mDuration;
}


Block::~Block()
{
    if(mWorkerThread) 
    {
        delete mWorkerThread;
        mWorkerThread = NULL;
    }
}


void Block::AddResource(
    const std::string & packageName,
    const std::string & resourceName,
    PRIORITY priority
) {
    Resource resource(resourceName, priority);
    mResources[packageName].push_back(resource);
}


void Block::AddResource(
    const std::string & packageName,
    Resource resource
) {
    mResources[packageName].push_back(resource);
}


std::string Block::Name() const 
{ 
    return mName; 
}


PRIORITY Block::Priority() const 
{ 
    return mPriority;
}


void Block::SetPriority(PRIORITY priority) 
{ 
    mPriority = priority; 
}


void Block::SetPriority(int priority) 
{ 
    mPriority = (PRIORITY) priority; 
}


float Block::Duration() const 
{ 
    return mDuration;
}


uint Block::Count()
{
    int size = 0;

    std::map<std::string, std::vector<Resource> >::iterator it = mResources.begin();
    for(; it != mResources.end(); it++)
    {
        size += it->second.size();
    }

    return size;
}


uint Block::Bytes()
{
    uint total = 0;

    std::map<std::string, std::vector<Resource> >::iterator it = mResources.begin();
    for(; it != mResources.end(); it++) 
    {
        std::vector<Resource> r = it->second;

        for(uint i=0; i<r.size(); i++)
        {
            struct stat filestatus;
            stat(r.at(i).Name().c_str(), &filestatus);

            total += filestatus.st_size;
        }
    }

    return total;
}


void Block::Load(
    bool useThreadingForBlock,
    bool useThreadingForResources
) {

    if(useThreadingForBlock)
    {
        __Load();

    } else {
        (useThreadingForResources 
            ? _LoadResourcesThreaded()
            : _LoadResourcesSequencial()
        );
    }
}


void Block::__Load()
{
    boost::function<void()> th_func;
    
    th_func = boost::bind(&Block::__LoadBlockThreaded, this);
    mWorkerThread = new boost::thread(th_func);

    mStarted = true;
}


void Block::__LoadBlockThreaded()
{
    float duration = 0.0f;

    std::map<std::string, std::vector<Resource> >::iterator it = mResources.begin();
    for(; it != mResources.end(); it++) 
    {
        std::vector<Resource> r = it->second;

        for(uint i=0; i<r.size(); i++)
        {
            ResourceLoader loader;
            loader.Do(r.at(i).Name());
            duration += loader.Duration();
        }
    }

    mDone = true;

    mDuration = duration;
}


void Block::Join()
{
    if(mWorkerThread)
    {
        mWorkerThread->join();
    }
}


void Block::_LoadResourcesThreaded()
{
    int count = Count();

    ResourceLoader worker[count];

    boost::function<void (std::string)> th_func[count];
    boost::thread *workerThreads[count];

    int resourceCounter = 0;

    std::map<std::string, std::vector<Resource> >::iterator it = mResources.begin();
    for(; it != mResources.end(); it++) 
    {
        std::vector<Resource> r = it->second;

        for(uint i=0; i<r.size(); i++)
        {
            th_func[resourceCounter] = boost::bind(&ResourceLoader::Do, &worker[resourceCounter], _1);
               
            workerThreads[resourceCounter] = new boost::thread(th_func[resourceCounter], r.at(i).Name());

            resourceCounter++;
        }
    }

    float duration = 0.0f;

    for(uint i=0; i<count; i++) 
    {
        if(workerThreads[i]) 
        {
            workerThreads[i]->join();

            delete workerThreads[i]; 
            workerThreads[i] = NULL;

            duration += worker[i].Duration();
        }
    }

    mDone = true;

    mDuration = duration;
}


void Block::_LoadResourcesSequencial()
{
    float duration = 0.0f;

    std::map<std::string, std::vector<Resource> >::iterator it = mResources.begin();
    for(; it != mResources.end(); it++) 
    {
        std::vector<Resource> r = it->second;

        for(uint i=0; i<r.size(); i++)
        {
            ResourceLoader worker;
            worker.Do(r.at(i).Name());
            duration += worker.Duration();
        }
    }

    mDone = true;

    mDuration = duration;
}


bool Block::Done()
{
    return mDone;
}


bool Block::Started()
{
    return mStarted;
}

/*
 */

VectorBlock Schedule::mBlocks = VectorBlock();
VectorBlock Schedule::mBlocksNext = VectorBlock();

/** Ascending ordering based on Block's priority.
 */
bool __compareBlocks(const Block* b1, const Block* b2) 
{ 
    Block *bb1 = (Block*) b1;
    Block *bb2 = (Block*) b2;

    return (bb1->Priority() > bb2->Priority());
}


VectorBlock::iterator Schedule::Next()
{
    if(mBlocksNext.size())
    {
        VectorBlock::iterator e = mBlocksNext.end();
        
        --e; 
        
        mBlocksNext.pop_back();
        
        return e;
    }

    std::sort(mBlocks.begin(), mBlocks.end(), __compareBlocks);

    VectorBlock::iterator it = mBlocks.begin();
    for(; it != mBlocks.end(); it++)
    {
        Block *b = ((Block*)(*it));

        if(!b->Done() && !b->Started()) 
        {
            return it;
        }
    }

    throw std::string("Schedule::Next has no next element");
}


void Schedule::AddBlockRef(const Block * ref)
{
    mBlocks.push_back(ref);
}


void Schedule::BringForward(const Block * ref)
{
    if(!ref) return;

    Block *b = (Block*) ref;

    if(b->Done()) return;
    if(b->Started()) return;

    mBlocksNext.push_back(b );
}

void Schedule::Join()
{
    VectorBlock::iterator it = mBlocks.begin();
    for(; it != mBlocks.end(); it++)
    {
        Block *b = ((Block*)(*it));

        b->Join();
    }
}

void Schedule::GatherDurations(MapDurations & mapstore)
{
    VectorBlock::iterator it = mBlocks.begin();
    for(; it != mBlocks.end(); it++)
    {
        Block *b = ((Block*)(*it));

        mapstore[b] = b->Duration();
    }
}

} // namespace Resource
