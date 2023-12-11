#pragma once
#include <btBulletDynamicsCommon.h>
//from Brian
// this is a support class to get return data to the game status, it won't have much extra info, so its safe in here.
//The only thing we really want is the worldTransform of the MotionState which supplies us with chunkPos and orientation
// data to allow us to render objects 
class GameValues : public btDefaultMotionState
{
public:
    GameValues(const btTransform& transform) // set up with our transform matrix
        : btDefaultMotionState(transform)
    {
    }


    void GetWorldTransform(btScalar* transform)
    {
        btTransform l_transform; // create a local bullet transform
        getWorldTransform(l_transform); // load it with the World Transform for this object
        l_transform.getOpenGLMatrix(transform); // convert to OpenGL format and store in the address provided
    }
};
