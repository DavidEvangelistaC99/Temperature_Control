/*
 * Fuzzy.h
 *
 *  Created on: Apr 28, 2024
 *      Author: Usuario
 */

#ifndef FUZZY_H_
#define FUZZY_H_

/////////////////////////////////

void inicio(float B[],int tama);
void borrar(float B[],int tama);
int calc_size(float vec[], float paso);
void regla_simple(float in[],float Uin[],float err,float V[], float UV[], float B[], int tam);
void regla_compuesta2(float in1[],float in2[],float U1[],float U2[],float c1,float c2, float V[], float UV[], float B[], int tam);
void regla_compuesta3(float in1[],float in2[],float in3[],float U1[],float U2[],float U3[],float c1,float c2,float c3, float V[], float UV[], float B[], int tam);
float absoluto(float dat);
float triangular(float x[],float params[],float date);
float trapezoidal(float x[],float params[],float date);
void inferencia_mamdani2(float B[],float c,float stru[],float U_VOL[],int tama);
float defusi(float conju[],float U_VOL[],int tama);

/////////////////////////////////

#endif /* FUZZY_H_ */
