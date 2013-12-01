//=============================================================================
//
//   Exercise code for the lecture
//   "Advanced Computer Graphics"
//
//   Adapted from Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) 2013 LGG, epfl
//
//   DO NOT REDISTRIBUTE
//=============================================================================

#ifndef MASS_SPRING_SYSTEM_H
#define MASS_SPRING_SYSTEM_H


//== INCLUDES =================================================================

#include "utils/vec3.h"
#include "utils/vec2.h"
#include <qgl.h>
#include <gl/glut.h>
#include <vector>

#include "QGLViewer/manipulatedFrame.h"

//== CLASS DEFINITION =========================================================


/** \class Particle Mass_spring_system.h <01-mass_springs/Mass_spring_system.h>
 Class for a particle that stores position, velocity, force and other data.
 */
struct Particle
{
    /// constructor
    Particle(vec3 p, vec3 v, float m, bool l)
    : position(p), velocity(v), force(0,0), mass(m), locked(l), acceleration(0,0)
    {}

    vec3  position; ///< position of the particle
    vec3  velocity; ///< velocity of the particle
    vec3  force;    ///< accumulated force acting on the particle
    float mass;     ///< mass of the particle
    bool  locked;   ///< is the particle locked?

    vec3  position_t;   ///< used for Midpoint integration
    vec3  velocity_t;   ///< used for Midpoint integration
    vec3  acceleration; ///< used for Verlet integration
};



//== CLASS DEFINITION =========================================================


/** \class Spring Mass_spring_system.h <01-mass_springs/Mass_spring_system.h>
 Class for representing springs
 */
class Spring
{
public:
    /// construct with two particles. automatically computes rest length.
    Spring(Particle& p0, Particle& p1, bool breakable = false)
        : particle0(&p0), particle1(&p1), is_broken(false), breakable(breakable)
    {
        rest_length = length();
        maximum_length = rest_length * 1.5f;
    }

    /// get current length of the spring
    float length() const
    {
        return norm(particle0->position - particle1->position);
    }

    void break_test() {
        if(breakable) {
            if (length() > maximum_length) {
                is_broken = true;
            }
        }
    }


    Particle*  particle0;   ///< pointer to first particle
    Particle*  particle1;   ///< pointer to second particle
    float      rest_length; ///< rest length

    float maximum_length;
    bool is_broken;
    bool breakable;

};



//== CLASS DEFINITION =========================================================


/** \class Triangle Mass_spring_system.h <01-mass_springs/Mass_spring_system.h>
 Class for storing triangles (for area preserving forces)
 */
struct Triangle
{
    /// construct with three particles. automatically computes rest area.
    Triangle(Particle& p0, Particle& p1, Particle& p2)
    : particle0(&p0), particle1(&p1), particle2(&p2)
    {
        rest_area = area();
    }

    /// compute current area
    float area() const
    {
        const vec3& p0 = particle0->position;
        const vec3& p1 = particle1->position;
        const vec3& p2 = particle2->position;
        return 0.5f * ((p1[0]-p0[0]) * (p2[1]-p0[1]) - (p2[0]-p0[0]) * (p1[1]-p0[1]));
    }

    Particle*  particle0; ///< pointer to first particle
    Particle*  particle1; ///< pointer to second particle
    Particle*  particle2; ///< pointer to third particle
    float      rest_area; ///< area in rest state
};



//== CLASS DEFINITION =========================================================


/** \class Mass_spring_system Mass_spring_system.h <01-mass_springs/Mass_spring_system.h>
 Class for managing a mass-spring system
 */
class Mass_spring_system
{
public:
    /// constructor
    Mass_spring_system() {}

    /// clear all particles, springs, and triangles
    void clear();

    /// add a particle
    void add_particle( vec3 position, vec3 velocity, float mass, bool locked );
    void add_particle(vec2 position, vec2 velocity, float mass, bool locked);

    /// add a spring
    void add_spring(unsigned int i0, unsigned int i1, bool breakable = false);

    /// add a triangle
    void add_triangle(unsigned int i0, unsigned int i1, unsigned int i2);

    /// render the mass spring system
    void draw(float particle_radius, bool show_forces, int selected = -1) const;

public:
    std::vector<Particle>  particles; ///< vector of all particles
    std::vector<Spring>    springs;   ///< vector of all springs
    std::vector<Triangle>  triangles; ///< vector of all triangles
    qglviewer::ManipulatedFrame mf_;
};


//=============================================================================
#endif
//=============================================================================

