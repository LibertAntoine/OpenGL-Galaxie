#include <glimac/glm.hpp>


class TrackballCamera {
private: 
	float m_fDistance; //Distance du centre scène.
	float m_fAngleX; //Angle cam sur X.
	float m_fAngleY; //Angle cam sur Y.
public:
	TrackballCamera();
	void moveFront(float delta);
	void rotateLeft(float degrees);
	void rotateUp(float degrees);
	glm::mat4 getViewMatrix() const;
};