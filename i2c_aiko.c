/*
 * i2c_aiko.c
 *
 *  Created on: 14-10-2016
 *      Author: Pedro Duran
 */

#include "i2c_aiko.h"


int getAccVector_LSM9DS0(s16 *vector_raw) {
	//Getting Acceleration Components

	int bytes = 0;
	u8 addressReg = (ADDRESS_ACC | 0x80); // El bytes MSB debe ser seteado a 1 para multiples lecturas
	u8 getAcceleration[6] = {0, 0, 0, 0, 0, 0};

	bytes = getValue_I2C(IIC_BASE_ADDRESS,
				 ADDRESS_ACC_MAG,
				 &addressReg,
				 getAcceleration,
				 6);

	vector_raw[0] = (s16)((getAcceleration[1]<<8) + getAcceleration[0]);
	vector_raw[1] = (s16)((getAcceleration[3]<<8) + getAcceleration[2]);
	vector_raw[2] = (s16)((getAcceleration[5]<<8) + getAcceleration[4]);

	return bytes;

}


int getGyrVector_LSM9DS0(s16 *vector_raw) {
	//Getting Gyro Components

	int bytes = 0;
	u8 getGyro[6] = {0, 0, 0, 0, 0, 0};
	u8 addressReg = (ADDRESS_GYR | 0x80); // El bytes MSB debe ser seteado a 1 para multiples lecturas
	bytes = getValue_I2C(IIC_BASE_ADDRESS,
				 ADDRESS_GYRO,
				 &addressReg,
				 getGyro,
				 6);

	vector_raw[0] = (s16)((getGyro[1]<<8) + getGyro[0]);
	vector_raw[1] = (s16)((getGyro[3]<<8) + getGyro[2]);
	vector_raw[2] = (s16)((getGyro[5]<<8) + getGyro[4]);

	return bytes;

}


int getMagVector_LSM9DS0(s16 *vector_raw) {
	//Getting Magneto Components

	int bytes = 0;
	u8 getMagneto[6] = {0, 0, 0, 0, 0, 0};
	u8 addressReg = (ADDRESS_MAG | 0x80); // El bytes MSB debe ser seteado a 1 para multiples lecturas
	bytes = getValue_I2C(IIC_BASE_ADDRESS,
				 ADDRESS_ACC_MAG,
				 &addressReg,
				 getMagneto,
				 6);

	vector_raw[0] = (s16)((getMagneto[1]<<8) + getMagneto[0]);
	vector_raw[1] = (s16)((getMagneto[3]<<8) + getMagneto[2]);
	vector_raw[2] = (s16)((getMagneto[5]<<8) + getMagneto[4]);

	return bytes;

}


int setValue_I2C(u32 IicBaseAddress,
				u8  adress7bit,
				u8 *sendBuffer,
				u8 SizeSendBuffer)
{
	/*
	 * Nota: La funcion XIic_Send setea el bit 1 del MSB de los bytes en sendBuffer.
	 * Por lo tanto, al parecer, esto permite la multiple lectura de bytes. Porque por default
	 * siempre le esta diciendo que leera multiples bytes.
	 */

	//u8 SizeSendBuffer = 2;
	int sendByteNumbers;

	sendByteNumbers = XIic_Send(IicBaseAddress,
			                    adress7bit,
			                    sendBuffer,
								SizeSendBuffer,
								XIIC_STOP); //XIIC_REPEATED_START

	return sendByteNumbers;
}


int getValue_I2C(u32 IicBaseAddress,
				u8  adress7bit,
				u8 *sendBuffer,
				u8 *receivedData,
				u8  sizeReceivedData)
{
	/*
	 * Nota: La funcion XIic_Send setea el bit 1 del MSB de los bytes en sendBuffer.
	 * Por lo tanto, al parecer, esto permite la multiple lectura de bytes. Porque por default
	 * siempre le esta diciendo que leera multiples bytes.
	 */


	int recievedByteNumbers;

	XIic_Send(IicBaseAddress,
			  adress7bit,
			  sendBuffer, 1, XIIC_STOP); //XIIC_REPEATED_START


	recievedByteNumbers = XIic_Recv(IicBaseAddress,
			                        adress7bit,
			                        receivedData,
									sizeReceivedData,
									XIIC_STOP);

	return recievedByteNumbers;
}

int Config_LSM9DS0(){

    u8 sendBuffer[2];
    int sendBytes_1 = 0, sendBytes_2 = 0, sendBytes_3 = 0, sendBytes_4 = 0, sendBytes_5 = 0, sendBytes_6 = 0;

    ///////////////////////////////////////////////////////////////
    ///////////////////////SETUP GIROSCOPIO ///////////////////////
    ///////////////////////////////////////////////////////////////
    /*
    % Seteando registro CTRL_REG1_G
    % 0xMN;
    % M: Output data rate selection y Bandwidth selection.
    %    *(0: ODR = 95 Hz, Cutoff = 12.5)
    %    (1: ODR = 95 Hz, Cutoff = 25.0)
    %    (2: ODR = 95 Hz, Cutoff = 25.0)
    %    (3: ODR = 95 Hz, Cutoff = 25.0)
    %    (4: ODR = 190 Hz, Cutoff = 12.5)
    %    (5: ODR = 190 Hz, Cutoff = 25)
    %    (6: ODR = 190 Hz, Cutoff = 50)
    %    (7: ODR = 190 Hz, Cutoff = 70)
    %    (8: ODR = 380 Hz, Cutoff = 20)
    %    (9: ODR = 380 Hz, Cutoff = 25)
    %    (A: ODR = 380 Hz, Cutoff = 50)
    %    (B: ODR = 380 Hz, Cutoff = 100)
    %    (C: ODR = 760 Hz, Cutoff = 30)
    %    (D: ODR = 760 Hz, Cutoff = 35)
    %    (E: ODR = 760 Hz, Cutoff = 50)
    %    (F: ODR = 760 Hz, Cutoff = 100)
    % N: (0: Power Down)
    %    (8: Sleep)
    %    (F: All Axis Enabled)*
    */

    // Seteando registro Seteando CTRL_REG1_G (GyroscopioAddress, CTRL_REG1_G = 0x20; value = 0x0F)
    sendBuffer[0] = ADDRESS_CTRL_REG1_G;
    sendBuffer[1] = 0x0F; // value
    sendBytes_1 = setValue_I2C(IIC_BASE_ADDRESS,
			         ADDRESS_GYRO,
					 sendBuffer,
					 2);

    /*
    % Seteando registro CTRL_REG4_G
    % Full-Scale Selection
    % 0xMN:
    %        *(0x00: Full Scale selection:  245 dps, continuos update, self-test disabled, SPI serial interf. mode: 4 wire)
    %         (0x10: Full Scale selection:  500 dps, continuos update, self-test disabled, SPI serial interf. mode: 4 wire)
    %         (0x20: Full Scale selection: 2000 dps, continuos update, self-test disabled, SPI serial interf. mode: 4 wire)
    */

    //Seteando registro CTRL_REG4_G (GyroscopioAddress, CTRL_REG4_G = 0x23; value = 0x00)
    sendBuffer[0] = ADDRESS_CTRL_REG4_G;
	sendBuffer[1] = 0x00; // value
	sendBytes_2 = setValue_I2C(IIC_BASE_ADDRESS,
					 ADDRESS_GYRO,
					 sendBuffer,
					 2);

	///////////////////////////////////////////////////////////////
	///////////////////////SETUP ACELEROMETRO /////////////////////
	///////////////////////////////////////////////////////////////
	/*
	% Seteando registro CTRL_REG1_XM
	% 0xMN;
	% M: Acceleration Date Rate
	%    (0: Power-Down Power)
	%    (1: 3.125 Hz)
	%    (2: 6.250 Hz)
	%    (3: 12.500 Hz)
	%    (4: 25.000 Hz)
	%    (5: 50.000 Hz)
	%    (6: 100.00 Hz)
	%    (7: 200.00 Hz)
	%    (8: 400.00 Hz)
	%    (9: 800.00 Hz)
	%    (A: 1600.0 Hz )*
	% N: (7: continuos update for acceleration y magnetic sensor)*
	%    (F: output registers not updated until MSB and LSB have been read)
	 */

	//Seteando registro CTRL_REG1_XM (AcelerometroMagAddress, CTRL_REG1_XM = 0x20; value = 0xA7)
	sendBuffer[0] = ADDRESS_CTRL_REG1_XM;
	sendBuffer[1] = 0xA7; // value
	sendBytes_3 = setValue_I2C(IIC_BASE_ADDRESS,
							   ADDRESS_ACC_MAG,
							   sendBuffer,
							   2);


	/*
	 * % Seteando registro CTRL_REG2_XM
	% anti-alias filter bandwidth = AFB
	% Acceleration full scale     = AFS
	% M: Accelerometer anti-alias filter bandwidth. Default value: 00
	%    (0x00: AFB= 773Hz; AFS = +-2g)
	%    (0x40: AFB= 194Hz; AFS = +-2g)*
	%    (0x80: AFB= 362Hz; AFS = +-2g)
	%    (0xC0: AFB=  50Hz; AFS = +-2g)
	%
	%    (0x08: AFB= 773Hz; AFS = +-4g)
	%    (0x48: AFB= 194Hz; AFS = +-4g)
	%    (0x88: AFB= 362Hz; AFS = +-4g)
	%    (0xC8: AFB=  50Hz; AFS = +-4g)
	%
	%    (0x10: AFB= 773Hz; AFS = +-6g)
	%    (0x50: AFB= 194Hz; AFS = +-6g)
	%    (0x90: AFB= 362Hz; AFS = +-6g)
	%    (0xD0: AFB=  50Hz; AFS = +-6g)
	%
	%    (0x18: AFB= 773Hz; AFS = +-8g)
	%    (0x58: AFB= 194Hz; AFS = +-8g)
	%    (0x98: AFB= 362Hz; AFS = +-8g)
	%    (0xD8: AFB=  50Hz; AFS = +-8g)
	%
	%    (0x20: AFB= 773Hz; AFS = +-16g)
	%    (0x60: AFB= 194Hz; AFS = +-16g)
	%    (0xA0: AFB= 362Hz; AFS = +-16g)
	%    (0xE0: AFB=  50Hz; AFS = +-16g)
	 */

	//Seteando registro CTRL_REG2_XM (AcelerometroMagAddress, CTRL_REG2_XM = 0x21; value = 0x40)
	sendBuffer[0] = ADDRESS_CTRL_REG2_XM;
	sendBuffer[1] = 0x40; // value
	sendBytes_4 = setValue_I2C(IIC_BASE_ADDRESS,
							   ADDRESS_ACC_MAG,
							   sendBuffer,
							   2);

	///////////////////////////////////////////////////////////////
	///////////////////////SETUP MAGNETOMETRO /////////////////////
	///////////////////////////////////////////////////////////////

	//Seteando registro CTRL_REG7_XM (AcelerometroMagAddress, CTRL_REG7_XM = 0x26; value = 0x00)
	sendBuffer[0] = ADDRESS_CTRL_REG7_XM;
	sendBuffer[1] = 0x00; // value
	sendBytes_5 = setValue_I2C(IIC_BASE_ADDRESS,
							   ADDRESS_ACC_MAG,
							   sendBuffer,
							   2);

	//Seteando registro CTRL_REG6_XM (AcelerometroMagAddress, CTRL_REG6_XM = 0x25; value = 0x00)
	sendBuffer[0] = ADDRESS_CTRL_REG6_XM;
	sendBuffer[1] = 0x00; // value
	sendBytes_6 = setValue_I2C(IIC_BASE_ADDRESS,
			                   ADDRESS_ACC_MAG,
							   sendBuffer,
							   2);
    if(
       (sendBytes_1 == 2) &&
       (sendBytes_2 == 2) &&
	   (sendBytes_3 == 2) &&
	   (sendBytes_4 == 2) &&
	   (sendBytes_5 == 2) &&
	   (sendBytes_6 == 2)
	   ) return 1;
    else return 0;
}


void Verifica_Config_LSM9DS0(){

	    u8 get_ctrl_reg1_g = 0x0;
	    u8 get_ctrl_reg4_g = 0x0;
	    u8 get_ctrl_reg1_xm = 0x0;
	    u8 get_ctrl_reg2_xm = 0x0;
	    u8 get_ctrl_reg7_xm = 0x0;
	    u8 get_ctrl_reg6_xm = 0x0;
	    u8 sendBuffer[2];
		int sendBytes_1 = 0, sendBytes_2 = 0, sendBytes_3 = 0, sendBytes_4 = 0, sendBytes_5 = 0, sendBytes_6 = 0;

	// Seteando registro Seteando CTRL_REG1_G (GyroscopioAddress, CTRL_REG1_G = 0x20; value = 0x0F)
	    sendBuffer[0] = ADDRESS_CTRL_REG1_G;
	    sendBuffer[1] = 0x0F; // value
	    sendBytes_1 = getValue_I2C(IIC_BASE_ADDRESS,
				         ADDRESS_GYRO,
						 sendBuffer,
						 &get_ctrl_reg1_g,
						 1);

	    //Seteando registro CTRL_REG4_G (GyroscopioAddress, CTRL_REG4_G = 0x23; value = 0x00)
	    sendBuffer[0] = ADDRESS_CTRL_REG4_G;
		sendBuffer[1] = 0x00; // value
		sendBytes_2 = getValue_I2C(IIC_BASE_ADDRESS,
						 ADDRESS_GYRO,
						 sendBuffer,
						 &get_ctrl_reg4_g,
						 1);

		///////////////////////////////////////////////////////////////
		///////////////////////SETUP ACELEROMETRO /////////////////////
		///////////////////////////////////////////////////////////////


		//Seteando registro CTRL_REG1_XM (AcelerometroMagAddress, CTRL_REG1_XM = 0x20; value = 0xA7)
		sendBuffer[0] = ADDRESS_CTRL_REG1_XM;
		sendBuffer[1] = 0xA7; // value
		sendBytes_3 = getValue_I2C(IIC_BASE_ADDRESS,
								   ADDRESS_ACC_MAG,
								   sendBuffer,
								   &get_ctrl_reg1_xm,
								   1);

		//Seteando registro CTRL_REG2_XM (AcelerometroMagAddress, CTRL_REG2_XM = 0x21; value = 0x40)
		sendBuffer[0] = ADDRESS_CTRL_REG2_XM;
		sendBuffer[1] = 0x40; // value
		sendBytes_4 = getValue_I2C(IIC_BASE_ADDRESS,
								   ADDRESS_ACC_MAG,
								   sendBuffer,
								   &get_ctrl_reg2_xm,
								   1);

		///////////////////////////////////////////////////////////////
		///////////////////////SETUP MAGNETOMETRO /////////////////////
		///////////////////////////////////////////////////////////////

		//Seteando registro CTRL_REG7_XM (AcelerometroMagAddress, CTRL_REG7_XM = 0x26; value = 0x00)
		sendBuffer[0] = ADDRESS_CTRL_REG7_XM;
		sendBuffer[1] = 0x00; // value
		sendBytes_5 = getValue_I2C(IIC_BASE_ADDRESS,
								   ADDRESS_ACC_MAG,
								   sendBuffer,
								   &get_ctrl_reg7_xm,
								   1);

		//Seteando registro CTRL_REG6_XM (AcelerometroMagAddress, CTRL_REG6_XM = 0x25; value = 0x00)
		sendBuffer[0] = ADDRESS_CTRL_REG6_XM;
		sendBuffer[1] = 0x00; // value
		sendBytes_6 = getValue_I2C(IIC_BASE_ADDRESS,
				                   ADDRESS_ACC_MAG,
								   sendBuffer,
								   &get_ctrl_reg6_xm,
								   1);

		printf("get_ctrl_reg1_g = 0x%x \n",get_ctrl_reg1_g);
		printf("get_ctrl_reg4_g = 0x%x \n",get_ctrl_reg4_g);
		printf("get_ctrl_reg1_xm = 0x%x \n",get_ctrl_reg1_xm);
		printf("get_ctrl_reg2_xm = 0x%x \n",get_ctrl_reg2_xm);
		printf("get_ctrl_reg7_xm = 0x%x \n",get_ctrl_reg7_xm);
		printf("get_ctrl_reg6_xm = 0x%x \n",get_ctrl_reg6_xm);

		/*  Resultado esperado
		 *
		 *  get_ctrl_reg1_g = 0xf
			get_reg4_g = 0x0
			get_reg1_xm = 0xa7
			get_ctrl_reg2_xm = 0x40
			get_ctrl_reg7_xm = 0x0
			get_ctrl_reg6_xm = 0x0
		 */
}

