/*
 * i2c_aiko.h
 *
 *  Created on: 14-10-2016
 *      Author: Pedro Duran
 */

#ifndef SRC_I2C_AIKO_H_
#define SRC_I2C_AIKO_H_



#endif /* SRC_I2C_AIKO_H_ */


#include "xparameters.h"
#include "xiic_l.h"
#include "xil_types.h"


//#include <stdint.h>
//#include <stddef.h>


#define IIC_BASE_ADDRESS	XPAR_IIC_0_BASEADDR

/*
 * Los siguientes parametros corresponden a direccion de slave y de registros del modulo "LSM9DS0"
*/
#define ADDRESS_ACC_MAG	     0x1D //ADDRESS_ACC_MAG: Direccion para acceder a los registros del Acelerometro y magnetrometro
                                  //Direccion de 7 bits (3A si se considera como 8 bits agregando un cero a la cifra menos significativa)
#define ADDRESS_GYRO	     0x6B //ADDRESS_ACC_MAG: Direccion para acceder a los registros del gyroscopio
                                  //Direccion de 7 bits (D6 si se considera como 8 bits agregando un cero a la cifra menos significativa)


#define ADDRESS_CTRL_REG1_XM     0x20 //Direccion de registro CTRL_REG1_XM del Acelerometro. Se utiliza para seteo inicial.
#define ADDRESS_CTRL_REG2_XM     0x21 //Direccion de registro CTRL_REG2_XM del Acelerometro. Se utiliza para seteo inicial.
#define ADDRESS_CTRL_REG1_G      0x20 //Direccion de registro CTRL_REG1_G del Gyroscopio. Se utiliza para seteo inicial.
#define ADDRESS_CTRL_REG4_G      0x23 //Direccion de registro CTRL_REG4_G del Gyroscopio. Se utiliza para seteo inicial.
#define ADDRESS_CTRL_REG6_XM     0x25 //Direccion de registro CTRL_REG6_XM del Magnetometro. Se utiliza para seteo inicial.
#define ADDRESS_CTRL_REG7_XM     0x26 //Direccion de registro CTRL_REG7_XM del Magnetometro. Se utiliza para seteo inicial.

#define ADDRESS_ACC	0x28 // Direccion del registro para obtener las 3 componentes de aceleracion. Se diferencia de ADDRESS_GYR por ADDRESS_ACC_MAG
#define ADDRESS_GYR	0x28 // Direccion del registro para obtener las 3 componentes de gyroscopio.  Se diferencia de ADDRESS_ACC por ADDRESS_GYRO
#define ADDRESS_MAG	0x08 // Direccion del registro para obtener las 3 componentes de magnetometro


/************************** Function Prototypes ******************************/

/*
 * get_acc_vector. Obtiene las componentes x, y, z de aceleracion.
 * vector_raw: es la direccion 0 de un array s16 de 3 elementos.
 *
 * return: El numero de bytes obtenidos. Debiera retornar 6 en un operacion correcta
 */
int getAccVector_LSM9DS0(s16 *vector_raw);

/*
 * get_gyr_vector. Obtiene las componentes x, y, z de velocidad angular (entregadas por el gyroscopio).
 * vector_raw: es la direccion 0 de un array s16 de 3 elementos.
 *
 * return: El numero de bytes obtenidos. Debiera retornar 6 en un operacion correcta
 */
int getGyrVector_LSM9DS0(s16 *vector_raw);

/*
 * get_mag_vector. Obtiene las componentes x, y, z de campo magnetico (entregadas por el magnetometro).
 * vector_raw: es la direccion 0 de un array s16 de 3 elementos.
 *
 * return: El numero de bytes obtenidos. Debiera retornar 6 en un operacion correcta
 */
int getMagVector_LSM9DS0(s16 *vector_raw);




/*
 * Config_LSM9DS0. Obtiene las componentes x, y, z de campo magnetico (entregadas por el magnetometro).
 * vector_raw: es la direccion 0 de un array s16 de 3 elementos.
 *
 * return: retorna un 1 si se consigue enviar el numero correcto de bytes. Sino devuelve 0.
 */
int Config_LSM9DS0();



/*
 * Verifica_Config_LSM9DS0. Verifica informalmente que los registros seteados por Config_LSM9DS0() hayan sido
 * seteados correctamente.
 *
 * return: void
 */
void Verifica_Config_LSM9DS0();



/*
 * setValue_I2C(). Envia bytes via I2C. Esta es una Funcion generica, si se quiere obtener data desde
 * el modulo inercial LSM9DS0, utilizar la funciones
 * getAccVector_LSM9DS0(), getGyrVector_LSM9DS0(), getMagVector_LSM9DS0()
 *
 * u32 IicBaseAddress: 	Direccion del I2C en el bus AXI
 * u8 adress7bit:    	Direccion de 7 bits del slave
 * u8 *sendBuffer:    	Contiene la dirreccion del array de bytes para enviar.
 * u8 SizeSendBuffer    Numero de bytes enviados.
 *
 * return:
 */
int setValue_I2C(u32 IicBaseAddress,
				u8  adress7bit,
				u8 *sendBuffer,
				u8 SizeSendBuffer);



/*
 * getValue_I2C(). Obtiene bytes via I2C. Esta es una Funcion generica, si se quiere obtener data desde
 * el modulo inercial LSM9DS0, utilizar la funciones
 * getAccVector_LSM9DS0(), getGyrVector_LSM9DS0(), getMagVector_LSM9DS0()
 *
 * u32 IicBaseAddress: 	Direccion del I2C en el bus AXI
 * u8 adress7bit:    	Direccion de 7 bits del slave
 * u8 *sendBuffer:		Contiene la dirreccion del array de bytes para enviar.
 * u8 SizeSendBuffer:	Tamano del array de bytes que se envian.
 * u8 *receivedData:	Contiene la dirreccion del array de bytes recibidos
 * u8  sizeSendBuffer:  Numero de bytes recibidos.
 *
 * return:
 */
int getValue_I2C(u32 IicBaseAddress,
				u8  adress7bit,
				u8 *sendBuffer,
				u8 *receivedData,
				u8  sizeSendBuffer);



