#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void bmpFilter(char *file, char *output_file)
{
    int i, n;
    FILE *f = fopen(file, "rb");
    FILE *out = fopen(output_file, "w");

    unsigned char header_buffer[54];

    fread(header_buffer, sizeof(unsigned char), 54, f);

    int width = *(int *)&header_buffer[18];
    int height = *(int *)&header_buffer[22];

    int image_size = 3 * width * height;

    unsigned char *data = (unsigned char *)malloc(image_size * sizeof(unsigned char));

    fread(data, sizeof(unsigned char), image_size, f);
    fclose(f);

    unsigned char avg;

    for (i = 0; i < image_size; i += 3)
    {
        avg = (data[i] + data[i + 1] + data[i + 2]) / 3;
        data[i] = avg;
        data[i + 1] = avg;
        data[i + 2] = avg;
    }

    n = fwrite(header_buffer, sizeof(unsigned char), 54, out);
    printf("%d bytes of the header were written \n", n);
    n = fwrite(data, sizeof(char), image_size, out);
    printf("%d bytes of content were written \n", n);
    free(data);
    fclose(out);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("./bmp [files_prefix] [number_of_files] \n");
        return -1;
    }
    int i, number_of_files;
    char input_name[15], output_name[15];
    number_of_files = atoi(argv[2]);

    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

#pragma omp parallel for private(input_name, output_name)
    for (i = 1; i <= number_of_files; i++)
    {
        sprintf(input_name, "%s%d.bmp", argv[1], i);
        sprintf(output_name, "output%d.bmp", i);
        printf("Filtering the image");
        bmpFilter(input_name, output_name);
    }

    gettimeofday(&t2, NULL);
    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);
    printf("Total time = %f\n", t_total);
    return 0;
}