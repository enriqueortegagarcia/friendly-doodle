/*---------------------------------------------------------------------------*/
/*									                                         */
/* Titulo: plotint.c							                             */
/*									                                         */
/* Subrutina para la presentacion grafica de una funcion dando su valor inte-*/
/* gral para una cota dada.					     	                         */
/*									                                         */
/*---------------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include "structs.h"
#include "LeerElementos.h"

/* NO CAMBIAR LAS SIGUIENTES DECLARACIONES E IMPLEMENTACIONES */

/* Contexto gráfico donde se dibuja */
extern HDC hDC;

extern float minimo, maximo, intervalo;

// Función Auxiliar -- NO cambiar esta función
void line(HDC hDC, int x_comienzo, int y_comienzo, int x_fin, int y_fin )
{
  MoveToEx( hDC, x_comienzo, y_comienzo, NULL );
  LineTo( hDC, x_fin, y_fin );
}


void plotint(HDC hDC, int n_value, float *y, char *yn, char *titles)
{
        
		int i,j,imin,imax,scale,delta_scl;
		char fprint[5];
		float x0,x1,y0,y1;
		float min,max,med,stdev,deltax,float_iscr,factor;
                
        //float *y  = (float *) calloc(MAXDIM, sizeof(float));
        float *m50  = (float *) calloc(MAXDIM, sizeof(float));
        float *m200  = (float *) calloc(MAXDIM, sizeof(float));
        float *m500  = (float *) calloc(MAXDIM, sizeof(float));
        float *m1000  = (float *) calloc(MAXDIM, sizeof(float));
        
		/*for (i=0;i<n_value;i++)               //Asignamos valores a y[] e invertimos el orden del vector.
		    y[i]=registro[n_value-i-1].close; //Ojo, estan bien así los contadores en (i), no tocar!!
		*/
		
		minimax(n_value,&imin,&imax,y,&min,&max,&med,&stdev);
		
		float_iscr=ISCR;         //Preparamos parámetros
		deltax=float_iscr/n_value;
        
		/*Calculo y Dibujo las medias moviles de 50,200,500 y 1000 ultimas sesiones*/

		for (i=51;i<n_value;i++)	{
			for (j=1;j<=50;j++)	{
				m50[i] += y[i-j];
				}
				m50[i]=m50[i]/50;
				}
		for (i=201;i<n_value;i++)	{
			for (j=1;j<=200;j++)	{
				m200[i] += y[i-j];
				}
				m200[i]=m200[i]/200;
				}
		for (i=501;i<n_value;i++)	{
			for (j=1;j<=500;j++)	{
				m500[i]+= y[i-j];
				}
				m500[i]=m500[i]/500;
				}
		for (i=1001;i<n_value;i++)	{
			for (j=1;j<=1000;j++)	{
				m1000[i] += y[i-j];
				}
				m1000[i]=m1000[i]/1000;
				}
				
		x0=-deltax;
	    
        for (i=0; i<n_value-1; i++)	{
      		x0=x0+deltax;
	    	x1=x0+deltax;
		    if (i>=51)	{
              CambiarColorPincel( 255, 255, 0 );  /* Color Amarillo*/
             line(hDC, x0, JSCR-floor(m50[i]*JSCR/max), x1, JSCR-floor(m50[i+1]*JSCR/max));
		     }
       		if (i>=201)	{
              CambiarColorPincel( 125, 255, 0);  /* Color Verde */
              line(hDC, x0, JSCR-floor(m200[i]*JSCR/max), x1, JSCR-floor(m200[i+1]*JSCR/max));
		      }
        	if (i>=501)	{
               CambiarColorPincel( 0, 255, 255 );  /* Color Azul claro, ó Magenta */
               line(hDC, x0, JSCR-floor(m500[i]*JSCR/max), x1, JSCR-floor(m500[i+1]*JSCR/max));
               }
            if (i>=1001)	{
               CambiarColorPincel( 0, 0, 0 );  /* Color Negro */
               line(hDC, x0, JSCR-floor(m1000[i]*JSCR/max), x1, JSCR-floor(m1000[i+1]*JSCR/max));
		       }
		}
         
     //Disponemos un punto en ROJO en la última cotizacion recibida:
     CambiarColorPincel( 255, 0, 0 );  // Color Rojo
     //putpixel(hDC, ISCR, JSCR-floor(y[n_value-1]*JSCR/max));
     factor=0.5;     //Ampliamos ó reducimos el radio del círculo
     //Ellipse(hDC, ISCR-factor*(n_value/100)*deltax, JSCR-floor(y[n_value-1]*JSCR/max-factor*(n_value/100)*deltax), ISCR+factor*(n_value/100)*deltax, JSCR-floor(y[n_value-1]*JSCR/max+factor*(n_value/100)*deltax));       
     static HRGN hRegion;
     hRegion = CreateEllipticRgn(ISCR-1*factor*(n_value/100)*deltax, JSCR-floor(y[n_value-1]*JSCR/max-2*factor*(n_value/100)*deltax), ISCR+2*factor*(n_value/100)*deltax, JSCR-floor(y[n_value-1]*JSCR/max+1*factor*(n_value/100)*deltax));
     FillRgn(hDC, hRegion, (HPEN)SelectObject(hDC, CreatePen(PS_SOLID, 0, RGB(255,0,0))) );
     
       //Dibujamos Cuadrícula  10x10 en AZUL
		if (n_value >=20)
			{scale=10;}
			else	{scale=5;}
		delta_scl=floor(float_iscr/scale);
		x0=-deltax; y0=0.0;
		//CambiarColorPincel( 0, 255, 255 );  // Color Azul Claro
		CambiarColorPincel( 0, 0, 255 );  // Color Azul Oscuro
	   	for (i = 1; i <= scale+1; i++)	{
			x0 += delta_scl;	if (y0 < JSCR)	y0 += JSCR/scale;
			//if ( i!= scale ) {
         		line(hDC, x0, JSCR, x0, 0);
        		line(hDC,  0, y0, ISCR, y0);
            //}
			sprintf(fprint, "%d", i*n_value/scale );
	      		//outtextxy( x0, 410, fprint);
			sprintf(fprint, "%.0f", max-max*y0/JSCR );
	      		//outtextxy( 5, y0+1, fprint);
		   	}
                
      /*Dibujamos líneas Max,Min, Med+-Stdev*/
        if (min >= 0)	{
            CambiarColorPincel( 0, 0, 0 );     // Color Negro
			line(hDC, (imin+1)*deltax, JSCR, (imin+1)*deltax, JSCR-floor(min*JSCR/max));
			line(hDC, (imax+1)*deltax, JSCR, (imax+1)*deltax, 0);
			CambiarColorPincel( 255, 255, 0 );  // Color Amarillo
			line(hDC, 1, JSCR-floor(med*JSCR/max), ISCR, JSCR-floor(med*JSCR/max));
			line(hDC, 1, JSCR-floor((med+stdev)*JSCR/max), ISCR, JSCR-floor((med+stdev)*JSCR/max));
			line(hDC, 1, JSCR-floor((med-stdev)*JSCR/max), ISCR, JSCR-floor((med-stdev)*JSCR/max));
        }
        //Leyenda y Datos en Pie de Página.
		SetBkColor(hDC,RGB(192, 192, 192));         //Gris Claro
		//SetBkColor(hDC, (HBRUSH) GetStockObject(LTGRAY_BRUSH));	 //Gris Claro
		TextOut(hDC,10,JSCR+30,"Plot of Data Readings= ",23);
		sprintf(fprint,"%d",n_value);
		TextOut(hDC,170,JSCR+30,fprint,floor(log10(n_value)+1)); 
		TextOut(hDC,250,JSCR+30,"min= ",5);
		TextOut(hDC,JSCR,JSCR+30,"max= ",5);
		sprintf(fprint,"%.3f",minimo);
		TextOut(hDC,300,JSCR+30,fprint,7);
		sprintf(fprint,"%.3f",maximo);
		TextOut(hDC,450,JSCR+30,fprint,7);
		TextOut(hDC,250,JSCR+50,"med= ",5);
		TextOut(hDC,400,JSCR+50,"stdev= ",7);
		sprintf(fprint,"%.3f",med);
		TextOut(hDC,300,JSCR+50,fprint,7);
		sprintf(fprint,"%.3f",stdev);
		TextOut(hDC,450,JSCR+50,fprint,7);
		
		SetBkColor(hDC,RGB(255, 255, 0));	// Color Amarillo.
        SetTextColor(hDC,RGB(0, 0, 255));	//Texto en Azul.
        TextOut(hDC,200,250,titles,strlen(titles));
        //TextOut(hDC,200,100,"Perseus_2032_CAC40_Resultados",29);
  
    //free((float*)y);
	free((float*)m50);
	free((float*)m200);
	free((float*)m500);
	free((float*)m1000);
		
return;
}


void  minimax(int num_vals, int *i_min, int *i_max, float vals[], float
*min_val, float *max_val, float *mu, float *sigma)
{	
	int  i;
	
	*i_min=*i_max=0;
	*min_val = *max_val = vals[0];
	*mu = *sigma = 0.0;

   for (i=0;i<num_vals;i++)		{
		if (vals[i] < *min_val)		{
			*i_min=i;
			*min_val = vals[i];
			}
		if (vals[i] > *max_val) 	{
			*i_max=i;
			*max_val = vals[i];
			}
		*mu=*mu+vals[i];	
	}
	*mu=*mu/(int)(num_vals);
	for (i=0;i<num_vals;i++)	*sigma=*sigma+pow((vals[i]-*mu),2);
	*sigma=sqrt(*sigma/(int)(num_vals));
	
return;
}
#undef ISCR
#undef JSCR
