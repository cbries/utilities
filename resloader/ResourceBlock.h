/*
 * Copyright (C) 2012 Christian Benjamin Ries
 * Christian Benjamin Ries <mail@christianbenjaminries.de>
 * 
 * Date: 25th November 2012
 * License: MIT
 */
#ifndef __RESOURCEBLOCK_H__
#define __RESOURCEBLOCK_H__

// C++
//#include <iostream>
#include <string>
#include <vector>
#include <map>

// Boost
#include <boost/thread.hpp>

// Forward-declaration
namespace Resource 
{ 
    class Block;
    class Resource;

    typedef std::map<const Block*, float> MapDurations;
    typedef std::vector<const Block*> VectorBlock;
}

//
// Typedefs
//
typedef unsigned int uint;

/** @brief
 *************************************************************/
namespace Resource
{

/// Function for displaying the stopwatch time.
float __swatch(clock_t start, clock_t end);

/// Priority classifications
enum PRIORITY
{
    VeryLow=0, Low,
    Normal,
    High, VeryHigh
};

/** @brief
 *************************************************************/
class ResourceLoader
{
public:
    ResourceLoader();
    ResourceLoader(const ResourceLoader & loader);
    ~ResourceLoader();

    /// The resource loading function. This parts has to 
    /// be replaced by other functionalities, e.g. rman.Load(...).
    void Do(std::string resourceName);

    /// Returns the taken milliseconds for loading the individual resource.
    float Duration();

private:
    /// Loading progress's taken milliseconds.
    float mDuration;
};


/** @brief
 *************************************************************/
class Resource
{
public:
    Resource() { /* nothing to do */ }
    Resource(const std::string & name, PRIORITY priority=Normal);
    Resource(const Resource & resource);
    ~Resource();

    std::string Name() const { return mName; }
    PRIORITY Priority() { return mPriority; }

private:
    std::string mName;
    PRIORITY mPriority;
};


/** @brief
 *************************************************************/
class Block
{
public:
    Block(const std::string & name, PRIORITY priority=Normal);
    Block(const Block & block);
    ~Block();

    void AddResource(
        const std::string & packageName,
        const std::string & resourceName,
        PRIORITY priority = Normal
    );

    void AddResource(
        const std::string & packageName,
        Resource resource
    );

    std::string Name() const;

    PRIORITY Priority() const;
    void SetPriority(PRIORITY priority);
    void SetPriority(int priority);

    /** @brief Counts the resources of this Block.
     * @return Count of resources.
     */
    uint Count();

    /** @brief Retrieves the total byte of all resources of this Block.
     * @return Total bytes of all resources.
     */
    uint Bytes();

    /** @brief Loads the resources of this Block.
     * 
     *
     * @param useThreadingForResources TRUE will enable boost::thread for all resources, 
     *                                 otherwise sequencial loading is used.
     * @see __Load
     * @see __LoadBlockThreaded
     * @see _LoadResourcesThreaded
     * @see _LoadResourcesSequencial
     */
    void Load(bool useThreadingForBlock=false, bool useThreadingForResources=false);

private:
    /// Starts Block's resource loading, i.e. initiate a thread and calls __LoadBlockThreaded.
    void __Load();

    /// Loads Block's resources in a thread.
    void __LoadBlockThreaded();

    /// Loads the resources, i.e. for each resource a single thread is created.
    void _LoadResourcesThreaded();

    /// Loads the resources sequencial.
    void _LoadResourcesSequencial();

public:
    /** @brief Returns the current state of this block.
     * @return TRUE when the block is already loaded, otherwise FALSE.
     */
    bool Done();

    /** @brief Returns the current state, i.e. if it is progressing.
     * @return TRUE when the Block is in progress, otherwise FALSE.
     */
    bool Started();
    
private:
    /// Name of this Block.
    std::string mName;

    /// Priority of this Block.
    PRIORITY mPriority;

    /// mFiles[ package name ] = ressource name
    std::map<std::string, std::vector<Resource> > mResources;

    /// TRUE when the Block is already loaded, otherwise FALSE.
    bool mDone;

    /// TRUE when the Block is handled within a thread which has been started,
    /// otherwise FALSE.
    bool mStarted;

    /// Pointer for the internal Block-threading algorithm.
    boost::thread *mWorkerThread;

    /// Keeps the msecs for the whole loading process.
    float mDuration;

public:
    /// Returns the msecs of the loading process.
    float Duration() const;

public:
    /// Waits until the Block-thread has been finished.
    void Join();
};


/** @brief
 *************************************************************/
class Schedule
{
public:
    /// Returns the next Block which is planned for execution.
    static VectorBlock::iterator Next();

    /// Adds a Block which has be executed.
    static void AddBlockRef(const Block * ref);

    /// Brings one Block to the front, i.e. 
    /// will be loaded after the next Next() call.
    static void BringForward(const Block * ref);

    /// Waits until all Blocks are loaded.
    static void Join();

    /// Gather the durations of all individual Blocks.
    static void GatherDurations(MapDurations & mapstore);

private:
    /// 
    static VectorBlock mBlocks;

    /// Blocks which are more prioritized, i.e.
    /// they are brought forward by a call of BringForward().
    /// @see BringForward
    static VectorBlock mBlocksNext;
};

} // namespace Resource

#endif // __RESOURCEBLOCK_H__
