/*---------------------------------------------------------------------------*/
/*									                                         */
/* Titulo: LeerElementos.c							                         */
/*									                                         */
/* Lectura de datos de fichero secuencial tipo .csv y presentación gráfica   */
/* por polígono macizado en tono azul.                                       */
/*									                                         */
/*---------------------------------------------------------------------------*/

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "structs.h"
#include "LeerElementos.h"

#define EOS "\n"

#define PI 3.1415927

/* NO CAMBIAR LAS SIGUIENTES DECLARACIONES E IMPLEMENTACIONES */

/* Contexto gráfico donde se dibuja */
extern HDC hDC;

float minimo, maximo, intervalo;
//dato *registro  = (dato *) calloc(MAXDIM, sizeof(dato));

//Ejercicios sobre estructuras y registros: (1)
//Leer los registros de una tabla excell con valores separados por ";"
//esto es, de un fichero .csv con una serie de registros contenidos.

void LeerElementos(char *fich_in, dato *registro, int *ndatos, char *tipo_lectura) 
{
     int i,j;
     char *it=NULL;
     char cadena[MAX_STR];
     FILE *f;
       
    // El fichero se abre sólo para lectura y se espera que exista
    if ((f=fopen(fich_in,"rt")) == NULL)	{
	   fprintf(stderr,"Error al abrir fichero input de datos: %s\n", fich_in);
       exit(1);
       }
			i=0;
			fgets(cadena,300,f);       //Leemos Encabezado fichero .csv
			if (strcmp(tipo_lectura,"total")==0)
			{
                    while (!feof(f))
                    {
                   	      fgets(cadena,300,f);
                          /* La primera llamada con entrada*/
                          j=0;
                          it = strtok(cadena, ",");
                          strcpy(registro[i].fecha ,it);
                          //printf("%s\n",it); /*imprime el primer token*/
                          /*Las siguientes llamadas con NULL*/
                          while(it) {
                          		j++;
                            	it = strtok(NULL, ",");
                            	if (!it) break; /* abandona el bucle */
                             	if (j==1)	registro[i].close = atof(it);
                              	/*if (j==2)	registro[i].high = atof(it);
                               	if (j==3)	registro[i].low = atof(it);
                                if (j==4)	registro[i].close = atof(it);
                                if (j==5)	registro[i].volume = atof(it);
                                if (j==6)	registro[i].adj_close = atof(it);*/
                                }
                                i++;
                     }
             } else {
                           //while (i <= *ndatos)
                           while (!feof(f))
                           {
                                 fgets(cadena,300,f);
                                 /* La primera llamada con entrada*/
                                 j=0;
                                 //it = strtok(cadena, ";");
                                 registro[i].close = atof(cadena);
                                 //printf("%s\n",it); //imprime el primer token
                                 /*Las siguientes llamadas con NULL
                                 while(it) {
                                 		j++;
                                   		it = strtok(NULL, ",");
                                   		if (!it) break;   // abandona el bucle
                                     	if (j==1)	registro[i].open = atof(it);
                                      	if (j==2)	registro[i].high = atof(it);
                                        if (j==3)	registro[i].low = atof(it);
                                        if (j==4)	registro[i].close = atof(it);
                                        if (j==5)	registro[i].volume = atof(it);
                                        if (j==6)	registro[i].adj_close = atof(it);
                                        }*/
                           i++;						
                           }
                     }
          *ndatos=i-1;
	      fclose(f);
}
      
  
void readPolygon (HDC hDC, char *tipo_lectura, char *tipo_grafico, char *nombre_fichero)
{
     extern float minimo,maximo,intervalo;
     
     int i=0,imin,imax;
     int num_elem=10000, num_elem_a, n_episodios;
     //char *titles;
     char titles[30];
	 float min,max,med,stdev,mu,sigma,deltax;
	
     float *y  = (float *) calloc(num_elem, sizeof(float));
    
  //Dimensionamos un array de (*num_elem) structs con estructura 'registro'.
  dato *registro  = (dato *) calloc(num_elem+1, sizeof(dato));
  if (registro == NULL)
		{
		printf("\nNo hay suficiente memoria para alojar el struct registro(%d).\n",num_elem+1);
		exit(1);
		}
  POINT *puntos = (POINT *) calloc(num_elem+3, sizeof(POINT));
     if (puntos == NULL)
		{
		printf("\nNo hay suficiente memoria para alojar el struct puntos(%d).\n",num_elem+3);
		exit(1);
		}
    if(strcmp(nombre_fichero,"Combinacion")!=0)    {
        if (strcmp(tipo_lectura,"total")==0)
            LeerElementos(nombre_fichero, registro, &num_elem, "total");
        else
            LeerElementos(nombre_fichero, registro, &num_elem, "parcial");
         for (i=0;i<num_elem;i++) {     //Asignamos valores a y[] e invertimos el orden del vector si procede.
		    //y[i]=registro[*num_elem-i-1].close;
		    y[i]=registro[i].close;
           }
    }
    else    //Combinacion de ambos:  leemos ambos ficheros en array y[].
    {
             LeerElementos("C:\\Simulacion_Montecarlo\\20071203_Elite_CAC_1col.csv", registro, &num_elem, "parcial");
             for (i=0;i<num_elem;i++) {     //Asignamos valores a y[] e invertimos el orden del vector si procede.
		     //y[i]=registro[*num_elem-i-1].close;
		     y[i]=registro[i].close;
             }
             num_elem_a = num_elem;
             LeerElementos("C:\\Simulacion_Montecarlo\\PerseusCAC2032_Resultados.csv", registro, &num_elem, "parcial");
             for (i=num_elem_a;i<num_elem_a + num_elem;i++) {     //Asignamos valores a y[] e invertimos el orden del vector si procede.
		     //y[i]=registro[*num_elem-i-1].close;
		     y[i]=registro[i].close;
             }
             num_elem = num_elem_a + num_elem;
    }
      
    if (num_elem > 1000)            //Limitamos la memoria a utilizar
       n_episodios = 3500;         //cuando dimensionemos la matriz
    else                           //en montecarlo(...). Considero disponibles unos 40Mb para num_elem=1000
        if (num_elem > 500)
           n_episodios = 5000;
        else
            n_episodios = 10000;

   if ( strcmp(nombre_fichero, "C:\\Simulacion_Montecarlo\\20071203_Elite_CAC_1col.csv") ==0)
       strcpy(titles, "Elite_CAC40_10'");
   if ( strcmp(nombre_fichero, "C:\\Simulacion_Montecarlo\\PerseusCAC2032_Resultados.csv") ==0)
       strcpy(titles, "Perseus_2032_CAC40_20'");
   if ( strcmp(nombre_fichero, "Combinacion") ==0)
       strcpy(titles, "Combinacion_Ambos");
          
    montecarlo(y, &(int)(num_elem), n_episodios, tipo_grafico, titles, &max, &min, &mu, &sigma);
        
    if (strcmp(tipo_grafico, "plot_ddown")==0 || strcmp(tipo_grafico, "plot_benefmax")==0 )
    {
       num_elem = n_episodios;  
       minimo=(int)min;
       maximo=(int)max;
       intervalo=num_elem;
       }
    if (strcmp(tipo_grafico, "histo_ddown")==0 || strcmp(tipo_grafico, "histo_benefmax")==0 )
    {
       num_elem=100;
       minimo=(int)min;
       maximo=(int)max;
       intervalo=(int)(max-min);
       }
     
    minimax((int)(num_elem),&imin,&imax,y,&min,&max,&med,&stdev);
    //maximo=max;
   
   //Asignamos los valores leídos como vértices de un Polygon 'puntos': 
    for (i=0;i<num_elem;i++) {
        //puntos[i].x=(long)floor(ISCR-i*ISCR /(int)(*num_elem));
        puntos[i].x=(long)floor(i*ISCR /(int)(num_elem));
        //puntos[i].y=(long)floor(JSCR-registro[i].close*JSCR/max);
        puntos[i].y=(long)floor(JSCR-y[i]*JSCR/max);
        }
        puntos[num_elem].x=ISCR;
        puntos[num_elem].y=JSCR;
        puntos[num_elem+1].x=0;
        puntos[num_elem+1].y=JSCR;
        
   CambiarColorPincel( 0, 125, 255 );  // Color Azul Pastel para Gráfico.
   Polygon(hDC, puntos, (int)(num_elem)+3);
  
   if (strcmp(tipo_grafico, "plot_ddown")==0 || strcmp(tipo_grafico, "plot_benefmax")==0 )
       plotint(hDC, (int)(num_elem), y, "bar", titles);
   if (strcmp(tipo_grafico, "histo_ddown")==0 || strcmp(tipo_grafico, "histo_benefmax")==0 )
       plotnormal(hDC, (int)(num_elem), y, mu, sigma, "bar", titles, tipo_grafico); 
   
   free((float*)y);
   free((dato*)registro);
   free((POINT*)puntos);
}


void Pintar(HDC hdc, int izq, int cen, int der, POINTS punto, BOOL cli, char *tipo_lectura) 
{
     extern float minimo,maximo,intervalo;
     
     SetBkColor(hdc, RGB(255,255,120));
     char cad[32];
    
   if(cli) {
      //sprintf(cad, "x = %04d", punto.x);
      sprintf(cad, "x = %05d", (int)minimo + (int)intervalo*punto.x/ISCR);
      TextOut(hdc, 30, 30, cad, strlen(cad));
      sprintf(cad, "y = %.3f", floor((JSCR-punto.y)*maximo/JSCR));
      //sprintf(cad, "y = %04d", punto.y);
      TextOut(hdc, 30, 50, cad, strlen(cad));
      sprintf(cad, "minimo = %.3f, maximo = %.3f", minimo, maximo);
      TextOut(hdc, 30, 70, cad, strlen(cad));
   }
}
