#ifndef OPENGLGP_MODEL_H
#define OPENGLGP_MODEL_H

#include "Mesh.h"

enum Type {
    PLANE,
    CUBE,
    ROOF,
    LIGHT_DIR,
    LIGHT_POINT
};

class Model {
public:
    Mesh* mesh;
    std::vector<glm::vec3> translations;

    Model(Type type, std::vector<glm::vec3>& translations, bool instanced = false) {

        this->translations = translations;
        this->instanced = instanced;

        switch (type) {
            case PLANE:
                setupPlane();
                break;

            case CUBE:
                setupCube();
                break;

            case ROOF:
                setupRoof();
                break;

            case LIGHT_DIR:
                setupDirectional();
                break;

            case LIGHT_POINT:
                setupPoint();
                break;
        }
    }

    void Draw(Shader &shader) {
        if(this->mesh) {
            this->mesh->Draw(shader);
        }
    }

private:
    bool instanced;

    void setupPlane() {
        float width = 700.0f;
        std::vector<Vertex> vertices = {
                {glm::vec3(-width, -1.0f, width),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(-width, -1.0f, -width), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 50.0f)},
                {glm::vec3(width, -1.0f, -width),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(50.0f, 50.0f)},

                {glm::vec3(width, -1.0f, width),   glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(50.0f, 0.0f)},
                {glm::vec3(-width, -1.0f, width),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(width, -1.0f, -width),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(50.0f, 50.0f)}
        };

        Texture *texture = new Texture("./res/textures/grass.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);

        this->mesh = new Mesh(vertices, texture, translations);
    }

    void setupCube() {

        Texture *texture = new Texture("./res/textures/wall.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);

        this->mesh = new Mesh(buildCube(), texture, translations, instanced);

    }

    void setupRoof() {

        Texture *texture = new Texture("./res/textures/roof.jpg",  GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);

        this->mesh = new Mesh(buildPyramid(), texture, translations, instanced);
    }

    void setupDirectional() {
        Texture *texture = new Texture("./res/textures/light_tex.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
        this->mesh = new Mesh(buildPyramid(), texture, translations);
    }

    void setupPoint() {
        Texture *texture = new Texture("./res/textures/light_tex.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
        this->mesh = new Mesh(buildCube(), texture, translations);
    }

    std::vector<Vertex> buildCube() {

        std::vector<Vertex> vertices = {
                //front
                {glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},

                {glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},

                //back
                {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},

                {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
                {glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},

                //down
                {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                {glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},

                {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

                //up
                {glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},

                {glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},

                //left
                {glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                {glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},

                {glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},

                //right
                {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},

                {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                {glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
        };

        return vertices;
    }

    std::vector<Vertex> buildPyramid() {
        std::vector<Vertex> vertices = {
                //base
                {glm::vec3(-1.5f, 1.0f, -1.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(1.5f, 1.0f, -1.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                {glm::vec3(1.5f, 1.0f, 1.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},

                {glm::vec3(-1.5f, 1.0f, -1.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
                {glm::vec3(-1.5f, 1.0f, 1.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
                {glm::vec3(1.5f, 1.0f, 1.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},

                //front
                {glm::vec3(-1.5f, 1.0f, -1.5f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.5f, 1.0f, -1.5f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.0f, 2.5f, 0.0f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(0.5f, 1.0f)},

                //back
                {glm::vec3(-1.5f, 1.0f, 1.5f), glm::vec3(0.0f, 0.5f, 0.8f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.5f, 1.0f, 1.5f), glm::vec3(0.0f, 0.5f, 0.8f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.0f, 2.5f, 0.0f), glm::vec3(0.0f, 0.5f, 0.8f), glm::vec2(0.5f, 1.0f)},

                //left
                {glm::vec3(-1.5f, 1.0f, 1.5f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(-1.5f, 1.0f, -1.5f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.0f, 2.5f, 0.0f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec2(0.0f, 1.0f)},

                //right
                {glm::vec3(1.5f, 1.0f, -1.5f), glm::vec3(0.8f, 0.5f, 0.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.5f, 1.0f, 1.5f), glm::vec3(0.8f, 0.5f, 0.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(0.0f, 2.5f, 0.0f), glm::vec3(0.8f, 0.5f, 0.0f), glm::vec2(0.5f, 1.0f)}
        };

        return vertices;
    }

};

#endif //OPENGLGP_MODEL_H
