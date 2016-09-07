/*---------------------------------------------------------------------------*/
/*									                                         */
/* Titulo: plotnormal.c							                             */
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

#
/* NO CAMBIAR LAS SIGUIENTES DECLARACIONES E IMPLEMENTACIONES */

/* Contexto gráfico donde se dibuja */
extern HDC hDC;

extern float minimo, maximo, intervalo;

void plotnormal(HDC hDC, int n_value, float *y, float mu, float sigma, char *yn, char *titles, char *tipo_grafico)
{
        
		int i,j,imin,imax,scale,delta_scl;
		char fprint[5];
		float x,x0,x1,y0,y1;
		float min,max,med,stdev,deltax,float_iscr,factor;
        
        float *normal  = (float *) calloc(MAXDIM, sizeof(float));
		
		minimax(n_value,&imin,&imax,y,&min,&max,&med,&stdev);
		
		//float_iscr=ISCR;         //Preparamos parámetros
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
		
		float_iscr=ISCR;         //Preparamos parámetros
		deltax=float_iscr/n_value;
		
		if (strcmp(tipo_grafico, "histo_benefmax")==0 )
		{
        x = (mu-4*sigma);
		/*Calculo y Dibujo la Funcion de Densidad Normal(mu,sigma)*/
		for (i=0;i<n_value;i++)	{
                x += 4*2*sigma/n_value;
				normal[i]=max*exp(-1*pow((x-mu),2)/(2*pow(sigma,2)));
            }
		x0=-deltax;
        for (i=0; i<n_value; i++)	{
      		x0=x0+deltax;
	    	x1=x0+deltax;
              CambiarColorPincel( 255, 255, 0 );  /* Color Amarillo*/
             line(hDC, x0, JSCR-floor(normal[i]*JSCR/max), x1, JSCR-floor(normal[i+1]*JSCR/max));
		}
		/*Dibujamos líneas +-sigma, +-2*sigma*/
        CambiarColorPincel( 255, 255, 0 );  // Color Amarillo
        line(hDC, (int)((mu-3*sigma)/(mu+4*sigma)*n_value)*deltax, JSCR, (int)((mu-3*sigma)/(mu+4*sigma)*n_value)*deltax, JSCR*0.9); //-floor(normal[(int)((mu-2*sigma)/(mu+4*sigma)*n_value)]*JSCR/max));
		line(hDC, (int)((mu-2*sigma)/(mu+4*sigma)*n_value)*deltax, JSCR, (int)((mu-2*sigma)/(mu+4*sigma)*n_value)*deltax, JSCR-floor(normal[(int)((mu-2*sigma)/(mu+4*sigma)*n_value)]*JSCR/max));
		line(hDC, (int)((mu-sigma)/(mu+4*sigma)*n_value)*deltax, JSCR, (int)((mu-sigma)/(mu+4*sigma)*n_value)*deltax, JSCR-floor(normal[(int)((mu-sigma)/(mu+4*sigma)*n_value)]*JSCR/max));
		line(hDC, (int)((mu)/(mu+4*sigma)*n_value)*deltax, JSCR, (int)((mu)/(mu+4*sigma)*n_value)*deltax, JSCR-floor(normal[(int)((mu)/(mu+4*sigma)*n_value)]*JSCR/max));
		line(hDC, (int)((mu+sigma)/(mu+4*sigma)*n_value)*deltax, JSCR, (int)((mu+sigma)/(mu+4*sigma)*n_value)*deltax, JSCR-floor(normal[(int)((mu+sigma)/(mu+4*sigma)*n_value)]*JSCR/max));
		line(hDC, (int)((mu+2*sigma)/(mu+4*sigma)*n_value)*deltax, JSCR, (int)((mu+2*sigma)/(mu+4*sigma)*n_value)*deltax, JSCR-floor(normal[(int)((mu+2*sigma)/(mu+4*sigma)*n_value)]*JSCR/max));
		line(hDC, (int)((mu+3*sigma)/(mu+4*sigma)*n_value)*deltax, JSCR, (int)((mu+3*sigma)/(mu+4*sigma)*n_value)*deltax, JSCR*0.9); //-floor(normal[(int)((mu+2*sigma)/(mu+4*sigma)*n_value)]*JSCR/max));
			/*line(hDC, 0, JSCR, 0, JSCR-floor(normal[0]*JSCR/max));
			line(hDC, n_value/8*deltax, JSCR, n_value/8*deltax, JSCR-floor(normal[(int)(n_value/8)]*JSCR/max));
			line(hDC, 2*n_value/8*deltax, JSCR, 2*n_value/8*deltax, JSCR-floor(normal[(int)(2*n_value/8)]*JSCR/max));
			line(hDC, 3*n_value/8*deltax, JSCR, 3*n_value/8*deltax, JSCR-floor(normal[(int)(3*n_value/8)]*JSCR/max));
			line(hDC, n_value/2*deltax, JSCR, n_value/2*deltax, JSCR-floor(normal[(int)(n_value/2)]*JSCR/max));
			line(hDC, 5*n_value/8*deltax, JSCR, 5*n_value/8*deltax, JSCR-floor(normal[(int)(5*n_value/8)]*JSCR/max));
			line(hDC, 6*n_value/8*deltax, JSCR, 6*n_value/8*deltax, JSCR-floor(normal[(int)(6*n_value/8)]*JSCR/max));
			line(hDC, 7*n_value/8*deltax, JSCR, 7*n_value/8*deltax, JSCR-floor(normal[(int)(7*n_value/8)]*JSCR/max));
			line(hDC, n_value*deltax, JSCR, n_value*deltax, JSCR-floor(normal[n_value]*JSCR/max));*/
        } //Fin if hist_benef_max
        
        if (strcmp(tipo_grafico, "histo_ddown")==0 )
		{    
        x=0.0;  //x= -1*exp(mu+4*sigma)/n_value;
		/*Calculo y Dibujo la Funcion de Densidad LogNormal(mu,sigma)*/
		for (i=0;i<n_value;i++)	{
                x += exp(mu+4*sigma)/n_value;
				normal[i]= 1/(sigma*x)*exp(-1*pow((log(x)-mu),2)/(2*pow(sigma,2)));
            }
        minimax(n_value,&imin,&imax,normal,&min,&max,&med,&stdev);
		x0=-deltax;
        for (i=0; i<n_value; i++)	{
      		x0=x0+deltax;
	    	x1=x0+deltax;
              CambiarColorPincel( 255, 255, 0 );  /* Color Amarillo*/
             line(hDC, x0, JSCR-floor(normal[i]*JSCR/max), x1, JSCR-floor(normal[i+1]*JSCR/max));
		}
		//Dibujamos líneas exp(mu+-sigma), que nos dan un área del 68% probabilidades:
        line(hDC, (int)(exp(mu-2*sigma)/exp(mu+4*sigma)*n_value)*deltax, JSCR, (int)(exp(mu-2*sigma)/exp(mu+4*sigma)*n_value)*deltax, JSCR-floor(normal[(int)(exp(mu-2*sigma)/exp(mu+4*sigma)*n_value)]*JSCR/max));
		line(hDC, (int)(exp(mu-sigma)/exp(mu+4*sigma)*n_value)*deltax, JSCR, (int)(exp(mu-sigma)/exp(mu+4*sigma)*n_value)*deltax, JSCR-floor(normal[(int)(exp(mu-sigma)/exp(mu+4*sigma)*n_value)]*JSCR/max));
		line(hDC, (int)(exp(mu)/exp(mu+4*sigma)*n_value)*deltax, JSCR, (int)(exp(mu)/exp(mu+4*sigma)*n_value)*deltax, JSCR-floor(normal[(int)(exp(mu)/exp(mu+4*sigma)*n_value)]*JSCR/max));
		line(hDC, (int)(exp(mu+sigma)/exp(mu+4*sigma)*n_value)*deltax, JSCR, (int)(exp(mu+sigma)/exp(mu+4*sigma)*n_value)*deltax, JSCR-floor(normal[(int)(exp(mu+sigma)/exp(mu+4*sigma)*n_value)]*JSCR/max));
		line(hDC, (int)(exp(mu+2*sigma)/exp(mu+4*sigma)*n_value)*deltax, JSCR, (int)(exp(mu+2*sigma)/exp(mu+4*sigma)*n_value)*deltax, JSCR-floor(normal[(int)(exp(mu+2*sigma)/exp(mu+4*sigma)*n_value)]*JSCR/max));
		line(hDC, (int)(exp(mu+3*sigma)/exp(mu+4*sigma)*n_value)*deltax, JSCR, (int)(exp(mu+3*sigma)/exp(mu+4*sigma)*n_value)*deltax, JSCR-floor(normal[(int)(exp(mu+2*sigma)/exp(mu+4*sigma)*n_value)]*JSCR/max));
        		
		/*Dibujamos líneas +-sigma, +-2*sigma
        if (min >= 0)	{
			CambiarColorPincel( 255, 255, 0 );  // Color Amarillo
			line(hDC, 0, JSCR, 0, JSCR-floor(normal[0]*JSCR/max));
			line(hDC, n_value/8*deltax, JSCR, n_value/8*deltax, JSCR-floor(normal[(int)(n_value/8)]*JSCR/max));
			line(hDC, 2*n_value/8*deltax, JSCR, 2*n_value/8*deltax, JSCR-floor(normal[(int)(2*n_value/8)]*JSCR/max));
			line(hDC, 3*n_value/8*deltax, JSCR, 3*n_value/8*deltax, JSCR-floor(normal[(int)(3*n_value/8)]*JSCR/max));
			line(hDC, n_value/2*deltax, JSCR, n_value/2*deltax, JSCR-floor(normal[(int)(n_value/2)]*JSCR/max));
			line(hDC, 5*n_value/8*deltax, JSCR, 5*n_value/8*deltax, JSCR-floor(normal[(int)(5*n_value/8)]*JSCR/max));
			line(hDC, 6*n_value/8*deltax, JSCR, 6*n_value/8*deltax, JSCR-floor(normal[(int)(6*n_value/8)]*JSCR/max));
			line(hDC, 7*n_value/8*deltax, JSCR, 7*n_value/8*deltax, JSCR-floor(normal[(int)(7*n_value/8)]*JSCR/max));
			line(hDC, n_value*deltax, JSCR, n_value*deltax, JSCR-floor(normal[n_value]*JSCR/max));
            }*/
        } //Fin if histo_drawdown
        
     //Disponemos un punto en ROJO en la última cotizacion recibida:
     CambiarColorPincel( 255, 0, 0 );  // Color Rojo
     //putpixel(hDC, ISCR, JSCR-floor(y[n_value-1]*JSCR/max));
     factor=0.5;     //Ampliamos ó reducimos el radio del círculo
     //Ellipse(hDC, ISCR-factor*(n_value/100)*deltax, JSCR-floor(y[n_value-1]*JSCR/max-factor*(n_value/100)*deltax), ISCR+factor*(n_value/100)*deltax, JSCR-floor(y[n_value-1]*JSCR/max+factor*(n_value/100)*deltax));       
     static HRGN hRegion;
     hRegion = CreateEllipticRgn(ISCR-1*factor*(n_value/100)*deltax, JSCR-floor(normal[n_value-1]*JSCR/max-2*factor*(n_value/100)*deltax), ISCR+2*factor*(n_value/100)*deltax, JSCR-floor(normal[n_value-1]*JSCR/max+1*factor*(n_value/100)*deltax));
     FillRgn(hDC, hRegion, (HPEN)SelectObject(hDC, CreatePen(PS_SOLID, 0, RGB(255,0,0))) );
                
      /*Dibujamos líneas Max,Min, Med+-Stdev
        if (min >= 0)	{
            CambiarColorPincel( 0, 0, 0 );     // Color Negro
			line(hDC, (imin+1)*deltax, JSCR, (imin+1)*deltax, JSCR-floor(min*JSCR/max));
			line(hDC, (imax+1)*deltax, JSCR, (imax+1)*deltax, 0);
			CambiarColorPincel( 255, 255, 0 );  // Color Amarillo
			line(hDC, 1, JSCR-floor(med*JSCR/max), ISCR, JSCR-floor(med*JSCR/max));
			line(hDC, 1, JSCR-floor((med+stdev)*JSCR/max), ISCR, JSCR-floor((med+stdev)*JSCR/max));
			line(hDC, 1, JSCR-floor((med-stdev)*JSCR/max), ISCR, JSCR-floor((med-stdev)*JSCR/max));
        }*/
        
        //Leyenda y Datos en Pie de Página.
		SetBkColor(hDC,RGB(192, 192, 192));         //Gris Claro
		//SetBkColor(hDC, (HBRUSH) GetStockObject(LTGRAY_BRUSH));	 //Gris Claro
		TextOut(hDC,10,JSCR+30,"Plot of Data Readings= ",23);
		sprintf(fprint,"%d",n_value);
		TextOut(hDC,170,JSCR+30,fprint,floor(log10(n_value)+1)); 
		TextOut(hDC,250,JSCR+30,"min= ",5);
		TextOut(hDC,JSCR,JSCR+30,"max= ",5);
		sprintf(fprint,"%.3f",min);
		TextOut(hDC,300,JSCR+30,fprint,7);
		sprintf(fprint,"%.3f",max);
		TextOut(hDC,450,JSCR+30,fprint,7);
		TextOut(hDC,250,JSCR+50,"med= ",5);
		TextOut(hDC,400,JSCR+50,"stdev= ",7);
		sprintf(fprint,"%.3f",mu);
		TextOut(hDC,300,JSCR+50,fprint,7);
		sprintf(fprint,"%.3f",sigma);
		TextOut(hDC,450,JSCR+50,fprint,7);
		
		SetBkColor(hDC,RGB(255, 255, 0));	// Color Amarillo.
        SetTextColor(hDC,RGB(0, 0, 255));	//Texto en Azul.
        TextOut(hDC,200,250,titles,strlen(titles));
        //TextOut(hDC,200,100,"Perseus_2032_CAC40_Resultados",29);
        
	free((float*)normal);
return;
}
#undef ISCR
#undef JSCR
