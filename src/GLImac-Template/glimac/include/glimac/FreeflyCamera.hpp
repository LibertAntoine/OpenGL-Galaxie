#include <glimac/glm.hpp>


class FreeflyCamera {
private: 
	glm::vec3 m_Position;
	float m_fPhi;
	float m_fTheta;

	glm::vec3 m_FrontVector; //vecteur F
	glm::vec3 m_LeftVector; //vecteur L
	glm::vec3 m_UpVector; //vecteur U
public: 
	FreeflyCamera();
	void moveLeft(float t);
	void moveFront(float t);
	void rotateLeft(float degrees);
	void rotateUp(float degrees);
	glm::mat4 getViewMatrix() const;
private: 
	void computeDirectionVectors();

};