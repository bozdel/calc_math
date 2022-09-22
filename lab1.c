#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

double a;
double b;
double c;

#define ZERO(a, b) ((-b) < (a) && (a) < (b))

// gap is oriented by sign value (positive - step right, negative - step left)
//int
void find_interval(double origin, double gap, double eps, double (*func)(double), double *left, double *right) {
	/*if ( func(origin) > 0 ) {
		gap = -gap;
	}*/
	double side = origin + gap;
	while ( func(origin) * func(side) > 0 ) {
		// printf("f(%.3f): %.3f\nf(%.3f): %.3f\n\n", origin, func(origin), side, func(side));
		// sleep(1);
		origin = side;
		side += gap;
	}
	// printf("final: f(%.3f): %.3f\nfinal: f(%.3f): %.3f\n\n", origin, func(origin), side, func(side));

	// just for check
	if (func(origin) == 0.0) {
		printf("x: %.3f\n", origin);
		exit(0);
	}
	if (func(side) == 0.0) {
		printf("x: %.3f\n", side);
		exit(0);
	}

	if (gap > 0) {
		*left = origin;
		*right = side;
	}
	else {
		*left = side;
		*right = origin;
	}
}

double find_value(double eps, double (*func)(double), double left, double right) {
	double middle;
	do {
		middle = (left + right) / 2.0;
		if (func(middle) * func(left) < 0.0) {
			right = middle;
		}
		else {
			left = middle;
		}
	} while (!ZERO(func(middle), eps));
	return middle;
}

double cub(double x) {
	return (x * x * x) + a * (x * x) + b * (x) + c;
}

// true if > 0, otherwise false
bool check_dis() {
	double A = 3.0, B = 2.0 * a, C = b;
	return (B * B - 4 * A * C) > 0.0;
}

void solve_sq(double *p, double *q) {
	double A = 3.0, B = 2.0 * a, C = b;
	double D = sqrt(B * B - 4 * A * C);
	// printf("D: %f\nsqrt(D): %f\n", B * B - 4 * A * C, D);
	*p = (-B - D) / (2 * A);
	*q = (-B + D) / (2 * A);
}

// int
void solve(double eps, double gap, double (*func)(double)) {
	double left = 0.0, right = 0.0;
	if (!check_dis()) {
		if (cub(0.0) > 0.0) find_interval(0.0, -gap, eps, cub, &left, &right);
		else find_interval(0.0, gap, eps, cub, &left, &right);
		
		double value = find_value(eps, cub, left, right);
		printf("x1: %f\n", value);
		return;
	}
	double p = 0.0, q = 0.0;
	solve_sq(&p, &q);

	// if ( (-eps < cub(p) && cub(p) < eps) && (-eps < cub(q) && cub(q) < eps) ) {
	if ( ZERO(cub(p), eps) && ZERO(cub(q), eps) ) {
		double answer = (p + q) / 2.0;
		printf("x1: %f\n", answer);
		return;
	}
	// else if ( (-eps < cub(p) && cub(p) < eps) ) {
	else if ( ZERO(cub(p), eps) ) {
		double answer1 = p;
		find_interval(q, gap, eps, cub, &left, &right); // x from [q, +inf)
		double value = find_value(eps, cub, left, right);
		printf("x1: %f, x2: %f\n", answer1, value);
		return;
	}
	// else if ( (-eps < cub(q) && cub(q) < eps) ) {
	else if ( ZERO(cub(q), eps) ) {
		double answer1 = q;
		find_interval(p, -gap, eps, cub, &left, &right); // x from (-inf, p]
		double value = find_value(eps, cub, left, right);
		printf("x1: %f, x2: %f\n", answer1, value);
		return;
	}
	if (cub(p) < -eps) {
		find_interval(q, gap, eps, cub, &left, &right); // x from [q, +inf)
		double value = find_value(eps, cub, left, right);
		printf("x1: %f\n", value);
		return;
	}
	if (cub(q) > eps) {
		find_interval(p, -gap, eps, cub, &left, &right); // x from (-inf, p]
		double value = find_value(eps, cub, left, right);
		printf("x1: %f\n", value);
		return;
	}
	if (cub(p) > 0 && cub(q) < 0) {
		double v1, v2, v3;
		find_interval(p, -gap, eps, cub, &left, &right); // x from (-inf, p]
		v1 = find_value(eps, cub, left, right);
		find_interval(p, gap, eps, cub, &left, &right); // x from [p, q]
		v2 = find_value(eps, cub, left, right);
		find_interval(q, gap, eps, cub, &left, &right); // x from [q, +inf)
		v3 = find_value(eps, cub, left, right);
		printf("x1: %f, x2: %f, x3: %f\n", v1, v2, v3);
		return;
	}
	return;
}

int main(int argc, char *argv[]) {
	int opt = 0;
	double eps = 0.01;
	double gap  = 0.5;
	a = 1.0; b = 1.0; c = 1.0;
	
	
	/*double left, right;
	find_interval(2.0, -0.5, eps, cub, &left, &right);
	printf("[%.3f, %.3f]\n", left, right);
	double val = find_value(eps, cub, left, right);
	printf("x: %.3f\n", val);
	printf("D > 0: %d\n", check_dis());
	double p, q;
	solve_sq(&p, &q);
	printf("p: %.3f, q: %.3f\n", p, q);*/
	solve(eps, gap, cub);

	return 0;
}