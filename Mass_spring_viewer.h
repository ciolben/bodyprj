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

#ifndef MASS_SPRING_VIEWER_H
#define MASS_SPRING_VIEWER_H


//== INCLUDES =================================================================
#include "Eigen/Eigen"
#include "Mass_spring_system.h"
#include "utils/vec2.h"
#include <vector>

#include <QKeyEvent>
#include <QObject>
#include <QGLViewer/qglviewer.h>

#include "sphere.h"

//== CLASS DEFINITION =========================================================


/** \class Mass_spring_viewer Mass_spring_viewer.h <01-mass_springs/Mass_spring_viewer.h>
 Viewer class for the mass spring exercise.
 */
class Mass_spring_viewer : public QObject
{
    Q_OBJECT
public:
    /// constructor
    Mass_spring_viewer(const qglviewer::Camera *camera);

    /// destructor
    ~Mass_spring_viewer();

    /// draw scene
    virtual void draw();
    /// handle keyboard events
    virtual bool keyboard(QKeyEvent *key);

    /// perform one time step using either Euler, Midpoint, or Verlet
    void time_integration(float dt);

    /// handle mouse events (used for interactive spring)
    virtual void mouse(bool release, int _x, int _y);

    /// handle mouse move events (used for interactive spring)
    virtual void motion(int x, int y);

    void printInfo(const QString& text);

    //draw particles with correct settings and names
    void drawWithNames();

    //set the selected particule
    void setSelected(int selected);

    bool moveSelectedParticule(const vec2 &mousePos);



signals:
    void newInfo(const QString& info);

private :

    void throw_sphere();

    //others
    /// is animation on/off?
    bool animate_;

    //selected particule (-1 : no selection)
    int selected_;

    const qglviewer::Camera* camera;

    /// add a 3d object (also manage its destruction)
    void addObject3D(Object3D* object3d);

    /// remove a 3d object (and delete its pointer)
    void removeObject3D(Object3D* object3d);

    /// remove all the 3d objects (and destruct them)
    void removeAllObject3D();

private: // GUI function


private: // simulation functions
    /// compute all external and internal forces
    void compute_forces();

    /// perform impulse-based collision handling
    void impulse_based_collisions();

private: // parameter settings
    /// parameter: mass of a particle
    float particle_mass_;

    /// parameter: amount of damping
    float damping_;

    /// parameter: strength of collision forces
    float collision_stiffness_;

    /// parameter: stiffness of springs
    float spring_stiffness_;

    /// parameter: internal damping of springs
    float spring_damping_;

    /// parameter: strength of area-preserving forces
    float area_stiffness_;

    /// paramters: which time-integration to use
    enum { Euler, Midpoint, Verlet, Implicit } integration_;

    /// paramters: external force type
    enum { None, Center, Gravitation } external_force_;

    /// parameter: how to handle collisiont
    enum { Force_based, Impulse_based } collisions_;

    /// parameter: use area-preserving forces?
    bool area_forces_;

    /// parameter: radius of particles (for rendering and collisions)
    float particle_radius_;

    /// parameter: visualize particle forces?
    bool show_forces_;

    bool camera_gravitation;

    bool cloth_simulation;

    int cloth_width;
    int cloth_height;

    bool cloth_show_particles;

    float m_oldDepth;

private: // simulation data
    /// the mass spring system to be simulated
    Mass_spring_system body_;

    /// class for storing the mouse spring
    struct Mouse_spring
    {
        /// position of the mouse cursor (one endpoint of spring)
        vec3 mouse_position;
        /// which particle is the other endpoint
        int  particle_index;
        /// is the spring active?
        bool active;
    };

    /// the interactive spring controlled by the mouse
    Mouse_spring mouse_spring_;

    /// Objects for collision
    std::vector<Object3D*> m_objects;
};


//=============================================================================
#endif // PHYSICS_ENGINE_VIEWER_HH defined
//=============================================================================

