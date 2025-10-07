#pragma once

#include <glm.hpp>

class Camera {
public:
    // Camera constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 10.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

    // Returns the view matrix calculated using Euler angles and the LookAt matrix
    glm::mat4 getViewMatrix() const;

    // Returns the projection matrix
    glm::mat4 getProjectionMatrix(float aspectRatio) const;

    // Processes input received from a keyboard-like input system
    void processKeyboard(float deltaTime, bool forward, bool back, bool left, bool right,
                         bool upKey, bool downKey);

    // Processes input received from a mouse input system
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event
    void processMouseScroll(float yoffset);

    // Getters for camera attributes
    glm::vec3 GetPosition() const { return Position; }
    glm::vec3 GetFront() const { return Front; }
    glm::vec3 GetUp() const { return Up; }
    glm::vec3 GetRight() const { return Right; }
    float GetYaw() const { return Yaw; }
    float GetPitch() const { return Pitch; }
    float GetZoom() const { return Zoom; }

    // Setters for camera attributes
    void SetPosition(const glm::vec3 &position) { Position = position; }

    void SetYaw(float yaw) {
        Yaw = yaw;
        updateCameraVectors();
    }

    void SetPitch(float pitch) {
        Pitch = pitch;
        updateCameraVectors();
    }

    void SetZoom(float zoom) { Zoom = glm::clamp(zoom, 1.0f, 90.0f); }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

private:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;

    // Camera options
    float MovementSpeed = 5.0f;
    float MouseSensitivity = 0.1f;
    float Zoom = 45.0f;
};
