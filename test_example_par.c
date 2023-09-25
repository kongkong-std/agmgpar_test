#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void uni2dstrip(int, int, double *, double *, int *, int *,
                int, int, int *, int);
void dagmgpar_(int *, double *, int *, int *, double *, double *,
               int *, int *, int *, int *, double *, int *, int *, int *);

int main(int argc, char **argv)
{
    double *a = NULL, *f = NULL, *x = NULL;
    int *ja = NULL, *ia = NULL, *listrank = NULL;
    int n = 0, iter = 0, iprint = 0, nhinv = 0, nproc = 0, irank = 0,
        mx = 0, my = 0, ifirstlistrank = 0;
    double tol = 0.;
    char filename[32] = {0};

    nhinv = 1000;
    iter = 50;
    tol = 1.e-6;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &irank);

    iprint = 10;
    // sprintf(filename, "res_c.out_%02d", irank);
    // FILE *output_file = fopen(filename, "wb");

    mx = nhinv - 1;
    my = (nhinv - 1) / nproc;
    if (irank < (nhinv - 1) % nproc)
    {
        my += 1;
    }

    n = mx * my;
    ifirstlistrank = n + 1;
    a = (double *)malloc(5 * n * sizeof(double));
    ja = (int *)malloc(5 * n * sizeof(int));
    ia = (int *)malloc((n + 1) * sizeof(int));
    f = (double *)malloc(n * sizeof(double));
    x = (double *)malloc(n * sizeof(double));
    listrank = (int *)malloc((2 * mx + ifirstlistrank) * sizeof(int));

    for (int index = 0; index < 2 * mx; ++index)
    {
        listrank[index] = 0;
    }
    ifirstlistrank = n + 1;
    uni2dstrip(mx, my, f, a, ja, ia, irank, nproc,
               listrank, ifirstlistrank);

    int ijob = 0, nrest = 1;
    int comm = MPI_Comm_c2f(MPI_COMM_WORLD);
    // int comm = (MPI_COMM_WORLD);
#if 1
    dagmgpar_(&n, a, ja, ia, f, x, &ijob, &iprint, &nrest, &iter,
              &tol, &comm, listrank + ifirstlistrank - 1, &ifirstlistrank);
#endif

#if 1
    sprintf(filename, "sol_c.out_%02d", irank);
    FILE *solution_file = fopen(filename, "wb");
    for (int index = 0; index < n; ++index)
    {
        fprintf(solution_file, "%022.15le\n", x[index]);
    }
    fclose(solution_file);
#endif

#if 1
    free(a);
    free(ja);
    free(ia);
    free(f);
    free(x);
    free(listrank);
#endif

    // fclose(output_file);

    MPI_Finalize();
    return 0;
}

void uni2dstrip(int mx, int my, double *f, double *a,
                int *ja, int *ia, int irank, int nproc,
                int *listrank, int ifirstlistrank)
{
    int k = 0, l = 0;
    ia[0] = 1;
    double zero = 0., cx = -1., cy = -1., cd = 4.;

    for (int index_i = 0; index_i < my; ++index_i)
    {
        for (int index_j = 0; index_j < mx; ++index_j)
        {
            k += 1;
            l += 1;
            a[l - 1] = cd;
            ja[l - 1] = k;
            f[k - 1] = zero;

            // if-else 1
            if (index_j < mx - 1)
            {
                l += 1;
                a[l - 1] = cx;
                ja[l - 1] = k + 1;
            }
            else
            {
                f[k - 1] -= cx;
            }

            // if-else 2
            if (index_i < my - 1)
            {
                l += 1;
                a[l - 1] = cy;
                ja[l - 1] = k + mx;
            }
            else if (irank == (nproc - 1))
            {
                f[k - 1] -= cy;
            }
            else
            {
                l += 1;
                a[l - 1] = cy;
                ja[l - 1] = k + mx;
                listrank[k + mx - 1] = irank + 1; // index????
                // listrank[k - 1] = irank + 1; // index????
            }

            // if-else 3
            if (index_j > 0)
            {
                l += 1;
                a[l - 1] = cx;
                ja[l - 1] = k - 1;
            }
            else
            {
                f[k - 1] -= cx;
            }

            // if-else 4
            if (index_i > 0)
            {
                l += 1;
                a[l - 1] = cy;
                ja[l - 1] = k - mx;
            }
            else if (irank == 0)
            {
                f[k - 1] -= cy;
            }
            else
            {
                l += 1;
                a[l - 1] = cy;
                ja[l - 1] = k + mx * (my + 1);
                listrank[k + mx * (my + 1) - 1] = irank - 1;
            }

            ia[k] = l + 1;
        }
    }
}
