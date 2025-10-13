#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/LineWidth>
#include <osg/ShapeDrawable>
#include <osg/Vec3>
#include <osgViewer/Viewer>

osg::ref_ptr<osg::Geode> createAxes(float length = 1.0f) {
  auto vertices = new osg::Vec3Array;
  auto colors = new osg::Vec4Array;

  osg::Vec3 origin(0.0f, 0.0f, 0.0f);

  vertices->push_back(origin);
  vertices->push_back(osg::Vec3(length, 0.0f, 0.0f)); // X
  colors->push_back(osg::Vec4(1, 0, 0, 1));
  colors->push_back(osg::Vec4(1, 0, 0, 1));

  vertices->push_back(origin);
  vertices->push_back(osg::Vec3(0.0f, length, 0.0f)); // Y
  colors->push_back(osg::Vec4(0, 1, 0, 1));
  colors->push_back(osg::Vec4(0, 1, 0, 1));

  vertices->push_back(origin);
  vertices->push_back(osg::Vec3(0.0f, 0.0f, length)); // Z
  colors->push_back(osg::Vec4(0, 0, 1, 1));
  colors->push_back(osg::Vec4(0, 0, 1, 1));

  auto geometry = new osg::Geometry;
  geometry->setVertexArray(vertices);
  geometry->setColorArray(colors, osg::Array::BIND_PER_VERTEX);
  geometry->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, vertices->size()));

  auto lineWidth = new osg::LineWidth(2.0f);
  geometry->getOrCreateStateSet()->setAttributeAndModes(
      lineWidth, osg::StateAttribute::ON);

  auto geode = new osg::Geode;
  geode->addDrawable(geometry);
  return geode;
}

osg::ref_ptr<osg::Geode> createPointSphere(const osg::Vec3 &position,
                                           const osg::Vec4 &color,
                                           float radius = 0.05f) {
  auto sphere = new osg::Sphere(position, radius);
  auto drawable = new osg::ShapeDrawable(sphere);
  drawable->setColor(color);

  auto geode = new osg::Geode;
  geode->addDrawable(drawable);
  return geode;
}

osg::ref_ptr<osg::Group> createMatrixPoints(const osg::Vec3 &col1,
                                            const osg::Vec3 &col2,
                                            const osg::Vec3 &col3) {
  auto group = new osg::Group;
  group->addChild(createPointSphere(col1, osg::Vec4(1, 0, 0, 1))); // red
  group->addChild(createPointSphere(col2, osg::Vec4(0, 1, 0, 1))); // green
  group->addChild(createPointSphere(col3, osg::Vec4(0, 0, 1, 1))); // blue
  return group;
}

int main() {
  // Matrix columns → points
  osg::Vec3 col1(1.0f, 0.0f, 1.0f);
  osg::Vec3 col2(0.0f, 1.0f, 1.5f);
  osg::Vec3 col3(-0.5f, 0.5f, 2.0f);

  auto root = new osg::Group;

  // Global axes
  root->addChild(createAxes(2.0f));

  // Points representing matrix columns
  root->addChild(createMatrixPoints(col1, col2, col3));

  osgViewer::Viewer viewer;
  viewer.setSceneData(root);
  viewer.getCamera()->setClearColor(osg::Vec4(1, 1, 1, 1)); // white bg

  viewer.setUpViewInWindow(100, 100, 800, 600);
  viewer.getCameraManipulator()->setHomePosition(
      osg::Vec3d(3.0, 3.0, 3.0), // Eye position
      osg::Vec3d(0.0, 0.0, 0.0), // Center of scene
      osg::Vec3d(0.0, 0.0, 1.0)  // Up vector
  );
  viewer.home(); // Apply the home position immediately
  return viewer.run();
}
