#pragma once

#include "OpenGL.h"
#include "Shader.h"
#include "MainWindow.h"

class Shape : public Entity {

public:
	Shape(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
		std::tuple<std::vector<GLfloat>, std::vector<GLfloat>, std::vector<GLuint>> geometry,
		std::string type, glm::vec3 color, Shader shader, MainWindow * main_window);
	~Shape(void);
	
	std::vector<GLuint> indices() const;

	void draw(glm::mat4 view, glm::mat4 projection, float delta_time,
			std::vector<std::shared_ptr<Entity>> lights) override;
	void draw_vertex();
	void use_shader(glm::mat4 view, glm::mat4 projection,
				std::vector<std::shared_ptr<Entity>> lights);

	void set_geometry();
	
	const std::string type() const;
	const glm::vec3 color() const;
	const Shader shader() const;
	
private:
	std::vector<GLfloat> 	_vertices;
	std::vector<GLfloat> 	_normals;
	std::vector<GLuint>		_indices;
	GLuint _VAO;

	glm::vec3 _color;
	Shader _shader;

	const std::string _type;
};
