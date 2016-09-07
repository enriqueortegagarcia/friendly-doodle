//------------------------------------------------------------------------------
// Simulacion de Montecarlo:
//
//            Dados un vector inicial de datos, calcula n_filas
// realizaciones aleatorias en el rango (min,max) de dicho vector,
// realizando una simulacion de Montecarlo que se almacena en la matriz
// "matriz". Elabora un vector salida con, ya sea el vector de Beneficios 
// Maximos realizados en la simulacion, o el vector de Drawdowns Maximos. 
// Realiza tambien un vector histograma para estudiar la distribucción 
// estadística de resultados, con n_histo intervalos.
//
//------------------------------------------------------------------------------

#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define  randomize()  srand(time(0))
#define  random(M)    (rand() % (M))

extern float num_anyos;

void montecarlo(float *y, int *n_col, int n_filas, char *tipo_grafico, char *titles, float *max, float *min, float *mu, float *sigma)
{
	 int i,j,k,kmin,kIni,kDdown,n_columnas,n_histo=100;
	 float Ddown,mu_old;
	 float *histo  = (float *) calloc(n_histo, sizeof(float));
     
     //Restringimos el análisis Dd al 1ªaño, 
                      //a un promedio de 60 ejecuciones/año 
                      //para Perseus_2032,
                      //  y de 157 ejecs/año para Elite_10 .        
                      if ( strcmp(titles, "Elite_CAC40_10'") ==0)
                               *n_col = 157;
                      if ( strcmp(titles, "Perseus_2032_CAC40_20'") ==0)
                               *n_col =  60;
                      if ( strcmp(titles, "Combinacion_Ambos") ==0)
                                *n_col = (60 + 157);
                      n_columnas = num_anyos * *n_col;
                               
      float **matriz=(float **)malloc(n_filas*sizeof(float *));	//Dimensionamos matriz.
      if (matriz==NULL)
	  {
	     	printf("\nNo hay suficiente memoria para asignar la matriz...");
       		exit(0);
      }
	  for (i=0;i<n_filas;i++)
	  {
	      	*(matriz+i)=(float *)malloc(n_columnas*sizeof(float));
		    if (*(matriz+i)==NULL)
	     	{
		    	printf("\nNo hay suficiente memoria para asignar la matriz...");
			    exit(0);
             }
       }
	   
	   //Asignamos valores a la matriz de la Simulacion de Montecarlo:
	   randomize();       //Asignamos un valor aleatorio entre 0 y n_columnas:
              
       for (i=0;i<n_filas;i++)	{           //Asigmamos valores a matriz
       	   for (j=0;j<n_columnas;j++) 	{
                    matriz[i][j] = y[abs(random(*n_col + 1))];  //!Ojo!, sólo tenemos datos hasta y[*n_col].
                    //srand(time(0));
		      		//matriz[i][j] = y[rand()% n_col + 1 ];
	   }	}
	   
       //Una vez formada la matriz, integramos las series numericas fila a fila
       //obteniendo n_filas series temporales de resultados:
                    
      for (i=0;i<n_filas;i++)
           for (j=1;j<n_columnas;j++)
	          	  matriz[i][j] += matriz[i][j-1];        //!Ojo!
                   
      //Calculo del Drawdown máximo para cada serie por filas de valores de la matriz:
      //Definición de Drowdown: La mayor diferencia de cotas entre un valor dado y el 
      //valor mínimo local en la secuencia de valores/cotizaciones-dato desde dicho punto 
      //inicial ó valor dado y hasta que la gráfica vuelve a recuperar dicha cota de valor inicial.
            
      for (i=0;i<n_filas;i++) {
           Ddown = 0; j=0;    
           
           while (j<n_columnas-1)    {
               if (matriz[i][j] > matriz[i][j+1])   {   //punto inicial analisis drawdown
                  k=j; *min=matriz[i][j];
                  while (matriz[i][k] <= matriz[i][j] && k <= n_columnas-1)   {
                        if (matriz[i][k] < *min)  {   //Cambiando aquí > por < podríamos analizar DrownUps!
                           *min = matriz[i][k];
                           kmin = k;
                           }
                        k++;
                  }
                  if ( *min-matriz[i][j] < Ddown) {
                       Ddown = *min-matriz[i][j];  //Drawdown máximo
                       kDdown = kmin; kIni=j;     //kIni: Punto inicial del Ddown, 
                       j = k;                     //kDdown: Punto del DDdown max. (cota mínima)
                       }
               }
               j++;
           }   
           matriz[i][0] = -1*Ddown;      //Guardamos el Drawdown en col. 0
      }
      
      if (strcmp(tipo_grafico, "plot_ddown")==0 || strcmp(tipo_grafico, "plot_benefmax")==0 )
      {
         if (strcmp(tipo_grafico, "plot_benefmax")==0)
         {
            for (j=0;j<n_filas;j++)
               //y[j] = matriz[j][n_col-1];
               y[j] = matriz[j][n_columnas-1];  //Utilizamos la restriccion de n_columnas 
                                                //anteriormente definida en el bloque de calculo de Drawdown.
         }
         if (strcmp(tipo_grafico, "plot_ddown")==0)
         {
            for (j=0;j<n_filas;j++)
               y[j] = matriz[j][0];
         }
      } // fin if para tipo_graf= plot
      /*for (j=1;j<n_col;j++)     //Integramos y[]
               y[j] += y[j-1];
           Ddown = 0; j=0;
           while (j<n_col-1)    {
               if (y[j] > y[j+1])   {   //punto inicial analisis drawdown
                  k=j; min=y[j];
                  while (y[k] <= y[j] && k <= n_col-1)   {
                        if (y[k] < min)  {
                           min = y[k];
                           kmin = k;
                           }
                        k++;
                  }
                  if ( min-y[j] < Ddown) {
                       Ddown = min-y[j];
                       kDdown = kmin; kIni=j;
                       j = k;
                       }
               }
           j++;
           }*/
      
      if (strcmp(tipo_grafico, "histo_ddown")==0 || strcmp(tipo_grafico, "histo_benefmax")==0 )
      {
         if (strcmp(tipo_grafico, "histo_benefmax")==0)
         {
                    //Analisis del Beneficio Maximo Esperable: 	   
	                for (i=0;i<n_filas;i++)     //Asignamos valores de beneficio maximo a y[].
		                y[i] = matriz[i][n_columnas-1];
         }
         if (strcmp(tipo_grafico, "histo_ddown")==0)
         {   
          //Analisis del Drawdown Máximo Esperable: 	   
	                 for (i=0;i<n_filas;i++)    //Asignamos valores de Drawdown maximo a y[].
		                 y[i] = matriz[i][0];
          }
          
          //Formamos valores del histograma agrupados en n_histo intervalos:
          n_histo = 100;
          *max = *min = y[0];
         for (i=0;i<n_filas;i++) {		//Hallamos beneficio máximo y mínimo
	      	if (y[i] > *max)
			    *max = y[i];
		    if (y[i] < *min)
			    *min = y[i];
             }
          for (i=0;i<n_histo;i++)     //Conteamos ocurrencias en cada intervalo 1-n_histo del histograma
              for (j=0;j<n_filas;j++)
                            if (y[j] >= *min+i*(*max-*min)/n_histo && y[j] <= *min+(i+1)*(*max-*min)/n_histo)
                                     histo[i]++;
                                     
          *mu = *sigma = 0;
          if (strcmp(tipo_grafico, "histo_benefmax")==0)
         {
	         for (i=0;i<n_filas;i++)
                  *mu += y[i];
             *mu=*mu/(int)(n_filas);
             for (i=0;i<n_filas;i++)
                 *sigma += pow((y[i]-*mu),2);
             *sigma=sqrt(*sigma/(int)(n_filas-1));
          }
          if (strcmp(tipo_grafico, "histo_ddown")==0)
         {
	         for (i=0;i<n_filas;i++)
                 if (y[i] > 0)      
                    *mu += log(y[i]);
             *mu = mu_old = *mu/(int)(n_filas);
             for (i=0;i<n_filas;i++)
                 if (y[i] > 0)
                    *sigma += pow((log(y[i])-*mu),2);
             *sigma=sqrt(*sigma/(int)(n_filas-1));
                          
              /* *mu = exp(*mu+pow(*sigma,2)/2);      //Calculamos Media y Varianza Distrib. LogNormal
             //*sigma = exp(2*(mu_old+pow(*sigma,2)))-exp(2*mu_old+pow(*sigma,2));       //Expresion en un doc PDF
             *sigma = (exp(pow(*sigma,2))-1)*exp(2*mu_old+pow(*sigma,2));        //Expresión en Wikipedia
             *sigma = sqrt(*sigma);                                              //y en Doc. NTP418 Ministerio Trabajo
             */
          }
          
          for (i=0;i<n_histo;i++)   //Asignamos a y[] valores de histo[]
              y[i]=histo[i];
              
       } // fin if para tipo_graf= histo
          
free(histo);
for (i=0;i<n_filas;i++)
	  free(*(matriz+i));
free(matriz);
}
