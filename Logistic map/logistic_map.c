//ee25b063, ee25b038, ee25b110

#include <stdio.h>
#include <stdlib.h>

void plot_data();

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <rmin> <rmax> <rstep> <Niter>\n", argv[0]);
        return 1; 
    }

    double rmin = (double)atof(argv[1]);
    double rmax = (double)atof(argv[2]);
    double rstep = (double)atof(argv[3]);
    int Niter = atoi(argv[4]);

    FILE *outfile = fopen("data.txt", "w");
    if (outfile == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(outfile, "r           n         x\n");

    double prev_x = 0.5;
    double current_x;

    for (double r = rmin; r <= rmax; r += rstep) {
        
        prev_x = 0.5;

        for (int n = 0; n <= Niter; n++) {
            current_x = r * prev_x * (1.0 - prev_x);
            if (n > 1000) {
                fprintf(outfile, "%.4f          %d          %.6f\n", r, n, current_x);
            }
            
            prev_x = current_x;
        }
    }

    fclose(outfile);
    plot_data();

    return 0;
}

void plot_data() {
    printf("plotting...\n");

    FILE *gnuplotPipe = popen("gnuplot", "w");
    if (gnuplotPipe == NULL) {
        fprintf(stderr, "Gnuplot not found or could not be opened.\n");
        fprintf(stderr, "Please ensure gnuplot is installed and in your PATH.\n");
        return;
    }

    fprintf(gnuplotPipe, "set terminal pngcairo size 1400,1200\n");
    fprintf(gnuplotPipe, "set output 'logistic_map.jpg'\n");
    fprintf(gnuplotPipe, "set title 'Logistic Map'\n");
    fprintf(gnuplotPipe, "set xtics (2.4, 3.0, 3.6, 4.0)\n");
    fprintf(gnuplotPipe, "set xlabel 'r'\n");
    fprintf(gnuplotPipe, "set ylabel 'x'\n");
    fprintf(gnuplotPipe, "plot 'data.txt' using 1:3 with points pointtype 7 pointsize 0.4 lc rgb 'black' notitle\n");
    
    pclose(gnuplotPipe);
    
    printf("Plot saved to 'logistic_map.png'.\n");
}


