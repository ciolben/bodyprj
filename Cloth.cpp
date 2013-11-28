#include "Cloth.h"

Cloth::Cloth(unsigned int grid_width, unsigned int grid_height, float cloth_z_position, Mass_spring_system *body_)
    :body_(body_)
    , cloth_particle_mass(0.0001f)
{
    double minX = -1.0;
    double minY = -1.0;
    double maxX = 1.0;
    double maxY = 1.0;

    double rangeX = maxX - minX;
    double rangeY = maxY - minY;

    double stepX = rangeX / grid_width;
    double stepY = rangeY / grid_height;

    body_->clear();
    // add the particles
    for(double y(minY); y < maxY; y = y + stepY) {
        for(double x(minX); x < maxX; x = x + stepX) {
            body_->add_particle( vec3(x, y, cloth_z_position),
                                vec3(), cloth_particle_mass, true );

        }
    }

    // add the spring
    for(int i = 0; i < grid_height; ++i) {
        for(int j = 0; j < grid_width - 1; j++) {
            int particle_index = j + i * grid_width;
            int next_particle_index = particle_index + 1;

            body_->add_spring(particle_index, next_particle_index);
        }
    }

    for(int i = 0; i < grid_width; ++i) {
        for(int j = 0; j < grid_height - 1; j++) {
            int particle_index = i + j * grid_width;
            int next_particle_index = i + (j + 1) * grid_width;

            body_->add_spring(particle_index, next_particle_index);
        }
    }
}
