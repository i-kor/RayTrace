#ifndef VEC_H
#define VEC_H

struct Vec
{
    double e[3];
};

struct Vec Vec (double x, double y, double z);

struct Vec VecInv (const struct Vec vec);
struct Vec VecIncr (struct Vec left, const double right);
struct Vec VecDecr (struct Vec left, const double right);

double     VecLen (const struct Vec vec);
double     VecLenSq (const struct Vec vec);

struct Vec VecAdd (const struct Vec left, const struct Vec right);
struct Vec VecSub (const struct Vec left, const struct Vec right);
double     VecDot (const struct Vec left, const struct Vec right);
struct Vec VecCross (const struct Vec left, const struct Vec right);
struct Vec VecUnit (const struct Vec left);

char*      VecPrn(struct Vec vec);

#endif // VEC_H