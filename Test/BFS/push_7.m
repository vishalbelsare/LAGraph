function [results, n, nvals, nthreads, name] = push_7
% bfs_log: all-push
name = 'stdin' ;
n = 7 ;
nvals = 12 ;
d = 1.71429 ;
nthreads = 8 ; 
results = [
%%%%% n 7 nvals 12 s 1
1 7 1.71429  1 1 1 1  7.733e-06
1 7 1.71429  1 2 2 3  5.169e-06
1 7 1.71429  1 3 3 6  4.88601e-06
%%%%% n 7 nvals 12 s 4
4 7 1.71429  1 1 1 1  5.297e-06
4 7 1.71429  1 2 1 2  4.73199e-06
4 7 1.71429  1 3 1 3  4.62901e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  4.98301e-06
2 7 1.71429  1 2 1 2  4.659e-06
%%%%% n 7 nvals 12 s 5
5 7 1.71429  1 1 1 1  5.094e-06
5 7 1.71429  1 2 1 2  4.88199e-06
%%%%% n 7 nvals 12 s 1
1 7 1.71429  1 1 1 1  5.076e-06
1 7 1.71429  1 2 2 3  5.11701e-06
1 7 1.71429  1 3 3 6  5.002e-06
%%%%% n 7 nvals 12 s 3
3 7 1.71429  1 1 1 1  5.17399e-06
3 7 1.71429  1 2 2 3  4.91099e-06
3 7 1.71429  1 3 2 5  4.792e-06
%%%%% n 7 nvals 12 s 3
3 7 1.71429  1 1 1 1  5.062e-06
3 7 1.71429  1 2 2 3  4.74699e-06
3 7 1.71429  1 3 2 5  4.85699e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  4.97799e-06
2 7 1.71429  1 2 1 2  4.52501e-06
%%%%% n 7 nvals 12 s 1
1 7 1.71429  1 1 1 1  4.933e-06
1 7 1.71429  1 2 2 3  4.652e-06
1 7 1.71429  1 3 3 6  4.87e-06
%%%%% n 7 nvals 12 s 3
3 7 1.71429  1 1 1 1  5.02901e-06
3 7 1.71429  1 2 2 3  4.78799e-06
3 7 1.71429  1 3 2 5  4.78e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  4.892e-06
2 7 1.71429  1 2 1 2  4.54401e-06
%%%%% n 7 nvals 12 s 5
5 7 1.71429  1 1 1 1  4.96501e-06
5 7 1.71429  1 2 1 2  4.571e-06
%%%%% n 7 nvals 12 s 6
6 7 1.71429  1 1 1 1  5.108e-06
6 7 1.71429  1 2 3 4  4.84001e-06
6 7 1.71429  1 3 2 6  4.881e-06
%%%%% n 7 nvals 12 s 4
4 7 1.71429  1 1 1 1  4.997e-06
4 7 1.71429  1 2 1 2  4.653e-06
4 7 1.71429  1 3 1 3  4.61e-06
%%%%% n 7 nvals 12 s 6
6 7 1.71429  1 1 1 1  4.986e-06
6 7 1.71429  1 2 3 4  4.901e-06
6 7 1.71429  1 3 2 6  4.648e-06
%%%%% n 7 nvals 12 s 0
0 7 1.71429  1 1 1 1  4.90401e-06
0 7 1.71429  1 2 2 3  4.90701e-06
0 7 1.71429  1 3 3 6  4.97e-06
%%%%% n 7 nvals 12 s 3
3 7 1.71429  1 1 1 1  4.95999e-06
3 7 1.71429  1 2 2 3  4.773e-06
3 7 1.71429  1 3 2 5  4.61601e-06
%%%%% n 7 nvals 12 s 1
1 7 1.71429  1 1 1 1  4.966e-06
1 7 1.71429  1 2 2 3  4.80301e-06
1 7 1.71429  1 3 3 6  4.852e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  5.04899e-06
2 7 1.71429  1 2 1 2  4.42e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  4.81801e-06
2 7 1.71429  1 2 1 2  4.352e-06
%%%%% n 7 nvals 12 s 3
3 7 1.71429  1 1 1 1  4.91401e-06
3 7 1.71429  1 2 2 3  4.79e-06
3 7 1.71429  1 3 2 5  4.731e-06
%%%%% n 7 nvals 12 s 0
0 7 1.71429  1 1 1 1  4.98399e-06
0 7 1.71429  1 2 2 3  4.71401e-06
0 7 1.71429  1 3 3 6  4.876e-06
%%%%% n 7 nvals 12 s 5
5 7 1.71429  1 1 1 1  4.909e-06
5 7 1.71429  1 2 1 2  4.52201e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  4.84301e-06
2 7 1.71429  1 2 1 2  4.388e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  4.76201e-06
2 7 1.71429  1 2 1 2  4.371e-06
%%%%% n 7 nvals 12 s 5
5 7 1.71429  1 1 1 1  4.777e-06
5 7 1.71429  1 2 1 2  4.30901e-06
%%%%% n 7 nvals 12 s 5
5 7 1.71429  1 1 1 1  4.716e-06
5 7 1.71429  1 2 1 2  4.275e-06
%%%%% n 7 nvals 12 s 6
6 7 1.71429  1 1 1 1  4.97e-06
6 7 1.71429  1 2 3 4  4.82901e-06
6 7 1.71429  1 3 2 6  4.637e-06
%%%%% n 7 nvals 12 s 1
1 7 1.71429  1 1 1 1  4.93601e-06
1 7 1.71429  1 2 2 3  4.71e-06
1 7 1.71429  1 3 3 6  4.703e-06
%%%%% n 7 nvals 12 s 5
5 7 1.71429  1 1 1 1  4.831e-06
5 7 1.71429  1 2 1 2  4.545e-06
%%%%% n 7 nvals 12 s 5
5 7 1.71429  1 1 1 1  4.86401e-06
5 7 1.71429  1 2 1 2  4.33601e-06
%%%%% n 7 nvals 12 s 0
0 7 1.71429  1 1 1 1  4.881e-06
0 7 1.71429  1 2 2 3  4.687e-06
0 7 1.71429  1 3 3 6  4.813e-06
%%%%% n 7 nvals 12 s 3
3 7 1.71429  1 1 1 1  4.867e-06
3 7 1.71429  1 2 2 3  4.80699e-06
3 7 1.71429  1 3 2 5  4.748e-06
%%%%% n 7 nvals 12 s 5
5 7 1.71429  1 1 1 1  4.877e-06
5 7 1.71429  1 2 1 2  4.318e-06
%%%%% n 7 nvals 12 s 3
3 7 1.71429  1 1 1 1  4.89401e-06
3 7 1.71429  1 2 2 3  4.589e-06
3 7 1.71429  1 3 2 5  4.70801e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  4.884e-06
2 7 1.71429  1 2 1 2  4.37901e-06
%%%%% n 7 nvals 12 s 1
1 7 1.71429  1 1 1 1  4.915e-06
1 7 1.71429  1 2 2 3  4.645e-06
1 7 1.71429  1 3 3 6  4.813e-06
%%%%% n 7 nvals 12 s 0
0 7 1.71429  1 1 1 1  5.49901e-06
0 7 1.71429  1 2 2 3  4.75399e-06
0 7 1.71429  1 3 3 6  4.98499e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  5.026e-06
2 7 1.71429  1 2 1 2  4.48299e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  4.956e-06
2 7 1.71429  1 2 1 2  4.45299e-06
%%%%% n 7 nvals 12 s 1
1 7 1.71429  1 1 1 1  4.86101e-06
1 7 1.71429  1 2 2 3  4.50001e-06
1 7 1.71429  1 3 3 6  6.192e-06
%%%%% n 7 nvals 12 s 4
4 7 1.71429  1 1 1 1  5.23399e-06
4 7 1.71429  1 2 1 2  4.71999e-06
4 7 1.71429  1 3 1 3  4.58701e-06
%%%%% n 7 nvals 12 s 5
5 7 1.71429  1 1 1 1  4.95301e-06
5 7 1.71429  1 2 1 2  4.51601e-06
%%%%% n 7 nvals 12 s 0
0 7 1.71429  1 1 1 1  4.963e-06
0 7 1.71429  1 2 2 3  4.744e-06
0 7 1.71429  1 3 3 6  4.695e-06
%%%%% n 7 nvals 12 s 6
6 7 1.71429  1 1 1 1  5.01401e-06
6 7 1.71429  1 2 3 4  4.79999e-06
6 7 1.71429  1 3 2 6  4.637e-06
%%%%% n 7 nvals 12 s 3
3 7 1.71429  1 1 1 1  4.949e-06
3 7 1.71429  1 2 2 3  4.68601e-06
3 7 1.71429  1 3 2 5  4.80401e-06
%%%%% n 7 nvals 12 s 6
6 7 1.71429  1 1 1 1  4.93899e-06
6 7 1.71429  1 2 3 4  4.78e-06
6 7 1.71429  1 3 2 6  4.22499e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  4.34001e-06
2 7 1.71429  1 2 1 2  3.97701e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  4.35399e-06
2 7 1.71429  1 2 1 2  4.045e-06
%%%%% n 7 nvals 12 s 1
1 7 1.71429  1 1 1 1  4.34399e-06
1 7 1.71429  1 2 2 3  4.043e-06
1 7 1.71429  1 3 3 6  4.132e-06
%%%%% n 7 nvals 12 s 3
3 7 1.71429  1 1 1 1  4.346e-06
3 7 1.71429  1 2 2 3  4.03301e-06
3 7 1.71429  1 3 2 5  4.084e-06
%%%%% n 7 nvals 12 s 5
5 7 1.71429  1 1 1 1  4.36e-06
5 7 1.71429  1 2 1 2  3.87699e-06
%%%%% n 7 nvals 12 s 0
0 7 1.71429  1 1 1 1  4.38199e-06
0 7 1.71429  1 2 2 3  4.21201e-06
0 7 1.71429  1 3 3 6  4.259e-06
%%%%% n 7 nvals 12 s 1
1 7 1.71429  1 1 1 1  4.42e-06
1 7 1.71429  1 2 2 3  4.232e-06
1 7 1.71429  1 3 3 6  4.232e-06
%%%%% n 7 nvals 12 s 1
1 7 1.71429  1 1 1 1  4.30499e-06
1 7 1.71429  1 2 2 3  4.12999e-06
1 7 1.71429  1 3 3 6  4.27999e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  4.36901e-06
2 7 1.71429  1 2 1 2  3.92699e-06
%%%%% n 7 nvals 12 s 4
4 7 1.71429  1 1 1 1  4.30499e-06
4 7 1.71429  1 2 1 2  4.014e-06
4 7 1.71429  1 3 1 3  3.957e-06
%%%%% n 7 nvals 12 s 4
4 7 1.71429  1 1 1 1  4.437e-06
4 7 1.71429  1 2 1 2  4.161e-06
4 7 1.71429  1 3 1 3  4.05099e-06
%%%%% n 7 nvals 12 s 1
1 7 1.71429  1 1 1 1  4.30501e-06
1 7 1.71429  1 2 2 3  4.309e-06
1 7 1.71429  1 3 3 6  4.21501e-06
%%%%% n 7 nvals 12 s 3
3 7 1.71429  1 1 1 1  4.51e-06
3 7 1.71429  1 2 2 3  4.20801e-06
3 7 1.71429  1 3 2 5  4.06201e-06
%%%%% n 7 nvals 12 s 2
2 7 1.71429  1 1 1 1  4.38e-06
2 7 1.71429  1 2 1 2  4.02e-06
%%%%% n 7 nvals 12 s 6
6 7 1.71429  1 1 1 1  4.375e-06
6 7 1.71429  1 2 3 4  4.25399e-06
6 7 1.71429  1 3 2 6  4.118e-06
%%%%% n 7 nvals 12 s 4
4 7 1.71429  1 1 1 1  4.311e-06
4 7 1.71429  1 2 1 2  3.95199e-06
4 7 1.71429  1 3 1 3  4.047e-06
%%%%% n 7 nvals 12 s 5
5 7 1.71429  1 1 1 1  4.477e-06
5 7 1.71429  1 2 1 2  3.989e-06
] ;
