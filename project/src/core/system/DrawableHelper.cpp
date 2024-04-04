#include "DrawableHelper.h"
#include "../../util/Debug.h"

Material *DrawableHelper::create_material(std::string name, ShaderProg shaderProg, vec3 albedo, vec4 specular)
{
    Material *m = new Material;
    m->albedo = albedo;
    m->specular = specular;
    m->shaderProg = shaderProg;
    m->textureBitmask = 0;
    return m;
}

Material *DrawableHelper::attach_texture_to_material(Material *m, int idx, std::string textureName)
{
    if (m == nullptr || idx < 0 || idx >= MAX_TEXTURES || textureName == "")
    {
        debug_error("Invalid material/idx/texture.\n");
        return m;
    }

    m->textures[idx] = textureName;
    m->textureBitmask = m->textureBitmask | (1 << idx);
    return m;
}

Drawable *DrawableHelper::create_from_model(std::string path, Transform t)
{
    Drawable *ret = new Drawable;
    ret->setModel(LoadModel((MODELS_PATH + path).c_str()));
    ret->setTransform(t);
    return ret;
}

// Creates a UV sphere with given latitude and longitude
// minimum 4 for longi and lat, recommended 64
//referenced from https://gist.github.com/Pikachuxxxx/5c4c490a7d7679824e0e18af42918efc
Drawable *DrawableHelper::create_sphere(Transform t, int lat, int longi)
{
    // used c-style malloc for compatibility
    int numVert = (lat + 1) * (longi + 1);
    int numInd = numVert * 6;
    vec3 *vertices = (vec3 *)malloc(sizeof(vec3) * numVert);
    vec3 *normals = (vec3 *)malloc(sizeof(vec3) * numVert);
    vec2 *texCoords = (vec2 *)malloc(sizeof(vec2) * numVert);
    GLuint *indices = (GLuint *)malloc(sizeof(GLuint) * numInd);

    int v = 0;
    float deltaLat = M_PI / (float)lat;
    float deltaLongi = 2 * M_PI / (float)longi;
    float latAngle, longiAngle;
    for (int i = 0; i <= lat; ++i)
    {
        latAngle = M_PI / 2.0f - i * deltaLat; // from pi/2 to -pi/2
        float xy = cosf(latAngle);
        float z = sinf(latAngle);
        for (int j = 0; j <= longi; ++j)
        {
            longiAngle = j * deltaLongi;
            vertices[v] = vec3(xy * cosf(longiAngle), xy * sinf(longiAngle), z);
            texCoords[v] = vec2((float)j / longi, (float)i / lat);
            normals[v] = vertices[v]; // no need normalize as radius is 1
            ++v;
        }
    }

    GLuint k1, k2;
    v = 0;
    for (int i = 0; i < lat; ++i)
    {
        k1 = i * (longi+1);
        k2 = k1 + (longi + 1);
        for (int j = 0; j < longi; ++j)
        {
            if (i != 0)
            {
                indices[v++] = k1;
                indices[v++] = k2;
                indices[v++] = k1 + 1;
            }
            
            if (i != lat-1)
            {
                indices[v++] = k1 + 1;
                indices[v++] = k2;
                indices[v++] = k2 + 1;
            }
            ++k1; ++k2;
        }
    }

    Drawable *ret = new Drawable;
    Model *m = LoadDataToModel(vertices, normals, texCoords, NULL, indices, numVert, numInd);
    ret->setModel(m);
    ret->setTransform(t);

    free(vertices);
    free(normals);
    free(texCoords);
    free(indices);
    return ret;
}

Drawable *DrawableHelper::create_cube(Transform t)
{
    // todo bring cube code from lab here
    return nullptr;
}

Drawable *DrawableHelper::create_plane(Transform t)
{
    // todo bring ground code from lab here
    // vec3 vertices[] =
    //     {{-groundSize, 0.0f, -groundSize},
    //      {-groundSize, 0.0f, groundSize},
    //      {groundSize, -0.0f, -groundSize},
    //      {groundSize, -0.0f, groundSize}};
    // vec3 normals[] =
    //     {{0.0f, 1.0f, 0.0f},
    //      {0.0f, 1.0f, 0.0f},
    //      {0.0f, 1.0f, 0.0f},
    //      {0.0f, 1.0f, 0.0f}};
    // vec2 texCoords[] =
    //     {{0.0f, 0.0f},
    //      {0.0f, uvTiles},
    //      {uvTiles, 0.0f},
    //      {uvTiles, uvTiles}};
    // GLuint indices[] = {0, 1, 2, 1, 3, 2};

    // Model *m = LoadDataToModel(vertices, normals, texCoords, NULL, indices, 4, 6);
    return nullptr;
}
