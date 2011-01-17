#include <iostream>

#include "Events.hh"
#include "Camera.hh"
#include "MouseEvent.hh"
#include "Simulator.hh"
#include "Visual.hh"
#include "World.hh"
#include "SpotLightMaker.hh"

using namespace gazebo;

unsigned int SpotLightMaker::counter = 0;

SpotLightMaker::SpotLightMaker()
  : EntityMaker()
{
  this->state = 0;

  this->msg.type = LightMsg::SPOT;
  this->msg.diffuse.Set(0.5, 0.5, 0.5, 1);
  this->msg.specular.Set(0.1, 0.1, 0.1, 1);
  this->msg.attenuation.Set(0.5, 0.01, 0.0);
  this->msg.direction.Set(0,0,-1);
  this->msg.range = 20;
  this->msg.castShadows = false;
  this->msg.spotInnerAngle = 20;
  this->msg.spotOuterAngle = 40;
  this->msg.spotFalloff = 1.0;
}

SpotLightMaker::~SpotLightMaker()
{
}

void SpotLightMaker::Start(Scene *scene)
{
  std::ostringstream stream;
  stream << "user_spot_light_" << counter++;
  this->msg.id = stream.str();
  this->state = 1;
}

void SpotLightMaker::Stop()
{
  this->state = 0;
  Events::moveModeSignal(true);
}

bool SpotLightMaker::IsActive() const
{
  return this->state > 0;
}

void SpotLightMaker::MousePushCB(const MouseEvent &event)
{
  if (this->state == 0)
    return;

  Vector3 norm;
  norm.Set(0,0,1);

  this->msg.pose.pos = event.camera->GetWorldPointOnPlane(event.pressPos.x, event.pressPos.y, norm, 0);
  this->msg.pose.pos.z = 1.0;
}

void SpotLightMaker::MouseReleaseCB(const MouseEvent &event)
{
  if (this->state == 0)
    return;

  this->state++;

  this->CreateTheEntity();
  this->Stop();
}

void SpotLightMaker::MouseDragCB(const MouseEvent & /*event*/)
{
}

void SpotLightMaker::CreateTheEntity()
{
  Simulator::Instance()->SendMessage(this->msg);
}
