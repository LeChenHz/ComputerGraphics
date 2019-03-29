///* A very simple demonstration of modeling objects with metaballs (also called
//blobby objects).  Input is a list of balls (squared_radius, x, y, z) followed
//by a line with a negative number at the start.  The output is a 3-D scalar
//field in IBM Data Explorer format.  Coordinates are expected to be in the
//range [0->X, 0->Y, 0->Z] (see defines below).  All I/O through standard in
//and out.  Written by Matt Ward.  Last revised 3/12/97. */
//
//
//#include <iostream>
//#include <string>
//#include <fstream>
//using namespace std;
//
///* dimensions of resulting cube */
//#define X 40
//#define Y 40
//#define Z 40
//
///* coefficients of influence equation */
//#define A -.444444
//#define B 1.888889
//#define C -2.444444
//
///* some macros to keep things cleaner */
//#define SQ(val) ((val) * (val))
//#define CUBE(val) ((val) * (val) * (val))
//#define DIST(x1, y1, z1, x2, y2, z2) (SQ(x2 - (double)x1) + SQ(y2 - (double)y1) + SQ(z2-(double)z1))
//
//float buf[X][Y][Z];	/* field buffer - could be dynamically allocated */
//
//					/* function prototypes */
//void head(void), tail(void), init_buf(void), dump_buf(void);
//void set_buf(float, float, float, float);
//
//int main()
//{
//	float x, y, z, strength;
//	/* clear field */
//	init_buf();
//	/* read metaball info until a negative radius occurs */
//
//	ifstream filestream;
//	filestream.open("res\\point_cloud\\bunny.txt");
//	if (!filestream)
//	{
//		cout << "¶ÁÈ¡ÎÄ¼þÊ§°Ü" << endl;
//		return -1;
//	}
//	while (!filestream.eof())
//	{
//		filestream >> x >> y >> z;
//		cout << x << y << z << endl;
//		set_buf(x, y, z, 0.1);
//	}
//	/* output results in IBM Data Explorer format */
//	head();
//	dump_buf();
//	tail();
//	return 0;
//}
//
///* create an empty field */
//
//void init_buf(void)
//{
//	int i, j, k;
//	for (i = 0; i < X; i++)
//		for (j = 0; j < Y; j++)
//			for (k = 0; k < Z; k++)
//				buf[i][j][k] = 0.0;
//}
//
///* Output the field (not pretty, but it isn't important) */
//
//void dump_buf(void)
//{
//	int i, j, k;
//	for (i = 0; i < X; i++)
//		for (j = 0; j < Y; j++)
//		{
//			for (k = 0; k < Z; k++)
//				printf("%f  ", buf[i][j][k]);
//			printf("\n");
//		}
//}
//
///* Definition of data field in IBM Data Explorer format */
//
//void head(void) {
//	printf(" object 1 class array items %d data follows\n", X*Y*Z);
//}
//
///* Definition of grid positions and 3-D connections in IBM Data Explorer
//format */
//
//void tail(void) {
//	printf(" attribute \"dep\" string \"positions\"\n");
//	printf("\n");
//	printf("object 2 class gridpositions counts %d %d %d\n", X, Y, Z);
//	printf("origin  0.0  0.0  0.0 \n");
//	printf("delta    0.1   0.0   0.0\n");
//	printf("delta    0.0   0.1   0.0\n");
//	printf("delta    0.0   0.0   0.1\n");
//	printf("\n");
//	printf("object 3 class gridconnections counts %d %d %d\n", X, Y, Z);
//	printf("attribute \"element type\" string \"cubes\"\n");
//	printf("attribute \"ref\" string \"positions\"\n");
//	printf("\n");
//	printf("object \"blobby object\" class field\n");
//	printf("component \"data\" 1\n");
//	printf("component \"positions\" 2\n");
//	printf("component \"connections\" 3\n");
//}
//
///* function to update field based on the contribution of a single metaball.
//From Wyvill et. al., "Data Structure for Soft Objects", The Visual Computer,
//Vol. 2, pp. 227-234, 1986 */
//void set_buf(float x, float y, float z, float strength)
//{
//	int i, j, k;
//	float dist, effect;
//	for (i = 0; i < X; i++)
//		for (j = 0; j < Y; j++)
//			for (k = 0; k < Z; k++)
//				/* only need to update things if distance from point(i,j,k) to metaball is
//				within its sphere of influence */
//				if ((dist = DIST(i, j, k, x, y, z)) <= strength)
//				{
//					/* compute influence and update field */
//					effect = A * CUBE(dist) / CUBE(strength) +
//						B * SQ(dist) / SQ(strength) + C * dist / strength + 1.0;
//					buf[i][j][k] += effect;
//				}
//}