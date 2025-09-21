#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f,
        float pitch = 0.0f
    );

    // Getters for matrices
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;

    // Input processing
    void processKeyboard(float deltaTime, bool forward, bool back, bool left, bool right, bool upKey, bool downKey);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void processMouseScroll(float yoffset);

    // public config
    float MovementSpeed = 3.0f;
    float MouseSensitivity = 0.1f;
    float Zoom = 45.0f; // fov

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

private:
    // euler angles
    float Yaw;
    float Pitch;

    void updateCameraVectors();
};
