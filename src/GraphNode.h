#include "Model.h"
#include "Transform.h"
#include <vector>

#ifndef OPENGLGP_GRAPHNODE_H
#define OPENGLGP_GRAPHNODE_H

class GraphNode {
public:
    Model* model;
    Transform* transform;
    bool instanced;
    GraphNode* parent;
    bool render = true;

    GraphNode(Model *model, Transform *transform, bool instanced) : model(model), transform(transform),
                                                                    instanced(instanced) {
        this->parent = nullptr;
    }

    void addChild(GraphNode* child) {
        children.push_back(child);
        child->parent = this;
    }

    //huh
    void draw(Shader &shader, float deltaTime) {
        if(parent) {
            shader.setMat4("model", this->transform->getCombinedModel(this->parent->transform->getWorldModel()));
        }
        if(model && render) {
            this->model->Draw(shader);
        }

        for(auto child : children) {
            child->draw(shader, deltaTime);
        }

    }

private:
    bool light;
    std::vector<GraphNode*> children;

};

#endif //OPENGLGP_GRAPHNODE_H
