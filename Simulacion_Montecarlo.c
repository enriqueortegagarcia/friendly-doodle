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


#include <windows.h>
#include <windef.h>
#include <string.h>
#include <math.h>

#include "IDS.h"
#include "structs.h"
#include "LeerElementos.h"

extern HDC hDC;

WORD Comando;
char nombre_fichero[80];
float num_anyos;

/* Declaraciones de tipos */
typedef struct stDatos {
   char Texto[80];
} DATOS;


void m_plot_ddwon(HDC hDC, char *nombre_fichero);
void m_histo_ddwon(HDC hDC, char *nombre_fichero);
void m_plot_benefmax(HDC hDC, char *nombre_fichero);
void m_histo_benefmax(HDC hDC, char *nombre_fichero);


/*  Declaración del procedimiento de ventana  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)
{
    HWND hwnd;               /* Manipulador de ventana */
    MSG mensaje;             /* Mensajes recibidos por la aplicación */
    WNDCLASSEX wincl;        /* Estructura de datos para la clase de ventana */

    /* Estructura de la ventana */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = "NUESTRA_CLASE";
    wincl.lpfnWndProc = WindowProcedure;      /* Esta función es invocada por Windows */
    wincl.style = CS_DBLCLKS;                 /* Captura los doble-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Usar icono y puntero por defector */
    /* Usar icono y puntero por defector */
    wincl.hIcon = LoadIcon (hThisInstance, "Icono");
    wincl.hIconSm = LoadIcon (hThisInstance, "Icono");
    //wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    //wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = "Menu";
    wincl.cbClsExtra = 0;                      /* Sin información adicional para la */
    wincl.cbWndExtra = 0;                      /* clase o la ventana */
    /* Usar el color de fondo por defecto para la ventana */
    //wincl.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
    wincl.hbrBackground = (HBRUSH) GetStockObject( LTGRAY_BRUSH );
    //wincl.hbrBackground = (HBRUSH) GetStockObject( WHITE_BRUSH );
     
    /* Registrar la clase de ventana, si falla, salir del programa */
    if(!RegisterClassEx(&wincl)) return 0;
      
    /* La clase está registrada, crear la ventana */
    hwnd = CreateWindowEx(
           0,                   /* Posibilidades de variación */
           "NUESTRA_CLASE",     /* Nombre de la clase */
           "Simulación Montecarlo Sistemas Automáticos de Futuros",       /* Texto del título */
           WS_OVERLAPPEDWINDOW, /* Tipo por defecto */
           //CW_USEDEFAULT,     /* Windows decide la posición */
           //CW_USEDEFAULT,     /* donde se coloca la ventana */
           385,                 //Posicionamos la ventana
           20,                  //esquina sup_izda en (88,16)
           750,                 //Nuevas dimensiones
           550,                 //en pixels de la ventana.
           //650,               /* Ancho */
           //500,               /* Alto en pixels */
           HWND_DESKTOP,        /* La ventana es hija del escritorio */
           NULL,                /* Sin menú */
           hThisInstance,       /* Manipulador de instancia */
           NULL                 /* No hay datos de creación de ventana */
    );

    /* Mostrar la ventana */
    ShowWindow(hwnd, SW_SHOWDEFAULT);

    /* Bucle de mensajes, se ejecuta hasta que haya error o GetMessage devuelva FALSE */
    while(TRUE == GetMessage(&mensaje, NULL, 0, 0))
    {
        /* Traducir mensajes de teclas virtuales a mensajes de caracteres */
        TranslateMessage(&mensaje);
        /* Enviar mensaje al procedimiento de ventana */
        DispatchMessage(&mensaje);
    }

    /* Salir con valor de retorno */
    return mensaje.wParam;
}

/* Esta función es llamada por la función del API DispatchMessage() */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;
    PAINTSTRUCT ps;
    static POINTS punto;
    /* Variables para diálogo */
    static HINSTANCE hInstance;
    static DATOS Datos;
    static int izq, cen, der;
    HBRUSH hBrush, hOldBrush;

    switch (msg)                  /* manipulador del mensaje */
    {
           case WM_CREATE:
           hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
           /* Inicialización de los datos de la aplicación */
           strcpy(Datos.Texto, "1");
           return 0;
           break;
           
       case WM_MOUSEMOVE:
                      punto = MAKEPOINTS(lParam);
                      izq = (wParam & MK_LBUTTON) ? 1 : 0;
                      cen = (wParam & MK_MBUTTON) ? 1 : 0;
                      der = (wParam & MK_RBUTTON) ? 1 : 0;
                      hDC = GetDC(hwnd);
                      Pintar(hDC, izq, cen, der, punto, TRUE, "parcial");
                      ReleaseDC(hwnd, hDC);
                      break;
                      
        case WM_PAINT:
           hDC = BeginPaint(hwnd, &ps);
           //hBrush = CreateSolidBrush(RGB(240,0,0));       // Color Rojo para Gráfico.
           hBrush = CreateSolidBrush(RGB( 0, 125, 255 ));  // Color Azul Pastel para Gráfico.
           hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
           switch(Comando) {
           
              case CM_ELITE_10:
                strcpy(nombre_fichero , "C:\\Simulacion_Montecarlo\\20071203_Elite_CAC_1col.csv");
                //TextOut(hDC, 30, 70, nombre_fichero, strlen(nombre_fichero));
                break;
              case CM_PERSEUS_2032:
                strcpy(nombre_fichero , "C:\\Simulacion_Montecarlo\\PerseusCAC2032_Resultados.csv");
                break;
              case CM_COMBINACION:
                strcpy(nombre_fichero , "Combinacion");
                break;
              case CM_EXIT:
                exit(1);
                break;
                
             
              case CM_PLOT_DRAWDOWN:
                m_plot_ddwon(hDC, nombre_fichero);
                break;
              case CM_HISTO_DRAWADOWN:
                m_histo_ddwon(hDC, nombre_fichero);
                break;
              case CM_PLOT_BENEFMAX:
                m_plot_benefmax(hDC, nombre_fichero);
                break;
              case CM_HISTO_BENEFMAX:
                m_histo_benefmax(hDC, nombre_fichero);
                break;
             
             
                
              case CM_DIALOGO:
                  //DialogBoxParam(hInstance, "DialogoPrueba", hwnd, DlgProc, (LPARAM)&Datos);
                  //num_anyos = atof(Datos.Texto);
                  break;
           } 
           
           SelectObject(hDC, hOldBrush);
           DeleteObject(hBrush);
           EndPaint(hwnd, &ps);
           break;
           
        case WM_COMMAND:
           if(LOWORD(wParam) == CM_DIALOGO)   {
                DialogBoxParam(hInstance, "DialogoPrueba", hwnd, DlgProc, (LPARAM)&Datos);
                num_anyos = atof(Datos.Texto);
                }
                else
                    num_anyos = 1;
                    
           Comando = LOWORD(wParam);
           InvalidateRect(hwnd, NULL, TRUE);
        break;
           
        case WM_DESTROY:
           PostQuitMessage(0);    /* envía un mensaje WM_QUIT a la cola de mensajes */
           break;
           
        default:                  /* para los mensajes de los que no nos ocupamos */
           return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static DATOS *Datos;

    switch (msg)                  /* manipulador del mensaje */
    {
        case WM_INITDIALOG:
           SendDlgItemMessage(hDlg, ID_TEXTO, EM_LIMITTEXT, 80, 0L);
           Datos = (DATOS *)lParam;
           SetDlgItemText(hDlg, ID_TEXTO, Datos->Texto);
           SetFocus(GetDlgItem(hDlg, ID_TEXTO));
           return FALSE;  
        case WM_COMMAND:
           switch(LOWORD(wParam)) {
              case IDOK: 
                 GetDlgItemText(hDlg, ID_TEXTO, Datos->Texto, 80);
                 EndDialog(hDlg, FALSE);
                 break;
              case IDCANCEL:
                 EndDialog(hDlg, FALSE);
                 break;
           }
           return TRUE;
    }
    return FALSE;
}





void m_plot_ddwon(HDC hDC, char *nombre_fichero)
{
     readPolygon(hDC, "parcial","plot_ddown", nombre_fichero);
}

void m_histo_ddwon(HDC hDC, char *nombre_fichero)
{
    readPolygon(hDC, "parcial","histo_ddown", nombre_fichero);
}

void m_plot_benefmax(HDC hDC, char *nombre_fichero)
{
     readPolygon(hDC, "parcial","plot_benefmax", nombre_fichero);
}

void m_histo_benefmax(HDC hDC, char *nombre_fichero)
{
     readPolygon(hDC, "parcial","histo_benefmax", nombre_fichero);
}
