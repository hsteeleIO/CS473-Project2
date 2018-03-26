#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
    public:
        Camera();

        glm::mat4 genViewMatrix();

        void setPos(glm::vec3 newPos);
        void setLookAt(glm::vec3 newLook);
        void setRollPitchYaw(glm::vec3 newRPY);
        void changeRollPitchYawBy(glm::vec3 deltaRPY);

        void goAlong(float amt, glm::vec3 forwardVector);
        void goForward(float amt);
        void strafeRight(float amt);
        void goUp(float amt);

        void freeLook();
        void lockOn();
        void lockOnTo(glm::vec3 lockPos, float dist);
        void lockOnToCat(glm::vec3 lockPos, float dist);

        glm::vec3 getPos();
        glm::vec3 getLookAt();
        bool getFreeCam();


    protected:

    private:
        bool freeCam;
        double lockCamDist;

        void updateAxisFromRPY();
        void updateAxisFromLookAt();
        void calcApproxLookUp();

        void updatePosFromLock();

        glm::vec3 position;
        glm::vec3 lookAt;
        glm::vec3 rollPitchYaw;

        glm::vec3 approxLookUp;
        glm::vec3 cameraX;
        glm::vec3 cameraY;
        glm::vec3 cameraZ;

};

#endif // CAMERA_H
