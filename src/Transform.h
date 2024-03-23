//
// Created by fujitsu on 24.11.2022.
//

#ifndef OPENGLGP_TRANSFORM_H
#define OPENGLGP_TRANSFORM_H

class Transform {

public:
    std::vector<glm::vec3> translations;

    Transform(glm::vec3 rotS, glm::vec3 pos, glm::vec3 sc) {
        this->rotationSelf = rotS;
        this->position = pos;
        this->scale = sc;

        localModel = glm::translate(localModel, position);
        localModel = glm::scale(localModel, scale);

        localModel = glm::rotate(localModel, rotS.x, glm::vec3(1.0f, 0.0f, 0.0f));
        localModel = glm::rotate(localModel, rotS.y, glm::vec3(0.0f, 1.0f, 0.0f));
        localModel = glm::rotate(localModel, rotS.z, glm::vec3(0.0f, 0.0f, 1.0f));
//        worldModel = localModel;
        initModel = localModel;
    }

    glm::mat4 getWorldModel() {
        return worldModel;
    }

    glm::mat4 getInitModel() {
        return initModel;
    }

//    glm::mat4 getLocalModel(float deltaTime) {
//        updateLocal(deltaTime);
//        return localModel;
//    }

    glm::mat4 getCombinedModel(glm::mat4 parentModel) {
        worldModel = parentModel * localModel;
        return worldModel;
    }

    const glm::mat4 &getLocalModel() const {
        return localModel;
    }

    void constRotation(float deltaTime) {
        localModel = glm::rotate(localModel, rotationSelf.x * deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
        localModel = glm::rotate(localModel, rotationSelf.y * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
        localModel = glm::rotate(localModel, rotationSelf.z * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));

    }

    glm::vec3 getPosition() {
        return position;
    }

    void setPosition(glm::vec3 newPos) {
        position = newPos;
    }

    void showDirection(glm::vec3 angle, glm::vec3 sc) {
        localModel = glm::mat4(1.0f);
        //localModel = glm::translate(localModel, -position);

        localModel = glm::translate(localModel, position);

        localModel = glm::rotate(localModel, angle.x, glm::vec3(1.0f, 0.0f, 0.0f));
        localModel = glm::rotate(localModel, angle.y, glm::vec3(0.0f, 1.0f, 0.0f));
        localModel = glm::rotate(localModel, angle.z, glm::vec3(0.0f, 0.0f, 1.0f));

        //localModel = glm::rotate(localModel, rotS.x, glm::vec3(1.0f, 0.0f, 0.0f));
        localModel = glm::scale(localModel, sc);
    }

    void setTranslations(int amount) {
        int index = amount * 5;
        for(int y = -index + (int)position.z; y < index + (int)position.z; y += 10)
        {
            for(int x = -index + (int)position.x; x < index + (int)position.x; x += 10)
            {
                glm::vec3 translation;
                translation.x = (float)x / 1.5f;
                translation.y = 0.0f;
                translation.z = (float)y / 1.5f;
                translations.push_back(translation);
            }
        }
    }

    void setScale(glm::vec3 sc) {
        localModel = glm::scale(glm::mat4(1.0f), sc);
    }


private:
    glm::mat4 localModel = glm::mat4(1.0f);
    glm::mat4 worldModel = glm::mat4(1.0f);
    glm::mat4 initModel = glm::mat4(1.0f);

    glm::vec3 rotationSelf;
    glm::vec3 position;
    glm::vec3 scale;


};

#endif //OPENGLGP_TRANSFORM_H
