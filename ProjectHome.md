# GLU ES #


### GLU 1.3 partitial port (libutil and libtess components only) for OpenGL ES 1.x (CM - Common profile) and above. This port supports: ###

  * Quadrics: gluNewQuadric(), gluDeleteQuadric(), gluQuadricCallback(), gluQuadricNormals(), gluQuadricTexture(), gluQuadricOrientation(), gluQuadricDrawStyle(), gluCylinder(), gluDisk(), gluPartialDisk(), gluSphere().
  * Registry: gluGetString(), gluCheckExtension(), gluErrorString().
  * Projection matrix manipulation: gluOrtho2D(), gluPerspective(), gluLookAt(), gluProject(), gluUnProject(), gluUnProject4(), gluPickMatrix().
  * 2D Mipmaps: gluScaleImage(), gluBuild2DMipmapLevels(), gluBuild2DMipmaps().
  * Tesselation: gluBeginPolygon(), gluDeleteTess(), gluEndPolygon(), gluGetTessProperty(), gluNewTess(), gluNextContour(), gluTessBeginContour(), gluTessBeginPolygon(), gluTessCallback(), gluTessEndContour(), gluTessEndPolygon(), gluTessNormal(), gluTessProperty(), gluTessVertex().


---

Version 1.1 of GLU ES supports QNX 6.x build target only. Feel free to port this library to any platform, which supports OpenGL ES 1.x.

---

Version 1.2: added SDL 1.3 based tests.

---

Version 1.3: tesselation library and tesselation example for OpenGL ES 1.x (QNX native and SDL based) have been added.

---

Version 1.4: Small fixes, regarding intermediate casts to double during equation calculations. Added Win32 build target for PowerVR OpenGL ES 1.1 emulator. Added GLU ES documentation, based on the OpenGL GLU documentation with ES specific fixes.

---
