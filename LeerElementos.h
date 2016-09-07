/* LEERELEMENTOS.H
**
** Definición de Funciones y Macros auxiliares para programar gráficos si el/la estudiante no sabe usar
** el API de Windows.
*/

#ifndef _LEERELEMENTOS_
#define _LEERELEMENTOS_

/* Definición de Macros */
#define CambiarColorPincel(r,g,b) \
        DeleteObject( (HPEN)SelectObject(hDC, CreatePen(PS_SOLID, 0, RGB(r,g,b))) )
       
/* Definición de Funciones */
void line(HDC hDC, int x_comienzo, int y_comienzo, int x_fin, int y_fin );

/* Función principal para la parte gráfica la cual es modificada por el/la programador/a */

void readPolygon (HDC hDC, char *tipo_lectura, char *tipo_grafico, char *nombre_fichero);
void plotint(HDC hDC, int n_value, float *y, char *yn, char *titles);
void plotnormal(HDC hDC, int n_value, float *y, float mu, float sigma, char *yn, char *titles, char *tipo_grafico);
void LeerElementos(char *fich_in, dato *registro, int *ndatos, char *tipo_lectura);
void  minimax(int num_vals, int *i_min, int *i_max, float vals[], float *min_val, float *max_val, float *mu, float *sigma);
void montecarlo(float *y, int *n_col, int n_filas, char *tipo_grafico, char *titles, float *max, float *min, float *mu, float *sigma);

void Pintar(HDC hdc, int izq, int cen, int der, POINTS punto, BOOL cli, char *tipo_lectura);

#endif
