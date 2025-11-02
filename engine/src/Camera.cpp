#include "ext/matrix_clip_space.hpp"
#include "ext/matrix_transform.hpp"
#include <engine/Camera.h>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position_(position), world_up_(up), yaw_(yaw), pitch_(pitch),
      front_(glm::vec3(0.0f, 0.0f, -1.0f)) {
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position_, position_ + front_, up_);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(fov_), aspectRatio, 0.1f, 100.0f);
}

void Camera::processKeyboard(float deltaTime, bool forward, bool back, bool left, bool right,
                             bool upKey, bool downKey) {
    float velocity = movement_speed_ * deltaTime;
    if (forward)
        position_ += front_ * velocity;
    if (back)
        position_ -= front_ * velocity;
    if (left)
        position_ -= right_ * velocity;
    if (right)
        position_ += right_ * velocity;
    if (upKey)
        position_ += world_up_ * velocity;
    if (downKey)
        position_ -= world_up_ * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {

    if (!active_)
        return;
    xoffset *= mouse_sensitivity_;
    yoffset *= mouse_sensitivity_;

    yaw_ += xoffset;
    pitch_ += yoffset;

    if (constrainPitch) {
        if (pitch_ > 89.0f)
            pitch_ = 89.0f;
        if (pitch_ < -89.0f)
            pitch_ = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float xoffset, float yoffset) {
    fov_ -= yoffset;
    if (fov_ < 1.0f)
        fov_ = 1.0f;
    if (fov_ > 90.0f)
        fov_ = 90.0f;

    SE_LOG_INFO("Camera Fov: {}", fov_);
}

void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    // also re-calculate Right and Up vector
    right_ = glm::normalize(glm::cross(front_, world_up_)); // normalize the vectors
    up_ = glm::normalize(glm::cross(right_, front_));
}
