#include "objectloader.h"
#include <QFile>
#include <iostream>
#include <regex>
#include <QTextStream>

#include "line3d.h"

bool ObjectLoader::loadObjectFromFile(const char* filename, Mesh* object)
{
    if(object == nullptr)
        return false;

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    std::regex regex("\\s+");

    QTextStream in(&file);

    bool normalData = false;
    std::vector<float> normalsNotSorted; // Normals which are not mapped to the lines
    std::vector<float> normals;
    std::vector<unsigned> normalsIndices;

    while(!in.atEnd())
    {
        std::string line = in.readLine().toStdString();

        std::sregex_token_iterator it(line.begin(),line.end(),regex,-1);
        std::sregex_token_iterator end;
        std::string type = *it;
        ++it;

        if(type == "o")
            for(;it != end; ++it)
                if(it->str() == "Normals")
                {
                    normalData = true;
                    object->setHasNormals(normalData);
                }
                else
                    normalData = false;
        else if(type == "v")
            for(;it != end; ++it)
            {
               if(!normalData)
                   object->vertices.push_back((float)std::atof((it->str().c_str())));
               else
                   normalsNotSorted.push_back((float)std::atof((it->str().c_str())));
            }
        else if(type == "l")
            for(;it != end; ++it)
            {
                if(!normalData)
                    object->indices.push_back((unsigned int)std::atoi((it->str().c_str()))-1);
                else
                    normalsIndices.push_back((unsigned)std::atoi((it->str().c_str()))-1);
            }
    }

    file.close();

    // compute normals and create a Line class
    float x,y,z;
    float x1,y1,z1;
    float x2,y2,z2;
    float xn,yn,zn;
    int normalCount; // every edge needs 2 normals. If only one is found, the second is 0,0,0 by default
    glm::vec3 n;
    bool secondaryN;
    float eps = 0.001f;
    Line3D* l;
    unsigned int index = 1;
    for(unsigned int v=5; v<object->vertices.size(); v+=6, index+=2)
    {
        normalCount = 0;
        secondaryN = false;

        l = new Line3D();

        x2 = object->vertices[v-2];
        y2 = object->vertices[v-1];
        z2 = object->vertices[v  ];

        x1 = object->vertices[v-5];
        y1 = object->vertices[v-4];
        z1 = object->vertices[v-3];

        l->setStartEnd(cv::Point3f(x1,y1,z1),cv::Point3f(x2,y2,z2));
        l->setIndices(cv::Vec2i(object->indices[index-1],object->indices[index]));

        x = x2-x1;
        y = y2-y1;
        z = z2-z1;

        // Center of Edge
        x = x1 + x/2.f;
        y = y1 + y/2.f;
        z = z1 + z/2.f;

        l->setCenter(glm::vec3(x,y,z));

        // Search for normals corresponding to that edge
        for(unsigned int i=5; i<normalsNotSorted.size(); i+=6)
        {
            xn = normalsNotSorted[i-5];
            yn = normalsNotSorted[i-4];
            zn = normalsNotSorted[i-3];

            if((x >= xn - eps && x <= xn + eps) &&
               (y >= yn - eps && y <= yn + eps) &&
               (z >= zn - eps && z <= zn + eps))
            {
                // Normal found
                normalCount++;
                xn = normalsNotSorted[i-2] - xn;
                yn = normalsNotSorted[i-1] - yn;
                zn = normalsNotSorted[i]   - zn;

                n = glm::normalize(glm::vec3(xn,yn,zn));

                // Check the 2 different kinds of normals here
                if(!secondaryN)
                {
                    secondaryN = true;

                    l->setNormal(glm::vec3(n[0],n[1],n[2]));
                }
                else
                {
                    l->setNormalSec(glm::vec3(n[0],n[1],n[2]));
                }

            }
        }
        // If the Edge has only one normal (a texture edge for instance), create a default 0,0,0 secondary normal
        if(normalCount != 2)
            for(int k=0;k<6;k++)
            {
              l->setNormalSec(glm::vec3(0.f,0.f,0.f));
            }

        object->lines.push_back(l);
    }

    return true;
}

ObjectLoader::ObjectLoader()
{
}

