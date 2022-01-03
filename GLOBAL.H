/*--------------------------------------------------------------------------*/

#define GetObjX(x,y,z)       ( z=x[y].ob_x ) /* Zugriff auf ein Objekt				*/
#define GetObjY(x,y,z)       ( z=x[y].ob_y )
#define SetObjX(x,y,z)       ( x[y].ob_x=z )
#define SetObjY(x,y,z)       ( x[y].ob_y=z )
