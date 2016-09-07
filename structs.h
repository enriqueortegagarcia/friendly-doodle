#ifndef _STRUCTS_H
#define _STRUCTS_H

#define MAXDIM 20000
#define MAX_STR 300
#define ISCR 640
#define JSCR 400

#define PI 3.1415927

/* Tipo Dato */
typedef struct	dato {
	      char fecha[10];
	      float open;
	      float high;
	      float low;
	      float close;
	      float volume;
	      float adj_close;
} dato;

#endif
