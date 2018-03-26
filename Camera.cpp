#include "Camera.h"
#include "oGL_Tools.h"

#include <iostream>

Camera::Camera()
{
    this->position = glm::vec3(0.0, 0.0, 3.0);
    this->lookAt = glm::vec3(0.0, 0.0, 0.0);
    this->rollPitchYaw = glm::vec3(0.0, 0.0, 0.0);
    this->freeCam = false;
    this->lockCamDist = 6.0;
    updateAxisFromLookAt();
}

glm::mat4 Camera::genViewMatrix() {
    this->updateAxisFromLookAt();
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(this->position, this->lookAt, this->approxLookUp);
    //std::cout << "Pos: (" << position.x << ", " << position.y << ", " << position.z << ")  ";
    //std::cout << "LkAt:(" << lookAt.x << ", " << lookAt.y << ", " << lookAt.z << ")\r";
    return view;
}

void Camera::goForward(float amt) {
    if (freeCam) goAlong(amt, -this->cameraZ);
    else {
        this->lockCamDist = this->lockCamDist - amt;
        this->updatePosFromLock();
    }
}

void Camera::strafeRight(float amt) {
    if (freeCam) goAlong(amt, this->cameraX);
    else this->changeRollPitchYawBy(glm::vec3(0.0, 0.0, amt));
}

void Camera::goUp(float amt) {
    if (freeCam) goAlong(amt, this->cameraY);
    else this->changeRollPitchYawBy(glm::vec3(0.0, amt, 0.0));
}

void Camera::goAlong(float amt, glm::vec3 forwardVector) {
    glm::vec3 deltaPos = forwardVector * amt;
    glm::vec3 newPos = this->position + deltaPos;
    if (newPos.y < 0.0) setPos(glm::vec3(newPos.x, 0.0, newPos.z));
    else setPos(this->position + deltaPos);
    setLookAt(this->lookAt + deltaPos);
}

void Camera::calcApproxLookUp() {
    float roll = glm::radians(this->rollPitchYaw.x);
    this->approxLookUp = glm::vec3(sin(roll), cos(roll), 0.0);
}

void Camera::updateAxisFromLookAt() {

    calcApproxLookUp();
    this->cameraZ = glm::normalize(this->position - this->lookAt);
    this->cameraX = glm::normalize(glm::cross(this->approxLookUp, this->cameraZ));
    this->cameraY = glm::cross(this->cameraZ, this->cameraX);
}



void Camera::freeLook() {
    this->freeCam = true;
}

void Camera::lockOn() {
    this->freeCam = false;
}

void Camera::lockOnTo(glm::vec3 lockPos, float dist) {
    this->freeCam = false;
    this->lookAt = lockPos;
    this->lockCamDist = dist;
}

void Camera::lockOnToCat(glm::vec3 lockPos, float dist) {
    this->freeCam = false;
    this->lookAt = lockPos;
    this->lockCamDist = dist;
}

void Camera::updatePosFromLock() {
    float roll = glm::radians(this->rollPitchYaw.x);
    float pitch = glm::radians(this->rollPitchYaw.y);
    if (pitch < 0) pitch = 0;
    float yaw = glm::radians(this->rollPitchYaw.z);

    float xAmt = sin(yaw) * cos(roll) * this->lockCamDist;
    float yAmt = sin(pitch) * cos(roll) * this->lockCamDist;
    float zAmt = cos(pitch) * cos(yaw) * this->lockCamDist;

    this->position = this->lookAt + glm::vec3(xAmt, yAmt, zAmt);
    updateAxisFromLookAt();
}

void Camera::updateAxisFromRPY() {
    float roll = glm::radians(this->rollPitchYaw.x);
    float pitch = glm::radians(this->rollPitchYaw.y);
    float yaw = glm::radians(this->rollPitchYaw.z);

    float xAmt = sin(yaw) * cos(roll);
    float yAmt = sin(pitch) * cos(roll);
    float zAmt = cos(pitch) * cos(yaw);


    this->lookAt = this->position + glm::normalize(glm::vec3(xAmt, yAmt, -zAmt));
    updateAxisFromLookAt();
}

void Camera::setPos(glm::vec3 newPos) {this->position = newPos;}
void Camera::setLookAt(glm::vec3 newLook) {this->lookAt = newLook;}
void Camera::setRollPitchYaw(glm::vec3 newRPY) {this->rollPitchYaw = newRPY;}
void Camera::changeRollPitchYawBy(glm::vec3 deltaRPY) {
    this->rollPitchYaw = glm::vec3(normDegrees(this->rollPitchYaw.x + deltaRPY.x),
                                   capAmount(this->rollPitchYaw.y + deltaRPY.y, -89.0, 89.0),
                                   normDegrees(this->rollPitchYaw.z + deltaRPY.z));
    if (freeCam) updateAxisFromRPY();
    else updatePosFromLock();
}

glm::vec3 Camera::getPos() {return this->position;}
glm::vec3 Camera::getLookAt() {return this->lookAt;}
bool Camera::getFreeCam() {return this->freeCam;}
