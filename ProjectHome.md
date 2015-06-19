![http://virtualflowlab.googlecode.com/svn/trunk/media/r005.png](http://virtualflowlab.googlecode.com/svn/trunk/media/r005.png)

Virtual Flow Lab (VFL) is an open source Computational Fluid Dynamics (CFD) software developed mainly for educational purposes. Its current capabilities are

- Solution of Navier-Stokes equations for 2D, steady/unsteady, incompressible, laminar flows on Cartesian meshes.

- Uses Patankar's pressure based SIMPLE algorithm and its variants such as SIMPLER and SIMPLEC.

- Implements Patankar's blocked cell approach to create complex geometry flow fields on staggered Cartesian meshes. However, this may result in severe mesh bleeding such that unnecessarily fine cells are created in free stream regions of a flow field.

- Provides real time convergence plot and interactive post-processing.

- It is written using C++. GUIs are created with Nokia's Qt. It is tested on Linux and Windows.

VFL is being developed by [Dr. Cüneyt Sert](http://www.metu.edu.tr/~csert) and [Güneş Nakiboğlu](https://sites.google.com/site/gunesnakib). Some of its screenshots can be seen at http://www.metu.edu.tr/~csert/virtualFlowLab/

This Google Code project is created on October 26, 2007.