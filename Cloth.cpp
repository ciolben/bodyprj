#include "Cloth.h"

Cloth::Cloth(unsigned int grid_width, unsigned int grid_height, float cloth_y_position, Mass_spring_system *body_)
    :body_(body_)
    , cloth_particle_mass(0.001f)
{
    double minX = -0.5;
    double minZ = -0.5;
    double maxX = 0.5;
    double maxZ = 0.5;

    double rangeX = maxX - minX;
    double rangeZ = maxZ - minZ;

    double stepX = rangeX / grid_width;
    double stepZ = rangeZ / grid_height;

    body_->clear();
    // add the particles
    for(double z(minZ); z < maxZ; z = z + stepZ) {
        for(double x(minX); x < maxX; x = x + stepX) {
            body_->add_particle( vec3(x, cloth_y_position, z),
                                vec3(), cloth_particle_mass, false );

        }
    }

//    // add the spring
//    for(int i = 0; i < grid_height; ++i) {
//        for(int j = 0; j < grid_width - 1; j++) {
//            int particle_index = j + i * grid_width;
//            int next_particle_index = particle_index + 1;

//            body_->add_spring(particle_index, next_particle_index);
//        }
//    }

//    for(int i = 0; i < grid_width; ++i) {
//        for(int j = 0; j < grid_height - 1; j++) {
//            int particle_index = i + j * grid_width;
//            int next_particle_index = i + (j + 1) * grid_width;

//            body_->add_spring(particle_index, next_particle_index);
//        }
//    }

    //contour
//    for(int i = 0; i < grid_height - 1; ++i) {
//        for(int border = 0; border < 2; ++border) {
//            body_->add_spring(i + border * grid_height * (grid_width - 1),
//                              (i + 1) + border * grid_height * (grid_width - 1));
//        }
//    }
//    for(int i = 0; i < grid_height - 1; ++i) {
//        for(int border = 0; border < 2; ++border) {
//            body_->add_spring(i * grid_width + border * (grid_width-1),
//                              (i + 1) * grid_width  + border * (grid_width-1));
//        }
//    }

    //diagonal
    for(int i = 0; i < grid_height - 1; ++i) {
        for(int j = 0; j < grid_width - 1; ++j) {
            body_->add_spring(
                        i + j  * grid_height,
                        (i + 1) + (j + 1) * grid_height, true );
            body_->add_spring(
                        (i + 1) + j  * grid_height,
                        i + (j + 1) * grid_height, true );
        }
    }
}
