/*
 * main.c
 *
 *  Created on: 14-10-2016
 *      Author: Pedro Duran
 */

#include <stdio.h>
//#include <platform.h>
#include "xil_printf.h"
#include "i2c_aiko.h"


int main()
{
	printf("El retorno de la configuracion es %i\n", Config_LSM9DS0());
	//Verifica_Config_LSM9DS0();


	float factor_acc = 0.061/1000.0;
	float factor_gyr = 8.75/1000.0;
	float factor_mag = 0.08/1000.0;


	s16 vector_acc_raw[3], vector_gyr_raw[3], vector_mag_raw[3];
	float ax, ay, az; // Acceleration components
	float gx, gy, gz; // Gyro components
	float mx, my, mz; // Magneto components

    while(1){

    	getAccVector_LSM9DS0(vector_acc_raw);
    	ax = factor_acc * vector_acc_raw[0];
    	ay = factor_acc * vector_acc_raw[1];
    	az = factor_acc * vector_acc_raw[2];

    	getGyrVector_LSM9DS0(vector_gyr_raw);
    	gx = factor_gyr * vector_gyr_raw[0];
    	gy = factor_gyr * vector_gyr_raw[1];
    	gz = factor_gyr * vector_gyr_raw[2];

    	getMagVector_LSM9DS0(vector_mag_raw);
    	mx = factor_mag * vector_mag_raw[0];
    	my = factor_mag * vector_mag_raw[1];
    	mz = factor_mag * vector_mag_raw[2];


    	printf("ax = %8f, ay = %8f, az = %8f \n", ax, ay, az);
		printf("gx = %8f, gy = %8f, gz = %8f \n", gx, gy, gz);
		printf("mx = %8f, my = %8f, mz = %8f \n", mx, my, mz);
		printf("-----------------------------------------------------\n");
    }

	return 0;
}

