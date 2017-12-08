#ifndef AIC_H
#define AIC_H

#include "aic_brake.h"
#include "aic_comm.h"
#include "aic_sensors.h"
#include "aic_motor.h"

class aic
{
public:
   aic(aic_comm_config_t aic_connection_parameters);
  ~aic();

  void ping(void);
  void reset(void);
  void set_controller_reference(double reference);
  void set_motor_voltage(double voltage);
  void brake_on(void);
  void brake_off(void);
  aic_displacement_msg_t read_displacement_sensors(void);
  bool status_ok();

private:
  bool connect();
  bool shutdown();
  bool release();
  aic_comm_config_t connection_parameters;
  aic_comm * connection;
  aic_brake * brake;
  aic_sensors * sensors;
  aic_motor * motor;
  bool aic_ok;
};


#endif // AIC_H