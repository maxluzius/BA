#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H

#include "mesh.h"

/**
 * @brief The ObjectLoader class
 * Loads an Object
 */
class ObjectLoader
{
public:
    /**
     * @brief loadObjectFromFile load an object from file
     * @param filename
     * @param object the loaded mesh
     * @return
     */
    static bool loadObjectFromFile(const char* filename, Mesh* object);
protected:
    /**
     * @brief ObjectLoader ctr
     */
    ObjectLoader();

};

#endif // OBJECTLOADER_H
