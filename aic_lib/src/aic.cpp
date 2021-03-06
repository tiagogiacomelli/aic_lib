#include "aic.h"

aic::aic(aic_comm_config_t aic_connection_parameters)
  : connection_parameters(aic_connection_parameters), aic_ok(false)
{
  this->connect();
}

aic::~aic(void)
{
  if (this->connection->status_ok() == true) this->shutdown();
}

bool aic::shutdown(void){
  this->motor->off();
  this->brake->apply();  
  return this->release();
}

bool aic::release(void){
  this->motor->~aic_motor();
  this->brake->~aic_brake();
  this->sensors->~aic_sensors();
  this->connection->~aic_comm();
  return this->aic_ok = false;
}

void aic::reset(void)
{
  if (aic_ok == true){
    this->connection->send_command(AIC_RESET);
    usleep(10000);
    this->release();
  }
  this->connect();
}

bool aic::connect(void)
{
  aic_ok = false;
  this->connection = new aic_comm(this->connection_parameters);

  if (this->connection->status_ok() == true){
    this->brake = new aic_brake(this->connection);
    this->sensors = new aic_sensors(this->connection);
    this->motor = new aic_motor(this->connection);
    /* Test ---------------------------------------*/
//    this->reset();
    this->connection->send_command(AIC_RESET);
    usleep(10000);
    /* --------------------------------------------*/
    this->motor->on();
    this->brake->release();
    if(this->ping()==true) return aic_ok = true;
    else return aic_ok = false;
  }
}

void aic::set_controller_reference(const double &reference)
{  
  this->motor->set_controller_reference(reference);
}

void aic::set_motor_voltage(const double &voltage)
{  
  this->motor->set_motor_voltage(voltage);
}

void aic::brake_on(void)
{
  this->brake->apply();
}

void aic::brake_off(void)
{
  this->brake->release();
}

bool aic::ping(void)
{
  double displacement = 0;
  int index = 0;
  this->connection->send_command(AIC_STATUS);

  if (this->connection->get_status(AIC_STATUS,&displacement, &index)) return true;
  else return false;
}

aic_displacement_msg_t aic::read_displacement_sensors(void)
{
  return this->sensors->read_displacement();
}

bool aic::status_ok(void)
{
  return this->aic_ok;
}

