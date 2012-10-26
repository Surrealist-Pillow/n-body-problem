#if OWNER==1
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN int INPUT_SIZE, NTIMESTEPS; 
EXTERN double DTIME, DTHF, EPS;
EXTERN const double THRESHOLD;

#define MIN(X,Y) (X < Y ? X : Y)
#define MAX(X,Y) (X < Y ? Y : X)