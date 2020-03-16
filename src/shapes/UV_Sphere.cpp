#include "UV_Sphere.h"

#include <iostream>

UV_Sphere::UV_Sphere(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec2 faces, MainWindow * main_window)
		: Shape(
			position,
			rotation,
			scale,
			get_geometry(faces),
			{RND(), RND(), RND()},
			{
				"shaders/vert.vert",
				"shaders/error.frag"
			},
			main_window
		)
{
	set_geometry();
}

std::tuple<std::vector<GLfloat>, std::vector<GLfloat>, std::vector<GLuint>> UV_Sphere::get_geometry(glm::vec2 faces) {
	auto vertices = generate_vertices(faces);
	auto indices = generate_indices(faces);
	
	// CALCUL DE L\'ERREUR D\'APPROXIMATION
	float real_vol = ((4.0f/3.0f)*3.1415926535f);
	float vol = 0;

	// Pour chaque triangles
	for (uint64_t i = 0; i < indices.size(); i += 3) {
		// Récupere les 3 points du triangle
		glm::vec3 v0 = glm::vec3(vertices[indices[i]*3], vertices[indices[i]*3+1], vertices[indices[i]*3+2]);
		glm::vec3 v1 = glm::vec3(vertices[indices[i+1]*3], vertices[indices[i+1]*3+1], vertices[indices[i+1]*3+2]);
		glm::vec3 v2 = glm::vec3(vertices[indices[i+2]*3], vertices[indices[i+2]*3+1], vertices[indices[i+2]*3+2]);

		auto local_vol = v0.x*v1.y*v2.z + v1.x*v2.y*v0.z + v2.x*v0.y*v1.z - v0.x*v2.y*v1.z - v1.x*v0.y*v2.z - v2.x*v1.y*v0.z;
		vol += local_vol;
	}
	std::cout << "Nombre de triangles: " <<indices.size()/3 << " | Volume réel : " << real_vol << " | Volume actuel : " << vol/6 << std::endl;
	// FIN DU CALCUL DE L\'ERREUR D\'APPROXIMATION
	return {vertices, vertices, indices};
}

std::vector<GLfloat> UV_Sphere::generate_vertices(glm::vec2 faces) {
	std::vector<GLfloat> vertices;
	float x, y, z, xy;

	unsigned int longitude_count = faces[0];
	unsigned int latitude_count = faces[1];

	float longitude_step = 2*glm::pi<float>()/longitude_count;
	float latitude_step = glm::pi<float>()/latitude_count;
	float longitude_angle, latitude_angle;

	for (unsigned int i = 0; i <= latitude_count; ++i) {
		latitude_angle = glm::pi<float>()/2-i*latitude_step;
		xy = cosf(latitude_angle); 
		z = sinf(latitude_angle); 
		for (unsigned int j = 0; j <= longitude_count; ++j) {
			longitude_angle = j*longitude_step;

			x = xy * cosf(longitude_angle);
			y = xy * sinf(longitude_angle);

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	return vertices;
}

std::vector<GLuint> UV_Sphere::generate_indices(glm::vec2 faces) {
	std::vector<GLuint> indices;
	int k1, k2;
	unsigned int longitude_count = faces[0];
	unsigned int latitude_count = faces[1];

	for (unsigned int i = 0; i < latitude_count; ++i) {
		k1 = i*(longitude_count+1);
		k2 = k1+longitude_count+1;

		for (unsigned int j = 0; j < longitude_count; ++j, ++k1, ++k2) {
			if (i != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1+1);
			}
			if (i != latitude_count-1) {
				indices.push_back(k1+1);
				indices.push_back(k2);
				indices.push_back(k2+1);
			}
		}
	}

	return indices;
}

UV_Sphere::~UV_Sphere() {
	
}
