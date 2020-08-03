
/*****************************************************************************************
 * INCLUDES
 ****************************************************************************************/
#include "esmacat_epos4_mod.h"
#include "master.h"
#include <string>
#include <sstream>

esmacat_epos4::esmacat_epos4(){

  std::cout <<"Esmacat EPOS 4 Driver Object is created" << std::endl;

  esmacat_slave_vendor_id = ESMACAT_EPOS4_VENDOR_ID;
  esmacat_slave_product_id = ESMACAT_EPOS4_PRODUCT_ID;

  input_statusword = 0;
  input_encoder_counter = 0;
  output_mode_operation = 0;
  epos_enable = false;
  elapsed_time = 0;
  old_elapsed_time = 0;

  esmacat_app_one_cycle_time_sec = (float) 1000000L/1000000L; // need to be updated
  std::cout << "Control Period: " << esmacat_app_one_cycle_time_sec << " ms" << std::endl;

}

/***************************
 *        INPUTS           *
 ***************************/


int esmacat_epos4::get_statusword()
{
  return input_statusword;
}

int32_t esmacat_epos4::get_encoder_counter()
{
  return input_encoder_counter;
}

int32_t esmacat_epos4::get_encoder_filt_speed()
{
  return input_encoder_filt_speed;
}

int16_t esmacat_epos4::get_motor_filt_torque()
{
  return input_motor_filt_torque;
}

double esmacat_epos4::get_position()
{
  return (double)(input_encoder_counter/8192.0*360.0);
}

int esmacat_epos4::get_errorcode()
{
  std::cout << std::hex << input_errorcode << std::endl; // int decimal_value
  return input_errorcode;
}

int16_t esmacat_epos4::get_analog_input_mV()
{
  return input_analog;
}

esmacat_err esmacat_epos4::get_errorcode_hex()
{
  esmacat_err e = NO_ERR;
  if(input_errorcode != 0)
  {
  std::stringstream ss;
  ss << std::hex << input_errorcode; // int decimal_value
  std::string res ( ss.str() );
  std::cout << "Error Code: 0x" << res <<std::endl;
  }
    return e
            ;
}


/***************************
 *        OUTPUTS          *
 ***************************/

esmacat_err esmacat_epos4::set_controlword(uint16_t controlword){
  output_controlword = controlword;
  return NO_ERR;
}

esmacat_err esmacat_epos4::set_mode_operation(int8_t mode_operation){
  output_mode_operation = mode_operation;
  return NO_ERR;
}

esmacat_err esmacat_epos4::set_target_torque(int16_t target_torque){
  output_motor_target_torque = target_torque;
  return NO_ERR;
}

esmacat_err esmacat_epos4::set_current_limit(uint32_t current_limit){
  output_current_limit = current_limit;
  return NO_ERR;
}

esmacat_err esmacat_epos4::set_offset_torque(int16_t offset_torque){
  output_motor_offset_torque = offset_torque;
  return NO_ERR;
}

esmacat_err esmacat_epos4::reset_fault(){
  set_controlword(128);
  return NO_ERR;

}

esmacat_err esmacat_epos4::stop_motor(){
  set_controlword(6);
  return NO_ERR;

}

esmacat_err esmacat_epos4::start_motor(){
  set_controlword(15);
  return NO_ERR;

}

esmacat_err esmacat_epos4::set_elapsed_time(double elapsed_time_ms){
  elapsed_time = elapsed_time_ms-old_elapsed_time-esmacat_app_one_cycle_time_sec;
  old_elapsed_time = elapsed_time_ms;
  return NO_ERR;
}

double esmacat_epos4::get_elapsed_time(){
  return elapsed_time;
}

void esmacat_epos4::ecat_data_process(uint8_t* ec_slave_outputs,int oloop,uint8_t* ec_slave_inputs,int iloop){

  unsigned char input_variable[21];
  unsigned char output_variable[15];

  // TxPDO modified CST

  // Statusword
  // Index: 0x6041-00
  // Type: UNSIGNED16
	input_variable[0] = *(ec_slave_inputs+0);
  input_variable[1] = *(ec_slave_inputs+1);
  input_statusword = +(input_variable[0] << 0)+(input_variable[1] << 8);

  // Position actual value
  // Index: 0x6064-00
  // Type: INTEGER32
	input_variable[2] = *(ec_slave_inputs+2);
	input_variable[3] = *(ec_slave_inputs+3);
  input_variable[4] = *(ec_slave_inputs+4);
  input_variable[5] = *(ec_slave_inputs+5);
  input_encoder_counter = +(input_variable[2] << 0)+(input_variable[3] << 8)+(input_variable[4] << 16)+(input_variable[5] << 24);

  // Velocity actual value averaged
  // Index: 0x30D3-01
  // Type: INTEGER32
	input_variable[6] = *(ec_slave_inputs+6);
	input_variable[7] = *(ec_slave_inputs+7);
	input_variable[8] = *(ec_slave_inputs+8);
	input_variable[9] = *(ec_slave_inputs+9);
  input_encoder_filt_speed = +(input_variable[6] << 0)+(input_variable[7] << 8)+(input_variable[8] << 16)+(input_variable[9] << 24);

  // Torque actual value averaged (MotorRatedTorque/1000)
  // Index: 0x30D2-01
  // Type: INTEGER16
	input_variable[10] = *(ec_slave_inputs+10);
	input_variable[11] = *(ec_slave_inputs+11);
  input_motor_filt_torque = +(input_variable[10] << 0)+(input_variable[11] << 8);

  // Mode of Operation
  input_variable[12] = *(ec_slave_inputs+12);
  input_mode_operation = +(input_variable[12] << 0);

	input_variable[13] = *(ec_slave_inputs+13);
	input_variable[14] = *(ec_slave_inputs+14);
	input_variable[15] = *(ec_slave_inputs+15);
	input_variable[16] = *(ec_slave_inputs+16);

  // Analog Input 1 Voltage
  // Index: 0x3160-01
	input_variable[17] = *(ec_slave_inputs+17);
	input_variable[18] = *(ec_slave_inputs+18);
  input_analog = +(input_variable[17] << 0)+(input_variable[18] << 8);


  // Error Code
  // Index: 0x603F-00
  // Type: UNSIGNED16
  input_variable[19] = *(ec_slave_inputs+19);
  input_variable[20] = *(ec_slave_inputs+20);
  input_errorcode = +(input_variable[19] << 0)+(input_variable[20] << 8);



  // RxPDO CST

  output_variable[7] = 0;
	output_variable[8] = 0;
	output_variable[9] = 0;
	output_variable[10] = 0;

  // Set Controlword
  *(ec_slave_outputs+0)  = (output_controlword & 0x000000ff) >> 0;
  *(ec_slave_outputs+1)  = (output_controlword & 0x0000ff00) >> 8;

  // Set Target torque
  *(ec_slave_outputs+2) = (output_motor_target_torque & 0x000000ff) >> 0;
  *(ec_slave_outputs+3) = (output_motor_target_torque & 0x0000ff00) >> 8;

  // Set Offset torque
  *(ec_slave_outputs+4) = 0;
  *(ec_slave_outputs+5) = 0;

  // Set Mode of Operation
  *(ec_slave_outputs+6) = (output_mode_operation & 0x000000ff) >> 0;

    // Set Digital Outputs
	*(ec_slave_outputs+7) = output_variable[7];
	*(ec_slave_outputs+8) = output_variable[8];
	*(ec_slave_outputs+9) = output_variable[9];  
	*(ec_slave_outputs+10) = output_variable[10];

  // Set output current limit
  // Index: 0x3001-02
  // Type: UNSIGNED32
  *(ec_slave_outputs+11) = (output_current_limit & 0x000000ff) >> 0;
  *(ec_slave_outputs+12) = (output_current_limit & 0x0000ff00) >> 8;
  *(ec_slave_outputs+13) = (output_current_limit & 0x00ff0000) >> 16;
  *(ec_slave_outputs+14) = (output_current_limit & 0xff000000) >> 24;
}
